//
// Created by chudonghao on 17-12-23.
//

#ifndef UNTITLED_GAME_LAMP_H
#define UNTITLED_GAME_LAMP_H

#include <memory>
#include "has_id.h"

namespace untitled_game {

class lamp_t : public has_id_t {
public:
    typedef std::shared_ptr<lamp_t> shared_ptr;

    lamp_t();
};

}
#endif //UNTITLED_GAME_LAMP_H
