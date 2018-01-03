//
// Created by chudonghao on 17-12-26.
//

#ifndef UNNAMED_GAME_HAS_ID_H
#define UNNAMED_GAME_HAS_ID_H

#include <string>
#include "data_block.h"

namespace unnamed_game {

class has_id_t : public data_block_t {
public:
    std::string id;

    has_id_t() = default;

    ~has_id_t() override = default;
};

}

#endif //UNNAMED_GAME_HAS_ID_H
