//
// Created by chudonghao on 17-12-23.
//

#ifndef UNTITLED_GAME_OBJECT_H
#define UNTITLED_GAME_OBJECT_H

#include <list>
#include <glm/glm.hpp>
#include <set>
#include "has_id.h"

namespace untitled_game {

class object_t : public has_id_t {
public:
    typedef std::shared_ptr<object_t> shared_ptr;
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
    object_t::shared_ptr parent;
    std::set<object_t::shared_ptr> children;
    data_block_t::shared_ptr data;
};

void set_parent(object_t::shared_ptr &child, object_t::shared_ptr &parent);

}

#endif //UNTITLED_GAME_OBJECT_H
