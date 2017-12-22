//
// Created by chudonghao on 17-12-22.
//

#ifndef UNTITLED_GAME_DATA_H
#define UNTITLED_GAME_DATA_H


#include <map>
#include "mesh.h"

namespace untitled_game {

class data_t {
public:
    typedef std::shared_ptr<data_t> shared_ptr;
    typedef std::unique_ptr<data_t> unique_ptr;

    data_t();

    std::map<std::string/*name*/, untitled_game::mesh_t::shared_ptr> meshs;
    std::map<std::string/*name*/, untitled_game::material_t::shared_ptr> materials;

};

}


#endif //UNTITLED_GAME_DATA_H
