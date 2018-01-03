//
// Created by chudonghao on 17-12-22.
//

#ifndef UNNAMED_GAME_MESH_H
#define UNNAMED_GAME_MESH_H

#include <vector>
#include <glm/glm.hpp>
#include "has_id.h"
#include "triangles_group.h"
#include "scene.h"

namespace unnamed_game {

class mesh_hardware_data_t {
public:
    mesh_hardware_data_t();

    ~mesh_hardware_data_t();

    GLuint position_buffer_id;
    GLuint normal_buffer_id;
    GLuint uvcoord_buffer_id;
};

class mesh_t : public has_id_t {
public:
    typedef std::shared_ptr<mesh_t> shared_ptr;

    mesh_t();

    ~mesh_t() override;

    //TODO 分开静态mesh物体和动态mesh物体
    //TODO 使用渲染引擎借口
    mesh_hardware_data_t hardware_data;
    std::vector<triangles_group_t> triangles_groups;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvcoords;
    std::list<std::shared_ptr<modifier_t>> modifiers;
};

void transfer_data_to_hardware(mesh_t::shared_ptr mesh);

}
#endif //UNNAMED_GAME_MESH_H
