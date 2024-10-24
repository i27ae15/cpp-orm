#include <libpq-fe.h>
#include <string>

#include "db/settings.h"

namespace illion {

    class Connection {
        public:

        static Connection& getInstance();
        void init(
            std::string dbName,
            std::string dbHost,
            std::string dbUser,
            std::string dbPassword
        );
        void checkConnectionStatus();
        PGconn* getConnection();

        private:

            PGconn* conn;
            illion::DBSettings* dbSettings;

            Connection();
            ~Connection();
            void connect();


            Connection(const Connection&) = delete;
            Connection& operator=(const Connection&) = delete;

            void setSettings(
                std::string dbName,
                std::string dbHost,
                std::string dbUser,
                std::string dbPassword
            );


    };

}