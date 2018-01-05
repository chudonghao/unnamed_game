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
    glm::mat4 matrix_local;
    std::shared_ptr<bone_t> parent;
    std::set<std::shared_ptr<bone_t>> children;
};

void set_parent(std::shared_ptr<bone_t> &child, std::shared_ptr<bone_t> &parent);

}

#endif //UNNAMED_GAME_BONE_H
