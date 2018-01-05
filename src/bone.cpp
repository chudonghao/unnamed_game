//
// Created by chudonghao on 17-12-23.
//

#include "bone.h"

namespace unnamed_game {

void set_parent(std::shared_ptr<bone_t> &child, std::shared_ptr<bone_t> &parent) {
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
