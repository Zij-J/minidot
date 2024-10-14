// node & node tree for resource handling

#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include "../templates/vector.h"
#include "../templates/delegate.h"


class Node {    
public:
    Delegate tree_entered;

    Node() {}
    virtual ~Node();

    const Vector<Node *> &get_childs() const {
        return childs;
    }
    const Node *get_parent() const {
        return parent;
    }
    
    void add_child(Node *new_node);

private:
    bool is_deleting = false;
    Node *parent = nullptr;
    Vector<Node *> childs;
};


#endif