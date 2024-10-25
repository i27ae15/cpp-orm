#pragma once
#include <unordered_map>
#include <string>

namespace illion {
    enum FieldType {
        INTEGER,
        TEXT,
        FLOAT,
        BOOLEAN,
    };

    extern std::unordered_map<FieldType, std::string> fieldTypeToString;

    // Struct to represent a field (with optional size for VARCHAR)
    struct Field {
        std::string name;
        FieldType type;
        int size;  // Only applicable for VARCHAR
        std::string defaultValue;

        Field(
            const std::string& name,
            FieldType type,
            int size = 0,
            const std::string& defaultValue = ""
        )
            : name(name), type(type), size(size), defaultValue(defaultValue) {}
    };

    #define DEFINE_FIELD_2(name, type) \
    fields.push_back(illion::Field(name, type, 0, (type == illion::FieldType::TEXT ? "" : (type == illion::FieldType::BOOLEAN ? "false" : "0"))))

    #define DEFINE_FIELD_3(name, type, size) \
    fields.push_back(illion::Field(name, type, size, (type == illion::FieldType::TEXT ? "" : (type == illion::FieldType::BOOLEAN ? "false" : "0"))))

    #define DEFINE_FIELD_4(name, type, size, defaultValue) \
    fields.push_back(illion::Field(name, type, size, defaultValue))

    #define GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
    #define DEFINE_FIELD(...) \
    GET_MACRO(__VA_ARGS__, DEFINE_FIELD_4, DEFINE_FIELD_3, DEFINE_FIELD_2)(__VA_ARGS__)

}
