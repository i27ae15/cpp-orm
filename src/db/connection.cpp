#include <iostream>
#include <libpq-fe.h>
#include <string>

#include "db/settings.h"
#include "db/connection.h"

namespace illion {
    Connection& Connection::getInstance() {
        static Connection instance;
        return instance;
    }

    Connection::Connection()
    : dbSettings(nullptr), conn(nullptr) {}

    Connection::~Connection() {
        if (conn) PQfinish(conn);
    }

    void Connection::init(
        std::string dbName,
        std::string dbHost,
        std::string dbUser,
        std::string dbPassword
    )
    {
        setSettings(dbName, dbHost, dbUser, dbPassword);
        connect();
    }

    void Connection::setSettings(
        std::string dbName,
        std::string dbHost,
        std::string dbUser,
        std::string dbPassword
    ) {

        if (dbSettings) return;
        dbSettings = new illion::DBSettings(dbName, dbHost, dbUser, dbPassword);
    }

    void Connection::connect() {

        if (conn) return;
        conn = PQconnectdb(dbSettings->getConnL().c_str());
        checkConnectionStatus();
    }

    void Connection::checkConnectionStatus() {
        if (PQstatus(conn) == CONNECTION_BAD) {
            std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
            PQfinish(conn);
            exit(1);
        } else {
            std::cout << "Connected to the database successfully" << std::endl;
        }
    }

    PGconn* Connection::getConnection() {
        return conn;
    }

}