#include "vector.h"
#include "../nodes/node.h"

template <typename type>
void Vector<type>::remove(type element) {
    int i = 0;
    for (;i < num; ++i) {
        if (contents[i] == element) { // found, delete
            --num;
            break;
        }
    }
    for (; i <= num; ++i) { // not found: skip, found: move elements
        contents[i] = contents[i + 1];
    }
}

template void Vector<Node *>::remove(Node *element);  // implement template as Node *