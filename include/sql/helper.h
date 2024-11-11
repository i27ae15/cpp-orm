#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

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

    class AntiSQLInjection {

        private:

            bool checkSet {};
            bool checkDrop {};
            bool checkComma {};
            bool checkAlter {};
            bool checkSelect {};
            bool checkWhiteSpaces {};
            bool checkSemiColom {};

            bool isValidSQL {};
            bool isTextValidated {};
            bool (AntiSQLInjection::*checkMethods[6])();

            std::string textToEvaluate {};
            std::string upperTextToEvaluate {};

            static AntiSQLInjection* instancePtr;

            // METHODS
            void setTextToEvaluate(std::string& text) {

                textToEvaluate = text;
                upperTextToEvaluate = text;

                std::transform(upperTextToEvaluate.begin(), upperTextToEvaluate.end(), upperTextToEvaluate.begin(), [](unsigned char c) {
                    return std::toupper(c);
                });
            }


        public:
            AntiSQLInjection(
                std::string textToEvaluate,
                bool checkSet = true,
                bool checkDrop = true,
                bool checkComma = true,
                bool checkAlter = true,
                bool checkSelect = true,
                bool checkWhiteSpaces = true,
                bool checkSemiColom = true,
                bool throwException = true
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

            };

        bool getIsValidSQL() {
            if (!isTextValidated) throw std::runtime_error("AntiSQLInjection::runChecks must be called first");
            return isValidSQL;

        }

        bool runChecks() {

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

        bool containsSymbol(std::string symbol) {

            size_t found = upperTextToEvaluate.find(symbol);
            if (found != std::string::npos) return false;

            return true;

        }

        bool containsSet() {
            if (!checkSet) return false;
            return containsSymbol("SET");
        }

        bool containsDrop() {
            if (!checkDrop) return false;
            return containsSymbol("DROP");
        }

        bool containsComma() {
            if (!checkComma) return false;
            return containsSymbol(",");
        }

        bool containsAlter() {
            if (!checkAlter) return false;
            return containsSymbol("ALTER");
        }

        bool containsSelect() {
            if (!checkSelect) return false;
            return containsSymbol("SELECT");
        }

        bool containsWhiteSpaces() {
            if (!checkWhiteSpaces) return false;
            return containsSymbol(" ");
        }

        bool containsWhiteSemiColom() {
            if (!checkSemiColom) return false;
            return containsSymbol(";");
        }

    };

}

