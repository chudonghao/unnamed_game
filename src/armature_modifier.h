//
// Created by chudonghao on 17-12-31.
//

#ifndef UNNAMED_GAME_ARMATURE_MODIFIER_H
#define UNNAMED_GAME_ARMATURE_MODIFIER_H

#include "modifier.h"
#include "object.h"

namespace unnamed_game {

class armatrue_modifier_t : public modifier_t {
public:
    armatrue_modifier_t();

    //armature object
    std::shared_ptr<object_t> object;
};

}

#endif //UNNAMED_GAME_ARMATURE_MODIFIER_H
