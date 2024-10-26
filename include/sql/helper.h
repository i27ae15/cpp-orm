#include <string>
#include <sstream>

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
}