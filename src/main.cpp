#include "model/model.h"
#include "model/types.h"


class UserModel : public illion::Model {
    public:
        UserModel() {
            setAppName("app");
            setModelName("fourthModel");
        }
};

int main() {
    // Initialize the connection
    illion::Connection::getInstance().init("orm_db", "localhost", "postgres", "3.141592");

    // Create an instance of UserModel
    UserModel userModel;

    // Add new fields to the model
    std::vector<illion::Field> fields = {
        illion::Field("username", illion::FieldType::TEXT, 50),  // VARCHAR(50)
        illion::Field("age", illion::FieldType::INTEGER)
    };

    userModel.addField(fields);

    // Save changes (add fields to the table)
    userModel.save();

    return 0;
}
