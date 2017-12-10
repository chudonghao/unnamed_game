//
// Created by chudonghao on 17-12-4.
//

#include <boost/format.hpp>
#include "my_collada_writer.h"
#include "log.h"

namespace untitled_game {

static void test(const COLLADABU::Math::Matrix4 &m) {
    LOG_N << m[0][0] << " "
          << m[0][1] << " "
          << m[0][2] << " "
          << m[0][3] << " "
          << m[1][0] << " "
          << m[1][1] << " "
          << m[1][2] << " "
          << m[1][3] << " "
          << m[2][0] << " "
          << m[2][1] << " "
          << m[2][2] << " "
          << m[2][3] << " "
          << m[3][0] << " "
          << m[3][1] << " "
          << m[3][2] << " "
          << m[3][3];
}

/**
 * 函数存在的目的是确定转置场合
 * @param m
 * @return
 */
static glm::mat4 to_mat4(const COLLADABU::Math::Matrix4 &m) {
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
    return result;
}

void my_collada_writer_t::cancel(const COLLADAFW::String &errorMessage) {
    LOG_N << "cancel";
}

void my_collada_writer_t::start() {
    LOG_N << "start";
}

void my_collada_writer_t::finish() {
    LOG_N << "finish";
}

bool my_collada_writer_t::writeGlobalAsset(const COLLADAFW::FileInfo *asset) {
    LOG_N << "global asset:";
    return true;
}

bool my_collada_writer_t::writeScene(const COLLADAFW::Scene *scene) {
    LOG_N << "scene:";
    return true;
}

void my_collada_writer_t::process_node(primitive_data::node_t &primitive_node, COLLADAFW::Node *node, int deepth) {
    std::stringstream sb;
    for (int j = 0; j < deepth; ++j) {
        sb << "\t";
    }
    primitive_node.transformation_matrix = glm::transpose(to_mat4(node->getTransformationMatrix()));
    if (node->getType() == COLLADAFW::Node::NodeType::JOINT) {
        primitive_node.type = primitive_data::node_t::type_e::joint;
        LOG_N << "joint transform matrix:";
        test(node->getTransformationMatrix());
        LOG_N << sb.str() << "jonit:" << node->getName() << ":" << node->getUniqueId().toAscii();
        id_joint_map[node->getUniqueId()] = &primitive_node;
    } else {
        LOG_N << "node transform matrix:";
        test(node->getTransformationMatrix());

        LOG_N << sb.str() << node->getName();
        sb << "\t";
        //TODO node有多个instance
        if (!node->getInstanceCameras().empty()) {
            LOG_N << sb.str() << "instance camera:"
                  << node->getInstanceCameras()[0]->getInstanciatedObjectId().toAscii();
        }
        if (!node->getInstanceGeometries().empty()) {
            LOG_N << sb.str() << "instance geometry:" << node->getInstanceGeometries()[0]->getName();
            LOG_N << sb.str() << "instance geometry:"
                  << node->getInstanceGeometries()[0]->getMaterialBindings().getCount();
            LOG_N << sb.str() << "instance geometry:"
                  << node->getInstanceGeometries()[0]->getInstanciatedObjectId().toAscii();
            primitive_node.geometry_instance = id_geometry_map[node->getInstanceGeometries()[0]->getInstanciatedObjectId()];
        }
        if (!node->getInstanceControllers().empty()) {
            LOG_N << sb.str() << "instance controller:" << node->getInstanceControllers()[0]->getName();
            LOG_N << sb.str() << "instance controller:"
                  << node->getInstanceControllers()[0]->getMaterialBindings().getCount();
            LOG_N << sb.str() << "instance controller:"
                  << node->getInstanceControllers()[0]->getInstanciatedObjectId().toAscii();
            for (int i = 0; i < node->getInstanceControllers().getCount(); ++i) {
                primitive_node.controller_instances.push_back(id_controller_map[node->getInstanceControllers()[0]->getInstanciatedObjectId()]);
            }
        }
        if (!node->getInstanceLights().empty()) {
            LOG_N << sb.str() << "instance light:"
                  << node->getInstanceLights()[0]->getInstanciatedObjectId().toAscii();
        }
    }
    deepth += 1;
    for (int i = 0; i < node->getChildNodes().getCount(); ++i) {
        primitive_node.child_nodes.emplace_back();
        process_node(primitive_node.child_nodes.back(), node->getChildNodes()[i], deepth);
    }
}

bool my_collada_writer_t::writeVisualScene(const COLLADAFW::VisualScene *visualScene) {
    LOG_N << "visual scene:" << visualScene->getName();
    for (int i = 0; i < visualScene->getRootNodes().getCount(); ++i) {
        primitive_visual_scene.nodes.emplace_back();
        process_node(primitive_visual_scene.nodes.back(), visualScene->getRootNodes()[i], 1);
    }
    for (auto &&pair : controller_joint_ids_map) {
        for (auto &&id : pair.second) {
            pair.first->skin_controller.joints.push_back(id_joint_map[id]);
        }
    }
    //LOG_N << "joints count:";
    //for (auto &&controller : primitive_visual_scene.controllers) {
    //    LOG_N << controller.skin_controller.joints.size() << "@"<< controller.skin_controller.joints[0];
    //}
    return true;
}

bool my_collada_writer_t::writeLibraryNodes(const COLLADAFW::LibraryNodes *libraryNodes) {
    LOG_N << "library nodes:";
    return true;
}

bool my_collada_writer_t::writeGeometry(const COLLADAFW::Geometry *geometry) {
    primitive_visual_scene.geometries.emplace_back();
    auto &primitive_geometry = primitive_visual_scene.geometries.back();
    LOG_N << "geometry:" << geometry->getName() << " " << geometry->getUniqueId().toAscii();
    switch (geometry->getType()) {
        case COLLADAFW::Geometry::GEO_TYPE_MESH: {
            auto ge = const_cast<COLLADAFW::Geometry *>(geometry);
            auto mesh = static_cast<COLLADAFW::Mesh *>(ge);
            auto position_values = mesh->getPositions().getFloatValues();
            auto normal_values = mesh->getNormals().getFloatValues();
            auto uvcoord_values = mesh->getUVCoords().getFloatValues();
            primitive_geometry.mesh.positions.assign((glm::vec3 *) position_values->getData(),
                                                     (glm::vec3 *) (position_values->getData() +
                                                                    position_values->getCount()));
            primitive_geometry.mesh.normals.assign((glm::vec3 *) normal_values->getData(),
                                                   (glm::vec3 *) (normal_values->getData() +
                                                                  normal_values->getCount()));
            if (uvcoord_values)
                primitive_geometry.mesh.uvcoords.assign((glm::vec3 *) uvcoord_values->getData(),
                                                        (glm::vec3 *) (uvcoord_values->getData() +
                                                                       uvcoord_values->getCount()));
            for (int i = 0; i < mesh->getMeshPrimitives().getCount(); ++i) {
                primitive_geometry.mesh.triangles_groups.emplace_back();
                auto &triangles = primitive_geometry.mesh.triangles_groups.back();
                triangles.face_count = mesh->getMeshPrimitives()[i]->getFaceCount();
                auto &position_indices = mesh->getMeshPrimitives()[i]->getPositionIndices();
                auto &normal_indices = mesh->getMeshPrimitives()[i]->getNormalIndices();
                triangles.position_indices.assign(position_indices.getData(),
                                                  position_indices.getData() + position_indices.getCount());
                triangles.normal_indices.assign(normal_indices.getData(),
                                                normal_indices.getData() + normal_indices.getCount());
                if (mesh->getMeshPrimitives()[i]->getUVCoordIndicesArray().getData()) {
                    auto &uvcoord_indices = mesh->getMeshPrimitives()[i]->getUVCoordIndicesArray().getData()[0]->getIndices();
                    triangles.uvcoord_indices.assign(uvcoord_indices.getData(),
                                                     uvcoord_indices.getData() + uvcoord_indices.getCount());
                }
                LOG_N << "position indices count:" << position_indices.getCount();
                LOG_N << "normal indices count:" << normal_indices.getCount();
            }
            break;
        }
        case COLLADAFW::Geometry::GEO_TYPE_SPLINE:
            break;
        case COLLADAFW::Geometry::GEO_TYPE_CONVEX_MESH:
            break;
        case COLLADAFW::Geometry::GEO_TYPE_UNKNOWN:
            break;
    }
    id_geometry_map[geometry->getUniqueId()] = &primitive_geometry;
    return true;
}

bool my_collada_writer_t::writeMaterial(const COLLADAFW::Material *material) {
    LOG_N << "material:" << material->getName();
    return true;
}

bool my_collada_writer_t::writeEffect(const COLLADAFW::Effect *effect) {
    LOG_N << "effect:" << effect->getName();
    return true;
}

bool my_collada_writer_t::writeCamera(const COLLADAFW::Camera *camera) {
    LOG_N << "camera:" << camera->getName() << " " << camera->getUniqueId().toAscii();
    return true;
}

bool my_collada_writer_t::writeImage(const COLLADAFW::Image *image) {
    LOG_N << "image:" << image->getName();
    LOG_N << boost::format("\t%1%") % image->getImageURI().getPath();
    return true;
}

bool my_collada_writer_t::writeLight(const COLLADAFW::Light *light) {
    LOG_N << "light:" << light->getName() << " " << light->getUniqueId().toAscii();
    return true;
}

bool my_collada_writer_t::writeAnimation(const COLLADAFW::Animation *animation) {
    LOG_N << "animation:" << animation->getName();
    return true;
}

bool my_collada_writer_t::writeAnimationList(const COLLADAFW::AnimationList *animationList) {
    LOG_N << "animation list:" << animationList->getUniqueId().toAscii();
    animationList->getAnimationBindings();
    return true;
}

bool my_collada_writer_t::writeSkinControllerData(const COLLADAFW::SkinControllerData *skinControllerData) {
    LOG_N << "skin controller data:" << skinControllerData->getUniqueId().toAscii();
    primitive_visual_scene.skeleton_controller_datas.emplace_back();
    auto &primitive_skeleton_controller_data = primitive_visual_scene.skeleton_controller_datas.back();
    auto weights_array = skinControllerData->getWeights().getFloatValues();
    auto &joint_indices = skinControllerData->getJointIndices();
    auto &weight_indices = skinControllerData->getWeightIndices();
    auto &joint_per_vertex = skinControllerData->getJointsPerVertex();
    auto &inverse_bind_matrices = skinControllerData->getInverseBindMatrices();
    primitive_skeleton_controller_data.bind_shape_matrix = glm::transpose(
            to_mat4(skinControllerData->getBindShapeMatrix()));
    LOG_N << "skin controller data bind shape matrix:";
    test(skinControllerData->getBindShapeMatrix());

    primitive_skeleton_controller_data.weights.assign(weights_array->getData(),
                                                      weights_array->getData() + weights_array->getCount());
    primitive_skeleton_controller_data.joint_indices.assign(joint_indices.getData(),
                                                            joint_indices.getData() + joint_indices.getCount());
    primitive_skeleton_controller_data.weight_indices.assign(weight_indices.getData(),
                                                             weight_indices.getData() + weight_indices.getCount());
    primitive_skeleton_controller_data.joint_count_per_position.assign(joint_per_vertex.getData(),
                                                                       joint_per_vertex.getData() +
                                                                       joint_per_vertex.getCount());
    primitive_skeleton_controller_data.inverse_bind_matrices.resize(inverse_bind_matrices.getCount());
    for (int i = 0; i < inverse_bind_matrices.getCount(); ++i) {
        primitive_skeleton_controller_data.inverse_bind_matrices[i] = glm::transpose(to_mat4(inverse_bind_matrices[i]));
        LOG_N << "skin controller data inverse bind matrix:";
        test(inverse_bind_matrices[i]);
    }

    id_skeleton_controller_data_map[skinControllerData->getUniqueId()] = &primitive_skeleton_controller_data;
    return true;
}

bool my_collada_writer_t::writeController(const COLLADAFW::Controller *controller) {
    LOG_N << "controller:";
    switch (controller->getControllerType()) {
        case COLLADAFW::Controller::CONTROLLER_TYPE_SKIN: {
            auto skin_controller = static_cast<const COLLADAFW::SkinController *>(controller);

            primitive_visual_scene.controllers.emplace_back();
            auto &primitive_controller = primitive_visual_scene.controllers.back();
            primitive_controller.skin_controller.skin = id_geometry_map[skin_controller->getSource()];
            primitive_controller.skin_controller.data = id_skeleton_controller_data_map[skin_controller->getSkinControllerData()];
            for (int i = 0; i < skin_controller->getJoints().getCount(); ++i) {
                LOG_N << "\tjoints:" << skin_controller->getJoints()[i].toAscii();
            }
            id_controller_map[skin_controller->getUniqueId()] = &primitive_controller;
            auto &joint_ids = controller_joint_ids_map[&primitive_controller];
            joint_ids.assign(skin_controller->getJoints().getData(),
                             skin_controller->getJoints().getData() + skin_controller->getJoints().getCount());
            LOG_N << "\tskin source:" << skin_controller->getSource().toAscii();
            LOG_N << "\tskin controller:" << skin_controller->getUniqueId().toAscii();
            LOG_N << "\tskin controller data:" << skin_controller->getSkinControllerData().toAscii();
            break;
        }
        case COLLADAFW::Controller::CONTROLLER_TYPE_MORPH:
            break;
    }
    return true;
}

bool my_collada_writer_t::writeFormulas(const COLLADAFW::Formulas *formulas) {
    LOG_N << "formulas:";
    return true;
}

bool my_collada_writer_t::writeKinematicsScene(const COLLADAFW::KinematicsScene *kinematicsScene) {
    LOG_N << "kinematics scene:";
    return true;
}

my_collada_writer_t::my_collada_writer_t(primitive_data::virsual_scene_t &primitive_visual_scene)
        : IWriter(), primitive_visual_scene(primitive_visual_scene) {}
}