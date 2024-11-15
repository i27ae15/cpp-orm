#include <gtest/gtest.h>
#include <sql/helper.h>
#include <stdexcept>

TEST(ValidSQL, ValidSQLT) {

    std::string validSQL = "students";
    SqlHelper::AntiSQLInjection antiSqlInjection(validSQL);
    EXPECT_TRUE(antiSqlInjection.getIsValidSQL());
}

TEST(InvalidSQL, InvalidSQLT) {

    std::vector<std::string> invalidSymbols = {
        "SET", "set", ";", "hello ", "Update", "alTeR", "Drop", "select"
    };

    for (const std::string& symbol : invalidSymbols) {
        EXPECT_THROW({
            SqlHelper::AntiSQLInjection antiSqlInjection(symbol);
        }, std::runtime_error);  // Replace `std::exception_type` with the specific exception class expected
    }
}