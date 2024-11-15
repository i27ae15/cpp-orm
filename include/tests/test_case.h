#ifndef ILLIONT_TEST_CASE_H
#define ILLIONT_TEST_CASE_H

#include <gtest/gtest.h>
#include <tests/db_management.h>

namespace illionT {
    class TestCase : public ::testing::Test {
    protected:
        static void SetUpTestSuite();
        static void TearDownTestSuite();
    };
}

#endif  // ILLIONT_TEST_CASE_H
