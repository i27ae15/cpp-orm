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

        Field(const std::string& name, FieldType type, int size = 0)
            : name(name), type(type), size(size) {}
    };
}
