//
// Created by chudonghao on 17-12-23.
//

#ifndef UNNAMED_GAME_ARMATURE_H
#define UNNAMED_GAME_ARMATURE_H

#include "bone.h"
#include "has_id.h"
#include "name_shared_ptr_map.h"

namespace unnamed_game {

class armature_t : public has_id_t {
public:
    name_shared_ptr_map_t<bone_t> bones;
};

}

#endif //UNNAMED_GAME_ARMATURE_H
