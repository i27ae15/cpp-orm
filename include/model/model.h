#pragma once

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <libpq-fe.h>

#include "model/fields.h"
#include "db/connection.h"
#include "model/structs.h"


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
            std::unordered_map<std::string, std::string> getRecord(const std::string& fieldName, T match) {

                std::ostringstream sql;
                sql << "SELECT * FROM " << getTableName() << " WHERE " << fieldName << " = ";

                if constexpr (std::is_same<T, std::string>::value) {
                    sql << "'" << match << "'";
                } else if constexpr (std::is_same<T, bool>::value) {
                    sql << (match ? "true" : "false");
                } else {
                    sql << match;
                }

                sql << ";";

                return executeGetQuery(sql.str());
            }
            // void updateField(const std::string& fieldName, const std::string& value);

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
