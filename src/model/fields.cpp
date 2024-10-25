#include "model/fields.h"

namespace illion {

    // Define the fieldTypeToString map here
    std::unordered_map<FieldType, std::string> fieldTypeToString = {
        {INTEGER, "INTEGER"},
        {TEXT, "TEXT"},
        {FLOAT, "FLOAT"},
        {BOOLEAN, "BOOLEAN"}
    };

}