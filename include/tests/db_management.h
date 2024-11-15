#pragma once
#include <string>
#include <sstream>
#include <iostream>

#include <db/connection.h>
#include <sql/helper.h>
#include <tests/utils.h>

namespace illionT {

    class DBManager {

        public:
            static DBManager& getInstance();

            void initializeConnection();
            void createTestDB(bool fRun = true);
            void dropTestDB();
            void setDbName(std::string dbName);

        private:

            std::string dbName = "illiont_testdb";

            DBManager();
            ~DBManager();

            DBManager(const DBManager&) = delete;
            DBManager& operator=(const DBManager&) = delete;

    };
}