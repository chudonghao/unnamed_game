//
// Created by chudonghao on 17-12-23.
//

#ifndef UNNAMED_GAME_JOINT_H
#define UNNAMED_GAME_JOINT_H

#include <memory>
#include <list>
#include <glm/glm.hpp>

namespace unnamed_game {

class joint_t {
public:

    joint_t();

    std::list<std::shared_ptr<joint_t>> child_joints;
public:
    glm::mat4 world_transformation;
};

}

#endif //UNNAMED_GAME_JOINT_H
