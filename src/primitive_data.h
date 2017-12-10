//
// Created by chudonghao on 17-12-4.
//

#ifndef UNTITLED_GAME_PRIMITIVE_DATA_H
#define UNTITLED_GAME_PRIMITIVE_DATA_H


#include <cstddef>
#include <vector>
#include <list>
#include <glm/glm.hpp>

namespace untitled_game::primitive_data {
    class camera_t{
        float xfov;
        float aspect_ratio;
        float znear;
        float zfar;
    };
    class material_t{

    };
    class triangles_t{
    public:
        size_t face_count;
        material_t *material;
        std::vector<unsigned int> position_indices;
        std::vector<unsigned int> normal_indices;
        std::vector<unsigned int> uvcoord_indices;
    };
    class mesh_t{
    public:
        mesh_t();
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvcoords;
        std::vector<triangles_t> triangles_groups;
    };
    class geometry_t{
    public:
        geometry_t();
        //TODO 不同类型的geometry
        mesh_t mesh;
    };
    class skin_data_t{
    public:

    };
    class node_t;
    typedef node_t joint_t;
    class skeleton_controller_data_t{
    public:
        skeleton_controller_data_t();
        glm::mat4 bind_shape_matrix;
        std::vector<float> weights;
        std::vector<unsigned int> joint_indices;
        std::vector<unsigned int> weight_indices;
        std::vector<unsigned int> joint_count_per_position;
        std::vector<glm::mat4> inverse_bind_matrices;
    };
    class skin_controller_t{
    public:
        geometry_t*skin;
        std::vector<joint_t*> joints;
        skeleton_controller_data_t *data;
    };

    class controller_t{
    public:
        controller_t();
        //TODO 不同类型的controller
        skin_controller_t skin_controller;
    };
    class light_t{};
    class node_t{
    public:
        enum class type_e{
            node,
            joint
        };

        node_t();

        explicit node_t(type_e type);

        //glm::vec3 location;
        //glm::vec4 rotation_z;
        //glm::vec4 rotation_y;
        //glm::vec4 rotation_x;
        //glm::vec3 scale;
        glm::mat4 transformation_matrix;

        //TODO 不同的instance类型
        type_e type;
        camera_t *camera_instance;
        geometry_t *geometry_instance;
        light_t *light_instance;
        std::vector<controller_t*> controller_instances;

        std::list<node_t> child_nodes;
    };
    class virsual_scene_t {
    public:
        virsual_scene_t();
        std::list<skeleton_controller_data_t> skeleton_controller_datas;
        std::list<geometry_t> geometries;
        std::list<node_t> nodes;
        std::list<controller_t> controllers;
    };
}


#endif //UNTITLED_GAME_PRIMITIVE_DATA_H
