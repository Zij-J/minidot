// C-ish vector for speed and small code size

#ifndef VECTOR_H
#define VECTOR_H

#include "../error/error_handler.h"

template <typename type>
class Vector {
public:

    Vector() {}
    ~Vector() { free(contents); }

    int size() const {
        return num;
    }

    type &operator[](int index) const {
        ErrorHandler_FATAL_IF(index < 0 || index >= num, __FILE__, __LINE__);
        return contents[index];
    }

    void push_back(type new_data) {
        contents[num] = new_data;   // vector element add & expension
        ++num;
        if (num == max) {   
            max *= 2;
            contents = static_cast<type *>(realloc(contents, sizeof(type) * max));
        }
    }

    type pop_back() {
        --num;
        return contents[num]; // can't return references, things inside memory may be changed in the future
    }

    void remove(type element);

private:
    int num = 0;
    int max = 2;
    type *contents = static_cast<type *>(malloc(sizeof(type) * max));
};


#endif