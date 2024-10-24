#include <libpq-fe.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>

#include "model/model.h"
#include "model/types.h"

namespace illion {

    Model::Model() : isTableCreated{}, fieldsToAdd{} {};
    Model::~Model() {};

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

    PGconn* Model::getConnection() {
        PGconn* conn = Connection::getInstance().getConnection();
        if (conn == nullptr) throw std::runtime_error("No valid database connection");

        return conn;
    }

    void Model::addField(std::vector<illion::Field> fields) {

        for (illion::Field f : fields) {
            fieldsToAdd.push_back(f);
        }

    }

    void Model::createTable() {

        if (isTableCreated) return;

        PGconn* conn = getConnection();
        std::string sql = "CREATE TABLE " + getTableName() + " (id SERIAL PRIMARY KEY);";

        PGresult* res = PQexec(conn, sql.c_str());

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Error: " << PQerrorMessage(conn) << "\n";
            throw std::runtime_error("Table creation failed");
        }

        std::cerr << "Table created successfully \n";

        PQclear(res);
        isTableCreated = true;

    }

    void Model::save() {
        if (!isTableCreated) createTable();

        PGconn* conn = getConnection();

        for (const illion::Field& field : fieldsToAdd) {
            auto it = fieldTypeToString.find(field.type);

            if (it == fieldTypeToString.end()) throw std::runtime_error("Unknown field type");

            std::string fieldType = it->second;
            std::string sql = "ALTER TABLE " + getTableName() + " ADD COLUMN " + field.name + " " + fieldType;

            PGresult* res = PQexec(conn, sql.c_str());

            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::cerr << "Error adding field: " << PQerrorMessage(conn) << "\n";
                PQclear(res);
                throw std::runtime_error("Failed to add field: " + field.name);
            }

            PQclear(res);
            std::cerr << "Field: '" << field.name << "' added successfully \n";
        }

        fieldsToAdd.clear();
    }



}