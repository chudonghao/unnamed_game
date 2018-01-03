//
// Created by chudonghao on 17-12-23.
//

#ifndef UNNAMED_GAME_BONE_H
#define UNNAMED_GAME_BONE_H

#include <memory>
#include <list>
#include <glm/glm.hpp>
#include <set>
#include "data_block.h"

namespace unnamed_game {

class bone_t : public data_block_t {
public:
    typedef std::shared_ptr<bone_t> shared_ptr;
    glm::mat4 matrix_local;
    bone_t::shared_ptr parent;
    std::set<bone_t::shared_ptr> children;
};

void set_parent(bone_t::shared_ptr &child, bone_t::shared_ptr &parent);

}

#endif //UNNAMED_GAME_BONE_H
