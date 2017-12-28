//
// Created by chudonghao on 17-12-28.
//

#include <QtGui/QOpenGLFunctions_3_0>
#include "triangles_group.h"

namespace untitled_game {


triangles_group_hardware_data_t::triangles_group_hardware_data_t() : vao_id(0), element_buffer_id(0),
                                                                     element_count(0) {}

triangles_group_hardware_data_t::~triangles_group_hardware_data_t() {
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    if (f->glIsBuffer(element_buffer_id))
        f->glDeleteBuffers(1, &element_buffer_id);
}

}
