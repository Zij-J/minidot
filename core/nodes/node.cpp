#include "node.h"

Node::~Node() {
    is_deleting = true; 
    if (parent != nullptr && parent->is_deleting == false) { // info parent about deletion
        parent->children.remove(this);
    }

    for (int i = 0; i < children.size(); ++i) {
        delete children[i];
    }
}


void Node::add_child(Node *new_node) {
    children.push_back(new_node);

    new_node->parent = this;        // info child about parent + is in tree
    new_node->tree_entered.notify();
}