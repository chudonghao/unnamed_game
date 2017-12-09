//
// Created by chudonghao on 17-12-4.
//

#ifndef UNTITLED_GAME_SCENE_H
#define UNTITLED_GAME_SCENE_H

#include <list>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <QtGui/QOpenGLContext>
#include "primitive_data.h"

namespace untitled_game {
class world_t {
public:
    typedef std::shared_ptr<world_t> ptr;
};

class render_layer_t {
public:
    typedef std::shared_ptr<render_layer_t> ptr;
};

class object_t {
public:
    typedef std::shared_ptr<object_t> ptr;
    enum class type_e {
        undefined,
        mesh,
        light,
        joint,
        skeleton
    };

    object_t(type_e type);

    virtual ~object_t() {}

    type_e type;
    glm::mat4 transformation;
    std::list<object_t::ptr> child_objects;
};

class material_t {
public:
    typedef std::shared_ptr<material_t> ptr;
};

class triangle_group_t {
public:
    //TODO 分开静态mesh物体和动态mesh物体
    material_t::ptr material;
    GLuint vao_id;
    GLuint element_buffer_id;
    GLuint element_count;
    std::vector<unsigned int> elements;
};


class modifier_t {
public:
    enum class type_e {
        skeleton_modifier
    };
    typedef std::shared_ptr<modifier_t> ptr;

    modifier_t(type_e type) : type(type) {}

    virtual ~modifier_t() = default;

    type_e type;
};

class mesh_t : public object_t {
public:
    typedef std::shared_ptr<mesh_t> ptr;

    mesh_t();

    //TODO 分开静态mesh物体和动态mesh物体
    GLuint position_buffer_id;
    GLuint normal_buffer_id;
    GLuint uvcoord_buffer_id;
    std::vector<triangle_group_t> triangles_groups;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvcoords;
    std::list<modifier_t::ptr> modifiers;
};

class light_t : public object_t {
public:
    light_t();

    typedef std::shared_ptr<light_t> ptr;
};

class joint_t : public object_t {
public:
    typedef std::shared_ptr<joint_t> ptr;

    joint_t();

    std::list<joint_t::ptr> child_joints;
public:
    glm::mat4 world_transformation;
};

class skeleton_t : public object_t {
public:
    typedef std::shared_ptr<skeleton_t> ptr;

    skeleton_t();

    std::list<joint_t::ptr> joints;
};


class skeleton_modifier_t : public modifier_t {
public:
    typedef std::shared_ptr<skeleton_modifier_t> ptr;

    skeleton_modifier_t();

    ~skeleton_modifier_t() override = default;

    //TODO use vertices group
    std::vector<joint_t::ptr> joints;
    std::vector<float> weights;
    std::vector<unsigned int> joint_indices;
    std::vector<unsigned int> weight_indices;
    std::vector<unsigned int> joint_count_per_position;
    std::vector<glm::mat4> inverse_bind_matrices;
    std::vector<glm::vec3> positions_with_BSM;
};

class scene_t {
public:
    typedef std::shared_ptr<scene_t> ptr;

    void render();

    void input_scene(const primitive_data::virsual_scene_t &primitive_virsual_scene);

private:
    object_t::ptr input_object(const primitive_data::node_t &primitive_node, int depth);

    skeleton_t::ptr input_skeleton(const primitive_data::node_t &primitive_node);

    joint_t::ptr input_joint(const primitive_data::node_t &primitive_node, int depth);

public:
    //world_t::ptr world;
    //std::list<render_layer_t::ptr> render_layers;
    std::list<object_t::ptr> objects;
    std::list<skeleton_t::ptr> skeletons;
private:
    std::map<size_t, joint_t::ptr> id_joint_map;
};
}

#endif //UNTITLED_GAME_SCENE_H
