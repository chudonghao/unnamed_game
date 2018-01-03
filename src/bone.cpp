//
// Created by chudonghao on 17-12-23.
//

#include "bone.h"

namespace unnamed_game {

void set_parent(bone_t::shared_ptr &child, bone_t::shared_ptr &parent) {
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
