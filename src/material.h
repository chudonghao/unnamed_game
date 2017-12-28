//
// Created by chudonghao on 17-12-26.
//

#ifndef UNTITLED_GAME_MATERIAL_H
#define UNTITLED_GAME_MATERIAL_H

#include "has_id.h"

namespace untitled_game {

class material_t : public has_id_t {
public:
    typedef std::shared_ptr<material_t> shared_ptr;

};

}

#endif //UNTITLED_GAME_MATERIAL_H
