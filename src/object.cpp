//
// Created by chudonghao on 17-12-23.
//

#include "object.h"

namespace untitled_game {

object_t::object_t() : type(type_e::undefined) {}

void set_parent(object_t::shared_ptr &child, object_t::shared_ptr &parent) {
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

