//
// Created by chudonghao on 17-12-28.
//

#ifndef UNTITLED_GAME_TRIANGLES_GROUP_H
#define UNTITLED_GAME_TRIANGLES_GROUP_H

#include <QtGui>
#include "material.h"

namespace untitled_game {

class triangles_group_hardware_data_t {
public:
    triangles_group_hardware_data_t();

    ~triangles_group_hardware_data_t();

    GLuint vao_id;
    GLuint element_buffer_id;
    GLuint element_count;
};

class triangles_group_t {
public:
    triangles_group_t() = default;

    //TODO 分开静态mesh物体和动态mesh物体
    material_t::shared_ptr material;
    triangles_group_hardware_data_t hardware_data;
    std::vector<unsigned int> elements;
};

}

#endif //UNTITLED_GAME_TRIANGLES_GROUP_H
