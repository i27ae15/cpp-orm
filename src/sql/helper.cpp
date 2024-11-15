#include "sql/helper.h"
#include <tests/utils.h>

namespace SqlHelper {

    illion::ExecuteResult executeSQL(
        PGconn* conn,
        std::string& sql,
        std::string errorMessage,
        bool throwError,
        bool clearRes
    ) {
        PGresult* res = PQexec(conn, sql.c_str());
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK || PQresultStatus(res) == PGRES_TUPLES_OK);
        if (clearRes) PQclear(res);

        if (!errorMessage.size()) errorMessage = "Error while executing SQL:";

        if (!success) {
            if (throwError) {
                throw std::runtime_error(errorMessage + " " + PQerrorMessage(conn));
            } else {
                PRINT_ERROR(errorMessage << " " << PQerrorMessage(conn));
            }
        }

        return illion::ExecuteResult{success, PQerrorMessage(conn), res};
    }

    AntiSQLInjection::AntiSQLInjection(
        std::string textToEvaluate,
        bool checkSet,
        bool checkDrop,
        bool checkComma,
        bool checkAlter,
        bool checkSelect,
        bool checkWhiteSpaces,
        bool checkSemiColon,
        bool checkUpdate,
        bool throwException
    ) :
    isValidSQL {},
    isTextValidated {},
    failedSymbol {},
    textToEvaluate {},
    upperTextToEvaluate {},
    checkMethods  {
        &AntiSQLInjection::validateWhiteSpaces,
        &AntiSQLInjection::validateComma,
        &AntiSQLInjection::validateSemiColon,
        &AntiSQLInjection::validateDrop,
        &AntiSQLInjection::validateAlter,
        &AntiSQLInjection::validateSet,
        &AntiSQLInjection::validateSelect,
        &AntiSQLInjection::validateUpdate
    }
    {
        this->checkSet = checkSet;
        this->checkDrop = checkDrop;
        this->checkComma = checkComma;
        this->checkAlter = checkAlter;
        this->checkSelect = checkSelect;
        this->checkWhiteSpaces = checkWhiteSpaces;
        this->checkSemiColon = checkSemiColon;
        this->checkUpdate = checkUpdate;

        setTextToEvaluate(textToEvaluate);

        runChecks();
        if (!getIsValidSQL()) {
            PRINT_ERROR("Fail for " + failedSymbol);
            throw std::runtime_error("SQL INJECTION DETECTED");
        }

    }

    // METHODS
    void AntiSQLInjection::setTextToEvaluate(std::string& text) {

        textToEvaluate = text;
        upperTextToEvaluate = text;

        std::transform(upperTextToEvaluate.begin(), upperTextToEvaluate.end(), upperTextToEvaluate.begin(), [](unsigned char c) {
            return std::toupper(c);
        });
    }

    bool AntiSQLInjection::getIsValidSQL() {
        if (!isTextValidated) throw std::runtime_error("AntiSQLInjection::runChecks must be called first");
        return isValidSQL;

    }

    bool AntiSQLInjection::runChecks() {

        if (isTextValidated) return isValidSQL;

        isValidSQL = true;
        isTextValidated = true;

        for (auto checker : checkMethods) {
            bool passed = (this->*checker)();

            if (!passed) {
                isValidSQL = false;
                break;
            }
        }

        return isValidSQL;

    }

    bool AntiSQLInjection::validateSymbol(std::string symbol) {

        /**
        * @brief Validates if a specific symbol is absent in the `upperTextToEvaluate` string.
        *
        * This method checks if the `symbol` is present in `upperTextToEvaluate`. If found,
        * it logs an error message and returns `false`, indicating a validation failure.
        * If not found, it returns `true`, meaning the symbol is absent and the validation passes.
        *
        * @param symbol The string symbol to check for within `upperTextToEvaluate`.
        * @return `true` if `symbol` is not found in `upperTextToEvaluate`, `false` otherwise.
        */

        size_t found = upperTextToEvaluate.find(symbol);
        if (found != std::string::npos) {
            failedSymbol = symbol;
            return false;
        }

        return true;
    }

    bool AntiSQLInjection::validateSet() {
        if (!checkSet) return true;
        return validateSymbol("SET");
    }

    bool AntiSQLInjection::validateDrop() {
        if (!checkDrop) return true;
        return validateSymbol("DROP");
    }

    bool AntiSQLInjection::validateComma() {
        if (!checkComma) return true;
        return validateSymbol(",");
    }

    bool AntiSQLInjection::validateAlter() {
        if (!checkAlter) return true;
        return validateSymbol("ALTER");
    }

    bool AntiSQLInjection::validateSelect() {
        if (!checkSelect) return true;
        return validateSymbol("SELECT");
    }

    bool AntiSQLInjection::validateWhiteSpaces() {
        if (!checkWhiteSpaces) return true;
        return validateSymbol(" ");
    }

    bool AntiSQLInjection::validateSemiColon() {
        if (!checkSemiColon) return true;
        return validateSymbol(";");
    }

    bool AntiSQLInjection::validateUpdate() {
        if (!checkUpdate) return true;
        return validateSymbol("UPDATE");
    }

}

