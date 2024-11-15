#include <tests/test_case.h>

namespace illionT {

    void TestCase::SetUpTestSuite() {
        illionT::DBManager& instance = illionT::DBManager::getInstance();
        instance.initializeConnection();
        instance.createTestDB();
    }

    void TestCase::TearDownTestSuite() {
        illionT::DBManager& instance = illionT::DBManager::getInstance();
        instance.dropTestDB();  // Explicitly call the destructor to clea
    }

}

