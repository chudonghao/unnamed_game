//
// Created by chudonghao on 17-12-22.
//

#ifndef UNTITLED_GAME_DATA_H
#define UNTITLED_GAME_DATA_H


#include <map>
#include <set>
#include "mesh.h"
#include "id_shared_ptr_map.h"
#include "armature.h"
#include "camera.h"

namespace untitled_game {

class data_t : public data_block_t {
public:
    typedef std::shared_ptr<data_t> shared_ptr;
    typedef std::unique_ptr<data_t> unique_ptr;

    data_t();

    //TODO 是否考虑用set
    id_shared_ptr_map_t<untitled_game::mesh_t> meshs;
    id_shared_ptr_map_t<untitled_game::material_t> materials;
    id_shared_ptr_map_t<untitled_game::object_t> objects;
    id_shared_ptr_map_t<untitled_game::armature_t> armatures;
    id_shared_ptr_map_t<untitled_game::camera_t> cameras;
    id_shared_ptr_map_t<untitled_game::scene_t> scenes;
};

}


#endif //UNTITLED_GAME_DATA_H
