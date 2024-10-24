#pragma once

#include <string>
#include <vector>

#include "model/types.h"
#include "db/connection.h"

namespace illion {

    class Model {
        public:
            Model();
            ~Model();

            std::string getModelName();
            std::string getAppName();

            void setAppName(std::string name);
            void setModelName(std::string name);

            std::string getTableName();
            void addField(std::vector<illion::Field>);
            void save();

        private:
            PGconn* getConnection();

            void createTable();

            std::string appName;
            std::string modelName;
            std::vector<illion::Field> fields;
            std::vector<illion::Field> fieldsToAdd;

            bool isTableCreated;
    };

}
