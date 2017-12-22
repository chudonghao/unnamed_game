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
#include "log.h"

namespace untitled_game {

class node_t {

};

class node_factory_t {
public:
    template<typename T, typename ... ARG_T>
    static std::shared_ptr<T> create(const std::string &uri, ARG_T &&... arg);

    template<typename T>
    static std::shared_ptr<T> locate(const std::string &uri);

    static std::map<std::string, std::shared_ptr<node_t>> uri_node_map;
};

template<typename T, typename... ARG_T>
std::shared_ptr<T> node_factory_t::create(const std::string &uri, ARG_T &&... arg) {
    if (uri_node_map.find(uri) != uri_node_map.end()) {
        LOG_W << "uri_node_map already has node(uri=" << uri << ").";
        return nullptr;
    } else {
        auto node = std::shared_ptr<T>(new T(arg ...));
        uri_node_map[uri] = node;
        return node;
    }
}

template<typename T>
std::shared_ptr<T> node_factory_t::locate(const std::string &uri) {
    auto iter = uri_node_map.find(uri);
    if (iter != uri_node_map.end()) {
        return std::static_pointer_cast<T>(*iter);
    }
    return nullptr;
}

class world_t {
public:
    typedef std::shared_ptr<world_t> shared_ptr;
};

class render_layer_t {
public:
    typedef std::shared_ptr<render_layer_t> shared_ptr;
};

class object_t {
public:
    typedef std::shared_ptr<object_t> shared_ptr;
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
    std::list<object_t::shared_ptr> child_objects;
};

class material_t {
public:
    typedef std::shared_ptr<material_t> shared_ptr;
};

class triangles_group_t {
public:
    //TODO 分开静态mesh物体和动态mesh物体
    material_t::shared_ptr material;
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
    typedef std::shared_ptr<modifier_t> shared_ptr;

    modifier_t(type_e type) : type(type) {}

    virtual ~modifier_t() = default;

    type_e type;
};

class light_t : public object_t {
public:
    light_t();

    typedef std::shared_ptr<light_t> shared_ptr;
};

class joint_t : public object_t {
public:
    typedef std::shared_ptr<joint_t> shared_ptr;

    joint_t();

    std::list<joint_t::shared_ptr> child_joints;
public:
    glm::mat4 world_transformation;
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

class scene_t {
public:
    typedef std::shared_ptr<scene_t> shared_ptr;

    void render();

    void input_scene(const primitive_data::virsual_scene_t &primitive_virsual_scene);

private:
    object_t::shared_ptr input_object(const primitive_data::node_t &primitive_node, int depth);

    skeleton_t::shared_ptr input_skeleton(const primitive_data::node_t &primitive_node);

    joint_t::shared_ptr input_joint(const primitive_data::node_t &primitive_node, int depth);

public:
    //world_t::shared_ptr world;
    //std::list<render_layer_t::shared_ptr> render_layers;
    std::list<object_t::shared_ptr> objects;
    std::list<skeleton_t::shared_ptr> skeletons;
private:
    std::map<size_t, joint_t::shared_ptr> id_joint_map;
};
}

#endif //UNTITLED_GAME_SCENE_H
