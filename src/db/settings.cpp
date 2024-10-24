#include <string>
#include <sstream>
#include "db/settings.h"
#include <iostream>

namespace illion {

    DBSettings::DBSettings(
        std::string name,
        std::string host,
        std::string user,
        std::string password
    ) : name{name}, host{host}, user{user}, password{password}
    {
        setConnL();
    }

    DBSettings::~DBSettings() {}

    void DBSettings::setConnL() {

        std::stringstream ss;

        ss << "host=" << host
           << " port=5432"
           << " dbname=" << name
           << " user=" << user
           << " password=" << password;
        connL = ss.str();

    }

    std::string DBSettings::getName() {
        return name;
    }
    std::string DBSettings::getPassword() {
        return password;
    }
    std::string DBSettings::getHost() {
        return host;
    }
    std::string DBSettings::getUser() {
        return user;
    }
    std::string DBSettings::getConnL() {
        return connL;
    }

}