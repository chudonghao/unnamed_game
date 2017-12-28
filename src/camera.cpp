//
// Created by chudonghao on 17-12-26.
//

#include "config.h"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "log.h"

namespace untitled_game {

camera_t::~camera_t() {

}

camera_t::camera_t()
        : type(), x_fov(45.0f / 180.f * 3.141592653f), aspect_ratio(1.77777f), near_clipping_plane(0.1f),
          far_clipping_plane(100.f) {}

glm::mat4 calculate_look_at_matrix(const object_t::shared_ptr &camera_object) {
    if (!camera_object) {
        LOG_W << "object is nullptr.";
        return glm::lookAt(glm::vec3(0.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
    }
    if (camera_object->type != object_t::type_e::camera) {
        LOG_W << "object is not a camera.";
        return glm::lookAt(glm::vec3(0.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
    }
    //TODO 考虑使用别的方式获得lookat矩阵
    glm::vec4 up(0.f, 1.f, 0.f, 1.f);
    glm::vec4 center(0.f, 0.f, -1.f, 1.f);
    glm::vec4 eye(0.f, 0.f, 0.f, 1.f);
    //TODO *matrix_world
    up = camera_object->matrix_local * up;
    center = camera_object->matrix_local * center;
    eye = camera_object->matrix_local * eye;
    return glm::lookAt(glm::vec3(eye), glm::vec3(center), glm::vec3(up - eye));
}

glm::mat4 calculate_perspective_matrix(const camera_t::shared_ptr &camera) {
    if (!camera) {
        LOG_W << "camera is nullptr.";
        return glm::perspective((float) M_PI / 4, 1.f, 0.1f, 100.f);
    }
    return glm::perspective(camera->x_fov, camera->aspect_ratio, camera->near_clipping_plane,
                            camera->far_clipping_plane);
}

}
