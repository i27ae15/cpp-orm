#include <iostream>

#include "tests/test_case.h"
#include "model/model.h"
#include "model/fields.h"


class UserModel : public illion::Model {
    public:
        static UserModel& getInstance() {
            static UserModel instance;
            return instance;
        }

        private:
        UserModel() {
            setAppName("app");
            setModelName("07");

            // DEFINING THE FIELDS
            DEFINE_FIELD("username", illion::FieldType::TEXT, 255, "default_username");
            DEFINE_FIELD("age", illion::FieldType::INTEGER);
            DEFINE_FIELD("is_active", illion::FieldType::BOOLEAN, false);
            DEFINE_FIELD("height", illion::FieldType::FLOAT);
        }

        // Disallow copying
        UserModel(const UserModel&) = delete;
        UserModel& operator=(const UserModel&) = delete;

};

std::unordered_map<std::string, std::string> createEntry(UserModel& userModel) {
    std::unordered_map<std::string, std::string> entry = {
        {"username", "john_doe"},
        {"age", "25"},
        {"is_active", "true"},
        {"height", "1.75"},
    };
    userModel.addRecord(entry);
    return entry;
}

std::unordered_map<std::string, std::string> getRecord() {
    UserModel& userModel = UserModel::getInstance();
    auto userRecord = userModel.getRecord("id", 1);
    return userRecord;
}

template<typename T>
void updateRecord(std::pair<std::string, T> toUpdate) {
    UserModel& userModel = UserModel::getInstance();
    auto userRecord = userModel.getRecord("id", 1);
    userModel.updateField(userRecord["id"], toUpdate.first, toUpdate.second);
}

illion::ResultIterator* filterRecords() {
    UserModel& userModel = UserModel::getInstance();
    return userModel.filter("username", "john_doe", {"username", "id"});
}

bool testModel() {

    UserModel& model = UserModel::getInstance();
    model.makeMigrations();

    std::unordered_map<std::string, std::string> recordReturned {};

    std::unordered_map<std::string, std::string> entryCreated = createEntry(model);
    recordReturned = getRecord();

    // Testing get record
    for (const auto& pair : entryCreated) {
        if (pair.second != recordReturned[pair.first]) return false;
    }

    // Update
    std::pair<std::string, int> toUpdate("age", 25);
    entryCreated["age"] = 25;

    updateRecord(toUpdate);
    recordReturned = getRecord();

    // Testing get record
    for (const auto& pair : entryCreated) {
        if (pair.second != recordReturned[pair.first]) return false;
    }

    // Testing filter
    illion::ResultIterator* filter = filterRecords();
    filter->next();

    for (const auto& pair : entryCreated) {
        if (pair.second != filter->currentRecord[pair.first]) return false;
    }

    return true;
}

namespace illionT {
    TEST_F(TestCase, TestBasicModel) {
        EXPECT_TRUE(testModel);
    }
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
