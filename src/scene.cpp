//
// Created by chudonghao on 17-12-4.
//

#include <QOpenGLBuffer>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFunctions_3_0>
#include <QtGui/QOpenGLShaderProgram>
#include <glm/gtx/transform.hpp>
#include "scene.h"
#include "log.h"

namespace untitled_game {

using namespace std;

joint_t::ptr scene_t::input_joint(const primitive_data::node_t &primitive_node, int depth) {
    BOOST_ASSERT_MSG(primitive_node.type == primitive_data::node_t::type_e::joint, "not correct joint type");
    auto joint = make_shared<joint_t>();
    joint->transformation = primitive_node.transformation_matrix;
    id_joint_map[reinterpret_cast<size_t>(&primitive_node)] = joint;
    for (auto &&primitive_child_node:primitive_node.child_nodes) {
        joint->child_joints.push_back(input_joint(primitive_child_node, depth + 1));
    }
    return joint;
}

skeleton_t::ptr scene_t::input_skeleton(const primitive_data::node_t &primitive_node) {
    if (!primitive_node.child_nodes.empty()) {
        if (primitive_node.child_nodes.begin()->type == primitive_data::node_t::type_e::joint) {
            auto skeleton = make_shared<skeleton_t>();
            skeleton->transformation = primitive_node.transformation_matrix;
            for (auto &&primitive_child_node:primitive_node.child_nodes) {
                skeleton->joints.push_back(input_joint(primitive_child_node, 0));
            }
            return skeleton;
        } else {
            //非joint节点
            return nullptr;
        }
    } else {
        //非有效节点
        return nullptr;
    }
}

object_t::ptr scene_t::input_object(const primitive_data::node_t &primitive_node, int depth) {
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    object_t::ptr result = nullptr;
    if (primitive_node.geometry_instance) {
        auto mesh = make_shared<mesh_t>();
        result = mesh;
        mesh->transformation = primitive_node.transformation_matrix;
        //Test
        /*vector<glm::vec3> positions;
        vector<glm::vec3> normals;
        vector<unsigned int> elements;
        positions.push_back(glm::vec3(0.7f, 0.1f, 0.f));
        positions.push_back(glm::vec3(0.5f, -0.1f, 0.f));
        positions.push_back(glm::vec3(0.5f, 0.1f, 0.f));
        normals.push_back(glm::vec3(0.4f, 0.1f, 0.f));
        normals.push_back(glm::vec3(0.2f, -0.1f, 0.f));
        normals.push_back(glm::vec3(0.2f, 0.1f, 0.f));
        elements.push_back(0);
        elements.push_back(1);
        elements.push_back(2);
        f->glGenBuffers(1, &mesh->position_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->position_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);
        f->glGenBuffers(1, &mesh->normal_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
        mesh->triangles_groups.emplace_back();
        auto &triangles_group = mesh->triangles_groups.back();
        triangles_group.element_count = 3;
        f->glGenVertexArrays(1,&triangles_group.vao_id);
        f->glBindVertexArray(triangles_group.vao_id);
        f->glGenBuffers(1,&triangles_group.element_buffer_id);
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,triangles_group.element_buffer_id);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*elements.size(),&elements[0],GL_STATIC_DRAW);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->position_buffer_id);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        f->glEnableVertexAttribArray(0);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_buffer_id);
        f->glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
        f->glEnableVertexAttribArray(1);
        f->glBindVertexArray(0);*/
        auto &primitive_mesh = primitive_node.geometry_instance->mesh;
        auto &primitive_positions = primitive_mesh.positions;
        auto &primitive_normals = primitive_mesh.normals;
        auto &primitive_uvcoords = primitive_mesh.uvcoords;
        auto &positions = mesh->positions;
        auto &normals = mesh->normals;
        auto &uvcoords = mesh->uvcoords;
        positions.resize(primitive_positions.size());
        normals.resize(primitive_positions.size());
        uvcoords.resize(primitive_positions.size());

        for (auto &triangles_group:primitive_mesh.triangles_groups) {
            mesh->triangles_groups.emplace_back();
            vector<unsigned int> elements = vector<unsigned int>();
            elements.assign(triangles_group.position_indices.begin(), triangles_group.position_indices.end());
            for (int i = 0; i < triangles_group.position_indices.size(); ++i) {
                //TODO 修正
                auto position_index = triangles_group.position_indices[i];
                positions[position_index] = primitive_positions[position_index];

                auto normal_index = triangles_group.normal_indices[i];
                normals[position_index] = primitive_normals[normal_index];

                if (i < triangles_group.uvcoord_indices.size()) {
                    auto uvcoord_index = triangles_group.uvcoord_indices[i];
                    uvcoords[position_index] = primitive_uvcoords[uvcoord_index];
                }
            }
            mesh->triangles_groups.back().element_count = static_cast<GLuint>(elements.size());
            auto &element_buffer_id = mesh->triangles_groups.back().element_buffer_id;
            f->glGenBuffers(1, &element_buffer_id);
            f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
            f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), &elements[0],
                            GL_STATIC_DRAW);
        }
        f->glGenBuffers(1, &mesh->position_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->position_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);
        f->glGenBuffers(1, &mesh->normal_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
        f->glGenBuffers(1, &mesh->uvcoord_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->uvcoord_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvcoords.size(), &uvcoords[0], GL_STATIC_DRAW);
        for (auto &triangle_group:mesh->triangles_groups) {
            f->glGenVertexArrays(1, &triangle_group.vao_id);
            f->glBindVertexArray(triangle_group.vao_id);//!
            f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_group.element_buffer_id);//!
            f->glBindBuffer(GL_ARRAY_BUFFER, mesh->position_buffer_id);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, nullptr);//!
            f->glEnableVertexAttribArray(0);//!
            f->glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_buffer_id);
            f->glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, nullptr);//!
            f->glEnableVertexAttribArray(1);//!
            f->glBindBuffer(GL_ARRAY_BUFFER, mesh->uvcoord_buffer_id);
            f->glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, nullptr);//!
            f->glEnableVertexAttribArray(2);//!
            f->glBindVertexArray(0);//!
        }
    }
    if (primitive_node.light_instance) {
    }
    if (primitive_node.camera_instance) {
    }
    if (!primitive_node.controller_instances.empty()) {
        //TODO 多个controller_instance
        auto mesh = make_shared<mesh_t>();
        result = mesh;
        mesh->transformation = primitive_node.transformation_matrix;
        auto &primitive_mesh = primitive_node.controller_instances[0]->skin_controller.skin->mesh;
        auto &primitive_positions = primitive_mesh.positions;
        auto &primitive_normals = primitive_mesh.normals;
        auto &primitive_uvcoords = primitive_mesh.uvcoords;
        auto &positions = mesh->positions;
        auto &normals = mesh->normals;
        auto &uvcoords = mesh->uvcoords;
        positions.resize(primitive_positions.size());
        normals.resize(primitive_positions.size());
        uvcoords.resize(primitive_positions.size());

        for (auto &triangles_group:primitive_mesh.triangles_groups) {
            mesh->triangles_groups.emplace_back();
            vector<unsigned int> elements = vector<unsigned int>();
            elements.assign(triangles_group.position_indices.begin(), triangles_group.position_indices.end());
            for (int i = 0; i < triangles_group.position_indices.size(); ++i) {
                //TODO 修正
                auto position_index = triangles_group.position_indices[i];
                positions[position_index] = primitive_positions[position_index];

                auto normal_index = triangles_group.normal_indices[i];
                normals[position_index] = primitive_normals[normal_index];

                if (i < triangles_group.uvcoord_indices.size()) {
                    auto uvcoord_index = triangles_group.uvcoord_indices[i];
                    uvcoords[position_index] = primitive_uvcoords[uvcoord_index];
                }
            }
            mesh->triangles_groups.back().element_count = static_cast<GLuint>(elements.size());
            auto &element_buffer_id = mesh->triangles_groups.back().element_buffer_id;
            f->glGenBuffers(1, &element_buffer_id);
            f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
            f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), &elements[0],
                            GL_STATIC_DRAW);
        }
        f->glGenBuffers(1, &mesh->position_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->position_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_DYNAMIC_DRAW);//
        f->glGenBuffers(1, &mesh->normal_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
        f->glGenBuffers(1, &mesh->uvcoord_buffer_id);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh->uvcoord_buffer_id);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvcoords.size(), &uvcoords[0], GL_STATIC_DRAW);
        for (auto &triangle_group:mesh->triangles_groups) {
            f->glGenVertexArrays(1, &triangle_group.vao_id);
            f->glBindVertexArray(triangle_group.vao_id);//!
            f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_group.element_buffer_id);//!
            f->glBindBuffer(GL_ARRAY_BUFFER, mesh->position_buffer_id);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, nullptr);//!
            f->glEnableVertexAttribArray(0);//!
            f->glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_buffer_id);
            f->glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, nullptr);//!
            f->glEnableVertexAttribArray(1);//!
            f->glBindBuffer(GL_ARRAY_BUFFER, mesh->uvcoord_buffer_id);
            f->glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, nullptr);//!
            f->glEnableVertexAttribArray(2);//!
            f->glBindVertexArray(0);//!
        }
        auto skeleton_modifier = make_shared<skeleton_modifier_t>();
        mesh->modifiers.push_back(skeleton_modifier);
        auto &skin_controller = primitive_node.controller_instances[0]->skin_controller;
        skeleton_modifier->weights = skin_controller.data->weights;
        skeleton_modifier->inverse_bind_matrices = skin_controller.data->inverse_bind_matrices;
        skeleton_modifier->weight_indices = skin_controller.data->weight_indices;
        skeleton_modifier->joint_count_per_position = skin_controller.data->joint_count_per_position;
        skeleton_modifier->joint_indices = skin_controller.data->joint_indices;
        skeleton_modifier->positions_with_BSM.resize(mesh->positions.size());
        for (int i = 0; i < skeleton_modifier->positions_with_BSM.size(); ++i) {
            skeleton_modifier->positions_with_BSM[i] =
                    skin_controller.data->bind_shape_matrix * glm::vec4(mesh->positions[i], 1.0f);
            //skeleton_modifier->positions_with_BSM[i] =
            //        glm::vec4(mesh->positions[i], 1.0f) * skin_controller.data->bind_shape_matrix;
        }
        for (auto joint:skin_controller.joints) {
            skeleton_modifier->joints.push_back(id_joint_map[reinterpret_cast<size_t>(joint)]);
        }
    }
    if (!result)
        result = make_shared<object_t>(object_t::type_e::undefined);
    for (auto &&child_primitive_node:primitive_node.child_nodes) {
        result->child_objects.push_back(input_object(child_primitive_node, depth + 1));
        //LOG_W << "NOT process primitive child node,because the parent is NOT available.";
    }
    return result;
}

void scene_t::input_scene(const primitive_data::virsual_scene_t &primitive_virsual_scene) {
    for (auto &&primitive_node : primitive_virsual_scene.nodes) {
        //TODO 处理不是根节点的骨架
        auto skeleton = input_skeleton(primitive_node);
        if (skeleton) {
            skeletons.push_back(skeleton);
        }
    }
    for (auto &&primitive_node : primitive_virsual_scene.nodes) {
        objects.push_back(input_object(primitive_node, 0));
    }
}

static QOpenGLShaderProgram *opengl_shader_program;
static int attr_pos;
static int attr_col;
static int uniform_matrix;

static void render_object(object_t::ptr object, glm::mat4 PLM, int depth) {
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    if (object) {
        PLM = PLM * object->transformation;
        switch (object->type) {
            case object_t::type_e::undefined:
                break;
            case object_t::type_e::mesh: {
                auto mesh = static_pointer_cast<mesh_t>(object);
                for (auto &&triangles_group: mesh->triangles_groups) {
                    opengl_shader_program->bind();
                    //f->glUniformMatrix4fv(uniform_matrix, 1, 0, &object->transformation[0][0]);
                    f->glUniformMatrix4fv(uniform_matrix, 1, 0, &PLM[0][0]);
                    f->glBindVertexArray(triangles_group.vao_id);
                    f->glDrawElements(GL_TRIANGLES, triangles_group.element_count, GL_UNSIGNED_INT, nullptr);
                    f->glBindVertexArray(0);
                    opengl_shader_program->release();
                    break;
                }
            }
            case object_t::type_e::light:
                break;
            case object_t::type_e::joint:
                break;
            case object_t::type_e::skeleton:
                break;
        }
        for (auto &&child_object:object->child_objects) {
            render_object(child_object, PLM, depth + 1);
        }
    } else {}
}

static void apply_modifiers(object_t::ptr object, int depth) {
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    if (object) {
        switch (object->type) {
            case object_t::type_e::undefined:
                break;
            case object_t::type_e::mesh: {
                auto mesh = static_pointer_cast<mesh_t>(object);
                if (!mesh->modifiers.empty()) {
                    for (auto &modifier:mesh->modifiers) {
                        switch (modifier->type) {
                            case modifier_t::type_e::skeleton_modifier: {
                                auto skeleton_modifier = static_pointer_cast<skeleton_modifier_t>(modifier);
                                //outv = sum<i=0,n>(((v*BSM)*IBMi*JMi)*JW)
                                //n:    number of joints that influence vertex v
                                //BSM:  bind shape matrix
                                //IBMi: inverse bind matrix of joint i
                                //JMi:  joint matrix of joint i
                                //JW:   joint weight/influence of joint i on vertex v
                                //(v * BSM) is calculated and stored at load time.
                                int sum1 = 0;
                                for (int i = 0; i < skeleton_modifier->positions_with_BSM.size(); ++i) {
                                    glm::vec4 outv = glm::vec4(0.f);
                                    auto joint_count_per_position = skeleton_modifier->joint_count_per_position[i];
                                    for (int j = 0; j < joint_count_per_position; ++j) {
                                        int joint_index = skeleton_modifier->joint_indices[sum1 + j];
                                        int weight_index = skeleton_modifier->weight_indices[sum1 + j];
                                        float weight = skeleton_modifier->weights[weight_index];
                                        glm::vec4 outvj =
                                                skeleton_modifier->joints[joint_index]->world_transformation *
                                                //glm::inverse(skeleton_modifier->inverse_bind_matrices[joint_index]) *
                                                skeleton_modifier->inverse_bind_matrices[joint_index] *
                                                glm::vec4(skeleton_modifier->positions_with_BSM[i], 1.0f) *
                                                weight;
                                        outv += outvj;
                                    }
                                    mesh->positions[i] = glm::vec3(outv);
                                    sum1 += joint_count_per_position;
                                }
                                f->glBindBuffer(GL_ARRAY_BUFFER, mesh->position_buffer_id);
                                f->glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->positions.size() * sizeof(glm::vec3),
                                                   &mesh->positions[0]);
                                f->glBindBuffer(GL_ARRAY_BUFFER, 0);
                                break;
                            }
                        }
                    }
                }
            }
            case object_t::type_e::light:
                break;
            case object_t::type_e::joint:
                break;
            case object_t::type_e::skeleton:
                break;
        }
        for (auto &&child_object:object->child_objects) {
            apply_modifiers(child_object, depth + 1);
        }
    } else {}
}

static void calculate_joints_world_matrix(joint_t::ptr joint, glm::mat4 M, int depth) {
    //TODO change joint
    //if (depth == 1) {
    //    auto mat = glm::mat4(0.4717617, 0.8817261, 1.18468e-7, 0, -0.8817261, 0.4717616, -7.09735e-8, 1, -1.18468e-7,
    //                         -7.09735e-8, 1, 0, 0, 0, 0, 1);
    //    joint->world_transformation = joint->transformation * M;
    //    //joint->world_transformation = joint->transformation * M;
    //} else if (depth == 0) {
    //    auto mat = glm::mat4(1,0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1);
    //    joint->world_transformation = joint->transformation * M;
    //}
    joint->world_transformation = M * joint->transformation;
    //joint->world_transformation = joint->transformation * M;
    //glm::vec4 tmp = joint->world_transformation * glm::vec4(0.f, 0.f, 0.f, 1.f);
    //glm::vec3 tmp2 = glm::mat3(joint->world_transformation) * glm::vec3(1.f);
    //LOG_N << depth << " " << tmp.x << "\t" << tmp.y << "\t" << tmp.z << "\t" << tmp.w;
    //LOG_N << depth << " " << tmp2.x << "\t" << tmp2.y << "\t" << tmp2.z;
    for (auto &&child_joint:joint->child_joints) {
        calculate_joints_world_matrix(child_joint, joint->world_transformation, depth + 1);
    }
};

void scene_t::render() {
    static bool init = false;
    if (!init) {
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
        init = true;
    }
    static float tmp = static_cast<float>(M_PI_2 * 3);
    if (tmp > 2 * M_PI) {
        tmp = 0.f;
    } else {
        tmp += 0.01f;
    }
    glm::mat4 P = glm::perspective((float) M_PI / 3, 1.f, 0.1f, 100.f);
    glm::mat4 L = glm::lookAt(glm::vec3(4.f * cos(tmp), 4.f * sin(tmp), 2.f),
                              glm::vec3(0.f, 0.f, 2.f),
                              glm::vec3(0.f, 0.f, 1.f));
    for (auto &&skeleton :skeletons) {
        for (auto &&joint:skeleton->joints) {
            calculate_joints_world_matrix(joint, skeleton->transformation, 0);
        }
    }
    for (auto &&child_object : objects) {
        apply_modifiers(child_object, 0);
    }
    for (auto &&child_object : objects) {
        render_object(child_object, P * L, 0);
    }
}

object_t::object_t(object_t::type_e type) : type(type) {}

mesh_t::mesh_t() : object_t(type_e::mesh) {}

joint_t::joint_t() : object_t(type_e::joint) {}

light_t::light_t() : object_t(type_e::light) {}

skeleton_t::skeleton_t() : object_t(type_e::skeleton) {}

skeleton_modifier_t::skeleton_modifier_t() : modifier_t(type_e::skeleton_modifier) {}
}//namespace untitled_game
