#include <iostream>

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

void initializeConnection() {
    // Initialize the connection
    illion::Connection::getInstance().init("orm_db", "localhost", "postgres", "3.141592");
}

void createEntry(UserModel& userModel) {
    std::unordered_map<std::string, std::string> entry = {
        {"username", "john_doe"},
        {"age", "25"},
        {"is_active", "true"},
        {"height", "1.75"},
    };
    userModel.addRecord(entry);
}

void getRecord() {
    UserModel& userModel = UserModel::getInstance();
    auto userRecord = userModel.getRecord("id", 1);
    std::cout << "User: " << userRecord["username"] << std::endl;
    std::cout << "Age: " << userRecord["age"] << std::endl;

}

void updateRecord() {
    UserModel& userModel = UserModel::getInstance();
    auto userRecord = userModel.getRecord("id", 1);
    userModel.updateField(userRecord["id"], "age", 50);
}

void filterRecords() {
    UserModel& userModel = UserModel::getInstance();
    illion::ResultIterator* result = userModel.filter("username", "john_doe", {"username", "id"});

    while (result->next()) {
        std::cout << "User: " << result->currentRecord["username"] << std::endl;
        std::cout << "Age: " << result->currentRecord["age"] << std::endl;
    }
}

int main() {

    initializeConnection();

    getRecord();

    return 0;
}
