//
// Created by chudonghao on 17-12-23.
//

#ifndef UNNAMED_GAME_DATA_BLOCK_H
#define UNNAMED_GAME_DATA_BLOCK_H

#include <string>
#include <memory>

namespace unnamed_game {

class data_block_t {
public:
    typedef std::shared_ptr<data_block_t> shared_ptr;

    virtual ~data_block_t() = default;
};

}

#endif //UNNAMED_GAME_DATA_BLOCK_H
