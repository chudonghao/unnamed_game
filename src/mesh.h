//
// Created by chudonghao on 17-12-22.
//

#ifndef UNTITLED_GAME_MESH_H
#define UNTITLED_GAME_MESH_H

#include "scene.h"

namespace untitled_game {

class mesh_t : public object_t {
public:
    typedef std::shared_ptr<mesh_t> shared_ptr;

    mesh_t();

    //TODO 分开静态mesh物体和动态mesh物体
    unsigned int position_buffer_id;
    unsigned int normal_buffer_id;
    unsigned int uvcoord_buffer_id;
    std::vector<triangles_group_t> triangles_groups;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvcoords;
    std::list<std::shared_ptr<modifier_t>> modifiers;
};

}
#endif //UNTITLED_GAME_MESH_H
