#pragma once
#include <string>

namespace illion {
    class DBSettings {
        public:
            DBSettings(
                std::string name,
                std::string host,
                std::string user,
                std::string password
            );
            virtual ~DBSettings();

            std::string getName();
            std::string getPassword();
            std::string getHost();
            std::string getUser();
            std::string getConnL();

        private:

            void setConnL();

            std::string connL;
            const std::string name;
            const std::string password;
            const std::string host;
            const std::string user;
    };
}
