#pragma once

#include <string>
#include <libpq-fe.h>

namespace illion {
    struct ExecuteResult {
        bool success;
        std::string errorMessage;
        PGresult* result;
    };
}