#include <gtest/gtest.h>

int divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

TEST(DivideTest, ThrowsOnZero) {
    EXPECT_THROW(divide(10, 0), std::invalid_argument);
}
