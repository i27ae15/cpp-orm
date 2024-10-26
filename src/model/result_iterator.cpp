#include "model/result_iterator.h"

namespace illion {

    ResultIterator::ResultIterator(PGresult* result)
    :
    result{result},
    currentRow_{},
    totalRows_{PQntuples(result)},
    currentRecord{}
    {}

    ResultIterator::~ResultIterator() {
        PQclear(result);
    }

    void ResultIterator::setCurrentRecord() {
        for (size_t i {}; i < PQnfields(result); i++) {
            currentRecord[PQfname(result, i)] = PQgetvalue(result, currentRow_, i);
        }
    }

    bool ResultIterator::next() {


        if (currentRow_ >= totalRows_) return false;

        setCurrentRecord();

        currentRow_++;
        return true;
    }

    bool ResultIterator::previous() {

        if (currentRow_ == 0) return false;

        setCurrentRecord();

        currentRow_--;
        return true;
    }

    std::string ResultIterator::getFieldValue(int columnIndex) {
        return PQgetvalue(result, currentRow_, columnIndex);
    }

    int ResultIterator::getFieldCount() {
        return PQnfields(result);
    }

}
