//
// Created by chudonghao on 17-12-23.
//

#include "object.h"

namespace unnamed_game {

object_t::object_t() : type(type_e::undefined) {}

void set_parent(std::shared_ptr<object_t> &child, std::shared_ptr<object_t> &parent) {
    if (child) {
        if (child->parent) {
            child->parent->children.erase(child);
        } else {}
        child->parent = parent;
        if (parent) {
            parent->children.insert(child);
        } else {}
    } else {}
}

}

