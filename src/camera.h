//
// Created by chudonghao on 17-12-26.
//

#ifndef UNTITLED_GAME_CAMERA_H
#define UNTITLED_GAME_CAMERA_H

#include <glm/glm.hpp>
#include "has_id.h"
#include "object.h"

namespace untitled_game {

class camera_t : public has_id_t {
public:
    typedef std::shared_ptr<camera_t> shared_ptr;
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

glm::mat4 calculate_look_at_matrix(const object_t::shared_ptr &camera_object);

glm::mat4 calculate_perspective_matrix(const camera_t::shared_ptr &camera);

}


#endif //UNTITLED_GAME_CAMERA_H
