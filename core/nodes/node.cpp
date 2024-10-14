#include "node.h"

Node::~Node() {
    is_deleting = true; 
    if (parent != nullptr && parent->is_deleting == false) { // info parent about deletion
        parent->childs.remove(this);
    }

    for (int i = 0; i < childs.size(); ++i) {
        delete childs[i];
    }
}


void Node::add_child(Node *new_node) {
    childs.push_back(new_node);

    new_node->parent = this;        // info child about parent + is in tree
    new_node->tree_entered.notify();
}