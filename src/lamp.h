//
// Created by chudonghao on 17-12-23.
//

#ifndef UNNAMED_GAME_LAMP_H
#define UNNAMED_GAME_LAMP_H

#include <memory>
#include "has_id.h"

namespace unnamed_game {

class lamp_t : public has_id_t {
public:
    typedef std::shared_ptr<lamp_t> shared_ptr;

    lamp_t();
};

}
#endif //UNNAMED_GAME_LAMP_H
