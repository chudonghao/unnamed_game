//
// Created by chudonghao on 17-12-23.
//
#include "config.h"
#include <stack>
#include <iomanip>
#include "my_collada_writer1.h"
#include "mesh.h"
#include "log.h"

using namespace unnamed_game;
using namespace std;

namespace unnamed_game {

//COLLADA存储矩阵的方式是行优先，而gl是列优先
static glm::mat4 to_glm_mat4(const COLLADABU::Math::Matrix4 &m) {
    glm::mat4 result;
    result[0][0] = m[0][0];
    result[0][1] = m[0][1];
    result[0][2] = m[0][2];
    result[0][3] = m[0][3];
    result[1][0] = m[1][0];
    result[1][1] = m[1][1];
    result[1][2] = m[1][2];
    result[1][3] = m[1][3];
    result[2][0] = m[2][0];
    result[2][1] = m[2][1];
    result[2][2] = m[2][2];
    result[2][3] = m[2][3];
    result[3][0] = m[3][0];
    result[3][1] = m[3][1];
    result[3][2] = m[3][2];
    result[3][3] = m[3][3];
    result = glm::transpose(result);
    return result;
}

my_collada_writer1_t::my_collada_writer1_t(unnamed_game::context_t::shared_ptr context) : context(context) {

}

void my_collada_writer1_t::cancel(const COLLADAFW::String &errorMessage) {
    LOG_N << "cancle:";
}

void my_collada_writer1_t::start() {
    LOG_N << "start:";
}

void my_collada_writer1_t::finish() {
    LOG_N << "finish:";
}

bool my_collada_writer1_t::writeGlobalAsset(const COLLADAFW::FileInfo *asset) {
    LOG_N << "global asset:";
    return true;
}

bool my_collada_writer1_t::writeScene(const COLLADAFW::Scene *scene) {
    LOG_N << "scene:";
    return true;
}

bool my_collada_writer1_t::writeVisualScene(const COLLADAFW::VisualScene *visualScene) {
    LOG_N << "visual scene:" << visualScene->getName();
    auto &scene = context->data->scenes.create(visualScene->getName());
    //使用栈　进行树形数据结构的迭代
    struct stack_item_t {
        std::string object_parent_name;
        object_t::shared_ptr object_parent;
        bone_t::shared_ptr bone_parent;
        armature_t::shared_ptr armature;
        COLLADAFW::Node *node;
        int depth;

        stack_item_t(const std::string &object_parent_name, object_t::shared_ptr object_parent, COLLADAFW::Node *node,
                     int depth) : object_parent_name(object_parent_name), object_parent(std::move(object_parent)),
                                  node(node), depth(depth) {}

        stack_item_t(bone_t::shared_ptr bone_parent, armature_t::shared_ptr armature, COLLADAFW::Node *node, int depth)
                : bone_parent(std::move(bone_parent)), armature(std::move(armature)), node(node), depth(depth) {}
    };
    std::stack<stack_item_t> stack;
    //根节点入栈
    for (int i = (int) visualScene->getRootNodes().getCount() - 1; i >= 0; --i) {
        stack.push(stack_item_t("", nullptr, visualScene->getRootNodes()[i], 0));
    }
    //迭代树形数据结构
    for (; !stack.empty();) {
        //栈顶出栈
        auto item = stack.top();
        stack.pop();
        //debug
        auto ss = stringstream();
        for (int j = 0; j < item.depth; ++j) {
            if (j == item.depth - 1)
                ss << "├";
            else
                ss << "│";
        }
        LOG_N << ss.str() << item.node->getOriginalId();
        //处理不同类型的节点
        switch (item.node->getType()) {
            //一般性节点
            case COLLADAFW::Node::NODE: {
                //新建object
                auto object = context->data->objects.create(item.node->getOriginalId());
                //设置父子关系
                set_parent(object, item.object_parent);
                //设置坐标变换矩阵
                object->matrix_local = to_glm_mat4(item.node->getTransformationMatrix());
                //获取mesh
                if (!item.node->getInstanceGeometries().empty()) {
                    LOG_N << "instance mesh id:"
                          << unique_id_id_map[item.node->getInstanceGeometries()[0]->getInstanciatedObjectId()];
                    object->data = context->data->meshes[unique_id_id_map[item.node->getInstanceGeometries()[0]->getInstanciatedObjectId()]];
                    object->type = object_t::type_e::mesh;
                }
                //TODO 获取??
                if (!item.node->getInstanceControllers().empty()) {
                    LOG_N << "instance controller id:"
                          << item.node->getInstanceControllers()[0]->getInstanciatedObjectId().toAscii();
                }
                if (!item.node->getInstanceCameras().empty()) {
                    LOG_N << "instance camera count:" << item.node->getInstanceCameras().getCount();
                    LOG_N << "instance camera id:"
                          << item.node->getInstanceCameras()[0]->getInstanciatedObjectId().toAscii();
                    LOG_N << "instance camera id:"
                          << unique_id_id_map[item.node->getInstanceCameras()[0]->getInstanciatedObjectId()];
                    object->data = context->data->cameras[unique_id_id_map[item.node->getInstanceCameras()[0]->getInstanciatedObjectId()]];
                    object->type = object_t::type_e::camera;
                }
                //使用栈　处理树形数据结构
                for (int i = (int) item.node->getChildNodes().getCount() - 1; i >= 0; --i) {
                    //子节点入栈
                    stack.push(stack_item_t(item.node->getOriginalId(), object, item.node->getChildNodes()[i],
                                            item.depth + 1));
                }
                if (item.depth == 0) {
                    scene->root_objects.insert(object);
                }
                break;
            }
                //骨骼节点
            case COLLADAFW::Node::JOINT: {
                //骨骼树所属的armature
                armature_t::shared_ptr armature;
                //armature节点
                //ture 说明为根JOINT节点
                if (item.object_parent) {
                    //NODE->JOINT 父NODE节点没有其他类型
                    if (item.object_parent->type == object_t::type_e::undefined) {
                        //NODE(type=undefined)->JOINT 时新建armature
                        armature = context->data->armatures.create(item.object_parent_name);
                        //设置object(type=armature)
                        item.object_parent->type = object_t::type_e::armature;
                        item.object_parent->data = armature;
                    } else if (item.object_parent->type == object_t::type_e::armature) {
                        //NODE(type=armature)->JOINT 时读取armature
                        armature = static_pointer_cast<armature_t>(item.object_parent->data);
                    } else {
                        LOG_W << "joint node has wrong object parent(type=" << (int) item.object_parent->type << ")";
                    }
                } else {
                    armature = item.armature;
                }
                //bone节点
                auto bone = armature->bones.create(item.node->getOriginalId());
                //设置坐标系变换矩阵
                bone->matrix_local = to_glm_mat4(item.node->getTransformationMatrix());
                //设置父子关系
                set_parent(bone, item.bone_parent);
                //使用栈　处理树形数据结构
                for (int i = (int) item.node->getChildNodes().getCount() - 1; i >= 0; --i) {
                    //子节点入栈
                    stack.push(stack_item_t(bone, armature, item.node->getChildNodes()[i], item.depth + 1));
                }
                break;
            }
        }
    }
    return true;
}

bool my_collada_writer1_t::writeLibraryNodes(const COLLADAFW::LibraryNodes *libraryNodes) {
    LOG_N << "library nodes:";
    return true;
}

bool my_collada_writer1_t::writeGeometry(const COLLADAFW::Geometry *geometry) {
    LOG_N << "geometry:" << geometry->getOriginalId();
    switch (geometry->getType()) {
        case COLLADAFW::Geometry::GEO_TYPE_MESH: {
            auto ge = const_cast<COLLADAFW::Geometry *>(geometry);
            auto primitive_mesh = static_cast<COLLADAFW::Mesh *>(ge);
            unique_id_id_map[primitive_mesh->getUniqueId()] = primitive_mesh->getOriginalId();

            auto position_values = primitive_mesh->getPositions().getFloatValues();
            auto normal_values = primitive_mesh->getNormals().getFloatValues();
            auto uvcoord_values = primitive_mesh->getUVCoords().getFloatValues();
            auto &mesh = context->data->meshes.create(primitive_mesh->getOriginalId());
            mesh->positions.resize(position_values->getCount());
            mesh->normals.resize(position_values->getCount());

            if (uvcoord_values)
                mesh->uvcoords.resize(position_values->getCount());

            for (int i = 0; i < primitive_mesh->getMeshPrimitives().getCount(); ++i) {
                mesh->triangles_groups.emplace_back();
                auto &triangles = mesh->triangles_groups.back();
                auto &position_indices = primitive_mesh->getMeshPrimitives()[i]->getPositionIndices();
                auto &normal_indices = primitive_mesh->getMeshPrimitives()[i]->getNormalIndices();
                //TODO 多个uvcoord indices array
                unsigned int *uvcoord_indices;
                if (!primitive_mesh->getMeshPrimitives()[i]->getUVCoordIndicesArray().empty()) {
                    uvcoord_indices = primitive_mesh->getMeshPrimitives()[i]->getUVCoordIndicesArray()[0]->getIndices().getData();
                }
                triangles.elements.assign(position_indices.getData(),
                                          position_indices.getData() + position_indices.getCount());
                for (int j = 0; j < triangles.elements.size(); ++j) {
                    //TODO 修正
                    auto position_index = position_indices[j];
                    mesh->positions[position_index] = *(glm::vec3 *) (&position_values->getData()[position_index *
                                                                                                  3/*3 float*/]);

                    auto normal_index = normal_indices[j];
                    mesh->normals[position_index] = *(glm::vec3 *) (&normal_values->getData()[normal_index *
                                                                                              3/*3 float*/]);

                    if (uvcoord_values && !uvcoord_values->empty()) {
                        auto uvcoord_index = uvcoord_indices[j];
                        mesh->uvcoords[position_index] = *(glm::vec2 *) (&uvcoord_values->getData()[uvcoord_index *
                                                                                                    2/*2 float*/]);
                    }
                }
            }
            transfer_data_to_hardware(mesh);
            break;
        }
        case COLLADAFW::Geometry::GEO_TYPE_SPLINE:
            break;
        case COLLADAFW::Geometry::GEO_TYPE_CONVEX_MESH:
            break;
        case COLLADAFW::Geometry::GEO_TYPE_UNKNOWN:
            break;
    }
    return true;
}

bool my_collada_writer1_t::writeMaterial(const COLLADAFW::Material *material) {
    return true;
}

bool my_collada_writer1_t::writeEffect(const COLLADAFW::Effect *effect) {
    return true;
}

bool my_collada_writer1_t::writeCamera(const COLLADAFW::Camera *primitive_camera) {
    LOG_N << "camera:" << primitive_camera->getOriginalId();
    LOG_N << "camera:" << primitive_camera->getUniqueId().toAscii();
    auto &camera = context->data->cameras.create(primitive_camera->getOriginalId());
    unique_id_id_map[primitive_camera->getUniqueId()] = primitive_camera->getOriginalId();
    switch (primitive_camera->getCameraType()) {
        case COLLADAFW::Camera::UNDEFINED_CAMERATYPE:
            break;
        case COLLADAFW::Camera::ORTHOGRAPHIC:
            //TODO
            break;
        case COLLADAFW::Camera::PERSPECTIVE:
            camera->type = camera_t::type_e::perspective;
            //in rad
            camera->x_fov = static_cast<float>(primitive_camera->getXFov().getValue() / 180 * M_PI);
            camera->aspect_ratio = static_cast<float>(primitive_camera->getAspectRatio().getValue());
            camera->near_clipping_plane = static_cast<float>(primitive_camera->getNearClippingPlane().getValue());
            camera->far_clipping_plane = static_cast<float>(primitive_camera->getFarClippingPlane().getValue());
            break;
    }
    return true;
}

bool my_collada_writer1_t::writeImage(const COLLADAFW::Image *image) {
    return true;
}

bool my_collada_writer1_t::writeLight(const COLLADAFW::Light *light) {
    return true;
}

bool my_collada_writer1_t::writeAnimation(const COLLADAFW::Animation *animation) {
    return true;
}

bool my_collada_writer1_t::writeAnimationList(const COLLADAFW::AnimationList *animationList) {
    return true;
}

bool my_collada_writer1_t::writeSkinControllerData(const COLLADAFW::SkinControllerData *skinControllerData) {
    LOG_N << "skin controller data:" << skinControllerData->getOriginalId() << " "
          << skinControllerData->getUniqueId().toAscii();
    return true;
}

bool my_collada_writer1_t::writeController(const COLLADAFW::Controller *controller) {
    LOG_N << "controller:" << controller->getUniqueId().toAscii();
    switch (controller->getControllerType()) {
        case COLLADAFW::Controller::CONTROLLER_TYPE_SKIN: {
            auto skin_controller = static_cast<const COLLADAFW::SkinController *>(controller);
            break;
        }
        case COLLADAFW::Controller::CONTROLLER_TYPE_MORPH:
            break;
    }
    return true;
}

bool my_collada_writer1_t::writeFormulas(const COLLADAFW::Formulas *formulas) {
    return true;
}

bool my_collada_writer1_t::writeKinematicsScene(const COLLADAFW::KinematicsScene *kinematicsScene) {
    return true;
}

}
