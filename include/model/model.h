#pragma once

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <libpq-fe.h>

#include "db/connection.h"

#include "sql/helper.h"

#include "model/fields.h"
#include "model/structs.h"
#include "model/result_iterator.h"


namespace illion {

    class Model {
        public:

            // Constructor
            Model();
            ~Model();

            // Methods
            std::string getModelName();
            std::string getAppName();
            std::string getTableName();

            bool fieldExistsInDB(const std::string& fieldName);

            void save();
            void makeMigrations();
            void setAppName(std::string name);
            void setModelName(std::string name);
            void addRecord(const std::unordered_map<std::string, std::string>& record);

            template<typename T>
            std::unordered_map<std::string, std::string> getRecord(const std::string& fieldName, const T match) {
                std::string sql = SqlHelper::buildSelectWhere(getTableName(), fieldName, match);
                return executeGetQuery(sql);
            }

            template<typename T>
            void updateField(const std::string id, const std::string& fieldName, const T value) {
                std::ostringstream sql;
                sql << "UPDATE " << getTableName() << " SET " << fieldName << " = " << value << " WHERE id = " << id << ";";
                executeSQL(sql.str(), "Failed to update field");
            }

            template<typename T>
            ResultIterator* filter(
                const std::string& fieldName,
                const T& match,
                const std::vector<std::string>& mFields = {}
                ) {
                std::string sql = SqlHelper::buildSelectWhere(getTableName(), fieldName, match, mFields);
                ExecuteResult result = executeSQL(sql, "Failed to filter records", true, false);
                return new ResultIterator(result.result);
            }

        private:

            // Attributes
            bool isTableCreated;
            PGconn* conn;
            std::string appName;
            std::string modelName;

            // Methods
            PGconn* getConnection();

            void setConnection();
            void clearConnection();

            std::unordered_map<std::string, std::string> executeGetQuery(const std::string& sql);
            ExecuteResult executeSQL(std::string sql, std::string errorMessage, bool throwError = true, bool clearRes = true);

        protected:
            // Attributes
            std::vector<illion::Field> fields;

            void addColumns();
            void createTable();
    };

}
