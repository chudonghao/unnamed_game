//
// Created by chudonghao on 17-12-4.
//

#ifndef UNTITLED_GAME_SCENE_H
#define UNTITLED_GAME_SCENE_H

#include <set>
#include <list>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <QtGui/QOpenGLContext>
#include "primitive_data.h"
#include "data_block.h"
#include "object.h"
#include "joint.h"
#include "material.h"
#include "camera.h"

namespace untitled_game {

class world_t {
public:
    typedef std::shared_ptr<world_t> shared_ptr;
};

class render_layer_t {
public:
    typedef std::shared_ptr<render_layer_t> shared_ptr;
};

class modifier_t {
public:
    enum class type_e {
        skeleton_modifier
    };
    typedef std::shared_ptr<modifier_t> shared_ptr;

    modifier_t(type_e type) : type(type) {}

    virtual ~modifier_t() = default;

    type_e type;
};

class skeleton_t : public object_t {
public:
    typedef std::shared_ptr<skeleton_t> shared_ptr;

    skeleton_t();

    std::list<joint_t::shared_ptr> joints;
};


class skeleton_modifier_t : public modifier_t {
public:
    typedef std::shared_ptr<skeleton_modifier_t> shared_ptr;

    skeleton_modifier_t();

    ~skeleton_modifier_t() override = default;

    //TODO use vertices group
    std::vector<joint_t::shared_ptr> joints;
    std::vector<float> weights;
    std::vector<unsigned int> joint_indices;
    std::vector<unsigned int> weight_indices;
    std::vector<unsigned int> joint_count_per_position;
    std::vector<glm::mat4> inverse_bind_matrices;
    std::vector<glm::vec3> positions_with_BSM;
};

class scene_t : public has_id_t {
public:
    typedef std::shared_ptr<scene_t> shared_ptr;
    /**
     * active camera
     */
    object_t::shared_ptr active_camera;
    std::set<object_t::shared_ptr> root_objects;
};

void render(scene_t::shared_ptr scene);

}

#endif //UNTITLED_GAME_SCENE_H
