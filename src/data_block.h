//
// Created by chudonghao on 17-12-23.
//

#ifndef UNTITLED_GAME_DATA_BLOCK_H
#define UNTITLED_GAME_DATA_BLOCK_H

#include <string>
#include <memory>

namespace untitled_game {

class data_block_t {
public:
    typedef std::shared_ptr<data_block_t> shared_ptr;

    virtual ~data_block_t() = default;
};

}

#endif //UNTITLED_GAME_DATA_BLOCK_H
