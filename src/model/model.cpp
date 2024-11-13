#include <libpq-fe.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>

#include "model/model.h"
#include "model/fields.h"
#include "model/structs.h"

namespace illion {

    Model::Model() : isTableCreated(false), conn(nullptr) {
        setConnection();
    }
    Model::~Model() {
        clearConnection();
    }

    std::string Model::getTableName() {
        return appName + "__" + modelName;
    }

    std::string Model::getAppName() {
        return appName;
    }

    std::string Model::getModelName() {
        return modelName;
    }

    void Model::setAppName(std::string name) {
        appName = name;
    }

    void Model::setModelName(std::string name) {
        modelName = name;
    }

    void Model::setConnection() {
        conn = getConnection();
    }

    void Model::clearConnection() {
        conn = nullptr;
    }

    PGconn* Model::getConnection() {
        PGconn* conn = Connection::getInstance().getConnection();
        if (conn == nullptr) throw std::runtime_error("No valid database connection");

        return conn;
    }

    illion::ExecuteResult Model::executeSQL(std::string sql, std::string errorMessage, bool throwError, bool clearRes) {
        return SqlHelper::executeSQL(conn, sql, errorMessage, throwError, clearRes);
    }

    std::unordered_map<std::string, std::string> Model::executeGetQuery(const std::string& sql) {

        illion::ExecuteResult result = executeSQL(sql, "Error executing query", true, false);

        int numRows = PQntuples(result.result);

        if (numRows == 0) {
            PQclear(result.result);
            throw std::runtime_error("No records found");
        }
        if (numRows > 1) {
            PQclear(result.result);
            throw std::runtime_error("Multiple records found");
        }

        // Get the values
        std::unordered_map<std::string, std::string> record;
        for (int i = 0; i < PQnfields(result.result); i++) {
            record[PQfname(result.result, i)] = PQgetvalue(result.result, 0, i);
        }

        PQclear(result.result);
        return record;
    }

    void Model::addRecord(const std::unordered_map<std::string, std::string>& record) {

        std::stringstream sql;

        sql << "INSERT INTO " << getTableName() << " (";

        // Adding the column names
        for (auto it = fields.begin(); it != fields.end(); ++it) {
            sql << it->name;
            if (std::next(it) != fields.end()) sql << ", ";
        }

        sql << ") VALUES (";

        // Add values passed or default values
        for (auto it = fields.begin(); it != fields.end(); ++it) {
            auto valueIt = record.find(it->name);
            if (valueIt != record.end()) {

                SqlHelper::AntiSQLInjection* antiSQLInjection = new SqlHelper::AntiSQLInjection(valueIt->second);

                // This mean that we have a value for this field
                sql << "'" << valueIt->second << "'";
            } else {
                // We use the default value
                sql << it->defaultValue;
            }

            if (std::next(it) != fields.end()) sql << ", ";
        }

        sql << ");";

        std::string errorMessage = "Error adding record to " + getTableName();
        executeSQL(sql.str(), errorMessage);

        std::cout << "Record added successfully \n";

    }

    bool Model::fieldExistsInDB(const std::string& fieldName) {
        SqlHelper::AntiSQLInjection* antiSQLInjection = new SqlHelper::AntiSQLInjection(fieldName);

        PGconn* conn = getConnection();

        // Query to check if the column exists in the table

        std::string sql = "SELECT column_name FROM information_schema.columns WHERE EXISTS (SELECT column_name FROM " + getTableName() + " WHERE column_name '" + fieldName + "';";

        PGresult* res = PQexec(conn, sql.c_str());
        bool exists = (PQntuples(res) > 0);
        PQclear(res);

        return exists;

    }

    void Model::addColumns() {

        for (const illion::Field& field : fields) {

            if (fieldExistsInDB(field.name)) continue;

            auto it = fieldTypeToString.find(field.type);

            if (it == fieldTypeToString.end()) throw std::runtime_error("Unknown field type");

            std::string fieldType = it->second;

            SqlHelper::AntiSQLInjection* antiSQLInjection = new SqlHelper::AntiSQLInjection(fieldType);
            SqlHelper::AntiSQLInjection* antiSQLInjection1 = new SqlHelper::AntiSQLInjection(field.name);

            std::string sql = "ALTER TABLE " + getTableName() + " ADD COLUMN " + field.name + " " + fieldType;

            std::string errorMessage = "Error Adding field: " + field.name;
            executeSQL(sql, errorMessage);
            std::cerr << "Field: '" << field.name << "' added successfully \n";

        }
    }

    void Model::createTable() {

        if (isTableCreated) return;

        std::string sql = "CREATE TABLE " + getTableName() + " (id SERIAL PRIMARY KEY);";

        executeSQL(sql, "Table creation failed");

        std::cerr << "Table created successfully \n";

        isTableCreated = true;

    }

    void Model::save() {}

    void Model::makeMigrations() {
        setConnection();

        createTable();
        addColumns();

        clearConnection();
    }
}