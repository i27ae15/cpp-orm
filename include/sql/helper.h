#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <libpq-fe.h>
#include <iostream>
#include <type_traits>
#include "model/structs.h"


namespace SqlHelper {

    template<typename T>
    std::string buildSelectWhere(
        const std::string& tableName,
        const std::string& fieldName,
        const T& match,
        const std::vector<std::string>& mFields = {}
    ) {
        std::ostringstream sql;

        if (mFields.empty()) {
            sql << "SELECT * ";
        } else {
            sql << "SELECT ";
            for (size_t i {}; i < mFields.size(); i++) {
                sql << mFields[i];
                if (i < mFields.size() - 1) sql << ", ";
            }
            sql << " ";
        }

        sql << "FROM " << tableName << " WHERE " << fieldName << " = ";

        if constexpr (std::is_convertible_v<T, std::string> || std::is_same_v<T, const char*>) {
            sql << "'" << match << "'";
        } else if constexpr (std::is_same<T, bool>::value) {
            sql << (match ? "true" : "false");
        } else {
            sql << match;
        }

        sql << ";";

        return sql.str();
    }

    illion::ExecuteResult executeSQL(
        PGconn* conn,
        std::string& sql,
        std::string errorMessage = "",
        bool throwError = true,
        bool clearRes = true
    );

    class AntiSQLInjection {

        private:

            bool checkSet;
            bool checkDrop;
            bool checkComma;
            bool checkAlter;
            bool checkSelect;
            bool checkWhiteSpaces;
            bool checkSemiColon;
            bool checkUpdate;

            bool isValidSQL;
            bool isTextValidated;
            bool (AntiSQLInjection::*checkMethods[8])();

            std::string failedSymbol;

            std::string textToEvaluate;
            std::string upperTextToEvaluate;

            static AntiSQLInjection* instancePtr;

            // METHODS
            void setTextToEvaluate(std::string& text);

        public:
            AntiSQLInjection(
                std::string textToEvaluate,
                bool checkSet = true,
                bool checkDrop = true,
                bool checkComma = true,
                bool checkAlter = true,
                bool checkSelect = true,
                bool checkWhiteSpaces = true,
                bool checkSemiColon = true,
                bool checkUpdate = true,
                bool throwException = true
            );

            bool getIsValidSQL();

            bool runChecks();

            bool validateSymbol(std::string symbol);

            bool validateSet();

            bool validateDrop();

            bool validateComma();

            bool validateAlter();

            bool validateSelect();

            bool validateWhiteSpaces();

            bool validateSemiColon();

            bool validateUpdate();
    };

}

