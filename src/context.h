//
// Created by chudonghao on 17-12-22.
//

#ifndef UNNAMED_GAME_CONTEXT_H
#define UNNAMED_GAME_CONTEXT_H

#include "data.h"

namespace unnamed_game {

class context_t : public data_block_t {
public:
    context_t();

    std::shared_ptr<data_t> data;
    std::shared_ptr<scene_t> active_scene;
};

}

#endif //UNNAMED_GAME_CONTEXT_H
