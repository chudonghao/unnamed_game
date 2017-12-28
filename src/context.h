//
// Created by chudonghao on 17-12-22.
//

#ifndef UNTITLED_GAME_CONTEXT_H
#define UNTITLED_GAME_CONTEXT_H

#include "data.h"

namespace untitled_game {

class context_t : public data_block_t {
public:
    context_t();

    typedef std::shared_ptr<context_t> shared_ptr;
    data_t::shared_ptr data;
    scene_t::shared_ptr active_scene;
};

}

#endif //UNTITLED_GAME_CONTEXT_H
