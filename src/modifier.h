//
// Created by chudonghao on 17-12-31.
//

#ifndef UNNAMED_GAME_MODIFIER_H
#define UNNAMED_GAME_MODIFIER_H

#include "data_block.h"

namespace unnamed_game {

class modifier_t : public data_block_t {
public:
    enum class type_e {
        armatrue,
    };

    modifier_t(type_e type);

    type_e type;
    std::string name;
};

}

#endif //UNNAMED_GAME_MODIFIER_H
