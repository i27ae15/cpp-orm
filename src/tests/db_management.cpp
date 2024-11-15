#include "tests/db_management.h"

namespace illionT {

    DBManager& DBManager::getInstance() {
        static DBManager instance;
        return instance;
    }

    DBManager::~DBManager() {
        // Not being deleted bd when finishing tests
        // dropTestDB();
    }

    DBManager::DBManager() {}

    void DBManager::setDbName(std::string dbName) {
        this->dbName = dbName;
    }

    void DBManager::initializeConnection() {
        // Initialize the connection
        illion::Connection::getInstance().init("postgres", "localhost", "postgres", "3.141592");
    }

    void DBManager::createTestDB(bool fRun) {

        PRINT_SUCCESS("Creating " + dbName);

        std::string sql = "CREATE DATABASE " + dbName;
        PGconn* conn = illion::Connection::getInstance().getConnection();
        illion::ExecuteResult result = SqlHelper::executeSQL(conn, sql, "Database Creation", false, true);

        if (!result.success && fRun) {
            dropTestDB();
            createTestDB(false);
            return;
        }

        PRINT_SUCCESS(dbName + " Created successfully");
    }

    void DBManager::dropTestDB() {

        PRINT_COLOR(YELLOW, "Dropping " + dbName);

        std::string sql = "DROP DATABASE " + dbName;
        PGconn* conn = illion::Connection::getInstance().getConnection();
        SqlHelper::executeSQL(conn, sql);
    }

}