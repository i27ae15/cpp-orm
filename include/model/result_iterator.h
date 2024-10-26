#include <libpq-fe.h>
#include <string>
#include <unordered_map>
#include <iostream>

namespace illion {
    class ResultIterator {
        public:
            ResultIterator(PGresult* result);
            ~ResultIterator();

            std::unordered_map<std::string, std::string> currentRecord;

            bool next();
            bool previous();


            std::string getFieldValue(int columnIndex);
            int getFieldCount();

        private:
            PGresult* result;
            int currentRow_;
            int totalRows_;

            // methods
            void setCurrentRecord();
    };
}
