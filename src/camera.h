//
// Created by chudonghao on 17-12-26.
//

#ifndef UNNAMED_GAME_CAMERA_H
#define UNNAMED_GAME_CAMERA_H

#include <glm/glm.hpp>
#include "has_id.h"
#include "object.h"

namespace unnamed_game {

class camera_t : public has_id_t {
public:
    enum class type_e {
        perspective,
        orthographic
    };

    camera_t();

    ~camera_t() override;

    type_e type;
    float x_fov;
    float aspect_ratio;
    float near_clipping_plane;
    float far_clipping_plane;
};

glm::mat4 calculate_look_at_matrix(const std::shared_ptr<object_t> &camera_object);

glm::mat4 calculate_perspective_matrix(const std::shared_ptr<camera_t> &camera);

}


#endif //UNNAMED_GAME_CAMERA_H
