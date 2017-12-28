//
// Created by chudonghao on 17-12-4.
//

#include "config.h"
#include <QOpenGLBuffer>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFunctions_3_0>
#include <QtGui/QOpenGLShaderProgram>
#include <glm/gtx/transform.hpp>
#include <stack>
#include "scene.h"
#include "log.h"
#include "mesh.h"

namespace untitled_game {

using namespace std;

skeleton_t::skeleton_t() : object_t() { type = type_e::armature; }

skeleton_modifier_t::skeleton_modifier_t() : modifier_t(type_e::skeleton_modifier) {}


void render(scene_t::shared_ptr scene) {
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    //TODO shader
    static QOpenGLShaderProgram *opengl_shader_program;
    static int attr_pos;
    static int attr_col;
    static int uniform_matrix;
    static bool inited = false;
    if (!inited) {
        opengl_shader_program = new QOpenGLShaderProgram();
        //static const char *vertex_shader_source =
        //        "attribute highp vec4 attr_pos;\n"
        //                "attribute lowp vec4 attr_col;\n"
        //                "varying lowp vec4 col;\n"
        //                "uniform highp mat4 uniform_matrix;\n"
        //                "void main() {\n"
        //                "   col = attr_col;\n"
        //                "   gl_Position = uniform_matrix * attr_pos;\n"
        //                "}\n";
        //
        //static const char *fragment_shader_source =
        //        "varying lowp vec4 col;\n"
        //                "void main() {\n"
        //                "   gl_FragColor = col;\n"
        //                "}\n";
        static const char *vertex_shader_source =
                "in vec4 attr_pos;\n"
                        "in vec4 attr_col;\n"
                        "out vec4 col;\n"
                        "uniform mat4 uniform_matrix;\n"
                        "void main() {\n"
                        "   col = attr_col;\n"
                        "   gl_Position = uniform_matrix * attr_pos;\n"
                        "}\n";

        static const char *fragment_shader_source =
                "in vec4 col;\n"
                        "void main() {\n"
                        "   gl_FragColor = col;\n"
                        "}\n";
        opengl_shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader_source);
        opengl_shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader_source);
        opengl_shader_program->link();
        LOG_N << opengl_shader_program->isLinked();
        attr_pos = opengl_shader_program->attributeLocation("attr_pos");
        attr_col = opengl_shader_program->attributeLocation("attr_col");
        uniform_matrix = opengl_shader_program->uniformLocation("uniform_matrix");
        inited = true;
    }
    //TODO camera pers
    auto per_look_at_matrix =
            calculate_perspective_matrix(static_pointer_cast<camera_t>(scene->active_camera->data)) *
            calculate_look_at_matrix(scene->active_camera);
    //calculate_perspective_matrix(static_pointer_cast<camera_t>(scene->active_camera->data)) *
    //glm::lookAt(glm::vec3(0.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));

    struct stack_item_t {
        object_t::shared_ptr object;
    };
    std::stack<stack_item_t> stack;
    for (auto &&object:scene->root_objects) {
        if (object)
            stack.push({object});
        else
            LOG_W << "object(nullptr)";
    }
    for (; !stack.empty();) {
        auto &item = stack.top();
        auto object = item.object;
        stack.pop();
        auto parent = object->parent;
        //计算
        if (parent) {
            object->matrix_world = parent->matrix_world * object->matrix_local;
        } else {
            object->matrix_world = object->matrix_local;
        }
        //LOG_N << "object:" << object->id;
        switch (item.object->type) {
            case object_t::type_e::undefined:
                break;
            case object_t::type_e::armature:
                break;
            case object_t::type_e::camera:
                break;
            case object_t::type_e::lamp:
                break;
            case object_t::type_e::mesh: {
                auto mesh = static_pointer_cast<mesh_t>(object->data);
                auto matrix = per_look_at_matrix * object->matrix_world;
                //LOG_N << "mesh:" << mesh->id;
                //LOG_N << "    :" << mesh->triangles_groups.size();
                for (auto &&triangles_group: mesh->triangles_groups) {
                    opengl_shader_program->bind();
                    //f->glUniformMatrix4fv(uniform_matrix, 1, 0, &object->transformation[0][0]);
                    f->glUniformMatrix4fv(uniform_matrix, 1, 0, &matrix[0][0]);
                    f->glBindVertexArray(triangles_group.hardware_data.vao_id);
                    f->glDrawElements(GL_TRIANGLES, triangles_group.hardware_data.element_count, GL_UNSIGNED_INT,
                                      nullptr);
                    f->glBindVertexArray(0);
                    opengl_shader_program->release();
                    break;
                }
                break;
            }
        }
        for (auto &&child_object:item.object->children) {
            if (child_object)
                stack.push({child_object});
            else
                LOG_W << "object(nullptr)";
        }
    }
}
}//namespace untitled_game
