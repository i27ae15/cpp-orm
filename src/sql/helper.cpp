#include "sql/helper.h"

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

        if (!errorMessage.size()) errorMessage = "Error while executing SQL";

        if (!success) {
            if (throwError) {
                throw std::runtime_error(errorMessage + ": " + PQerrorMessage(conn));
            } else {
                std::cerr << "Error while executing SQL: " << PQerrorMessage(conn) << "\n";
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
        bool checkSemiColom,
        bool throwException
    ) :
    checkMethods  {
        &AntiSQLInjection::containsWhiteSpaces,
        &AntiSQLInjection::containsComma,
        &AntiSQLInjection::containsDrop,
        &AntiSQLInjection::containsAlter,
        &AntiSQLInjection::containsSet,
        &AntiSQLInjection::containsSelect
    }
    {
        this->checkSet = checkSet;
        this->checkDrop = checkDrop;
        this->checkComma = checkComma;
        this->checkAlter = checkAlter;
        this->checkSelect = checkSelect;
        this->checkWhiteSpaces = checkWhiteSpaces;
        this->checkSemiColom = checkSemiColom;

        setTextToEvaluate(textToEvaluate);

        runChecks();
        if (!getIsValidSQL()) throw std::runtime_error("SQL INJECTION DETECTED");

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
            bool result = (this->*checker)();

            if (result) {
                isValidSQL = false;
                break;
            }
        }

        return isValidSQL;

    }

    bool AntiSQLInjection::containsSymbol(std::string symbol) {

        size_t found = upperTextToEvaluate.find(symbol);
        if (found != std::string::npos) return false;

        return true;

    }

    bool AntiSQLInjection::containsSet() {
        if (!checkSet) return false;
        return containsSymbol("SET");
    }

    bool AntiSQLInjection::containsDrop() {
        if (!checkDrop) return false;
        return containsSymbol("DROP");
    }

    bool AntiSQLInjection::containsComma() {
        if (!checkComma) return false;
        return containsSymbol(",");
    }

    bool AntiSQLInjection::containsAlter() {
        if (!checkAlter) return false;
        return containsSymbol("ALTER");
    }

    bool AntiSQLInjection::containsSelect() {
        if (!checkSelect) return false;
        return containsSymbol("SELECT");
    }

    bool AntiSQLInjection::containsWhiteSpaces() {
        if (!checkWhiteSpaces) return false;
        return containsSymbol(" ");
    }

    bool AntiSQLInjection::containsWhiteSemiColom() {
        if (!checkSemiColom) return false;
        return containsSymbol(";");
    }

}

