//
// Created by chudonghao on 17-12-23.
//

#ifndef UNTITLED_GAME_ARMATURE_H
#define UNTITLED_GAME_ARMATURE_H

#include "bone.h"
#include "has_id.h"
#include "name_shared_ptr_map.h"

namespace untitled_game {

class armature_t : public has_id_t {
public:
    typedef std::shared_ptr<armature_t> shared_ptr;
    name_shared_ptr_map_t<bone_t> bones;
};

}

#endif //UNTITLED_GAME_ARMATURE_H
