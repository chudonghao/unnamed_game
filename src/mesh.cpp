//
// Created by chudonghao on 17-12-22.
//

#include <QtGui/QOpenGLFunctions_3_0>
#include "mesh.h"

namespace untitled_game {

mesh_t::mesh_t() {}

mesh_t::~mesh_t() {

}

void transfer_data_to_hardware(mesh_t::shared_ptr mesh) {
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    auto &position_buffer_id = mesh->hardware_data.position_buffer_id;
    auto &normal_buffer_id = mesh->hardware_data.normal_buffer_id;
    auto &uvcoord_buffer_id = mesh->hardware_data.uvcoord_buffer_id;
    auto &positions = mesh->positions;
    auto &normals = mesh->normals;
    auto &uvcoords = mesh->uvcoords;
    auto &triangles_groups = mesh->triangles_groups;
    f->glGenBuffers(1, &position_buffer_id);
    f->glBindBuffer(GL_ARRAY_BUFFER, position_buffer_id);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);
    f->glGenBuffers(1, &normal_buffer_id);
    f->glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
    f->glGenBuffers(1, &uvcoord_buffer_id);
    f->glBindBuffer(GL_ARRAY_BUFFER, uvcoord_buffer_id);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvcoords.size(), &uvcoords[0], GL_STATIC_DRAW);
    for (auto &triangle_group:triangles_groups) {
        auto &vao_id = triangle_group.hardware_data.vao_id;
        auto &element_buffer_id = triangle_group.hardware_data.element_buffer_id;
        auto &element_count = triangle_group.hardware_data.element_count;
        auto &elements = triangle_group.elements;
        f->glGenBuffers(1, &element_buffer_id);
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), &elements[0], GL_STATIC_DRAW);
        f->glGenVertexArrays(1, &vao_id);
        f->glBindVertexArray(vao_id);//!
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);//!
        f->glBindBuffer(GL_ARRAY_BUFFER, position_buffer_id);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, nullptr);//!
        f->glEnableVertexAttribArray(0);//!
        f->glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
        f->glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, nullptr);//!
        f->glEnableVertexAttribArray(1);//!
        f->glBindBuffer(GL_ARRAY_BUFFER, uvcoord_buffer_id);
        f->glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, nullptr);//!
        f->glEnableVertexAttribArray(2);//!
        f->glBindVertexArray(0);//!
        element_count = static_cast<GLuint>(triangle_group.elements.size());
    }
}

mesh_hardware_data_t::mesh_hardware_data_t() : position_buffer_id(0), uvcoord_buffer_id(0), normal_buffer_id(0) {}

mesh_hardware_data_t::~mesh_hardware_data_t() {
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    if (f->glIsBuffer(position_buffer_id))
        f->glDeleteBuffers(1, &position_buffer_id);
    if (f->glIsBuffer(normal_buffer_id))
        f->glDeleteBuffers(1, &normal_buffer_id);
    if (f->glIsBuffer(uvcoord_buffer_id))
        f->glDeleteBuffers(1, &uvcoord_buffer_id);
}
}
