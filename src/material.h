//
// Created by chudonghao on 17-12-26.
//

#ifndef UNNAMED_GAME_MATERIAL_H
#define UNNAMED_GAME_MATERIAL_H

#include "has_id.h"

namespace unnamed_game {

class material_t : public has_id_t {
public:
    typedef std::shared_ptr<material_t> shared_ptr;

};

}

#endif //UNNAMED_GAME_MATERIAL_H
