//
// Created by chudonghao on 17-12-23.
//

#ifndef UNNAMED_GAME_OBJECT_H
#define UNNAMED_GAME_OBJECT_H

#include <list>
#include <glm/glm.hpp>
#include <set>
#include "has_id.h"

namespace unnamed_game {

class object_t : public has_id_t {
public:

    enum class type_e {
        undefined,
        armature,
        camera,
        lamp,
        mesh,
    };


    object_t();

    ~object_t() override {}

    type_e type;
    union {
        glm::mat4 transformation;
        glm::mat4 matrix_local;
    };
    glm::mat4 matrix_world;
    std::shared_ptr<object_t> parent;
    std::set<std::shared_ptr<object_t>> children;
    std::shared_ptr<has_id_t> data;
};

void set_parent(std::shared_ptr<object_t> &child, std::shared_ptr<object_t> &parent);

}

#endif //UNNAMED_GAME_OBJECT_H
