//
// Created by chudonghao on 17-12-22.
//

#ifndef UNNAMED_GAME_DATA_H
#define UNNAMED_GAME_DATA_H


#include <map>
#include <set>
#include "mesh.h"
#include "id_shared_ptr_map.h"
#include "armature.h"
#include "camera.h"

namespace unnamed_game {

class data_t : public data_block_t {
public:
    data_t();
    //TODO 是否考虑用set
    id_shared_ptr_map_t<unnamed_game::mesh_t> meshes;
    id_shared_ptr_map_t<unnamed_game::material_t> materials;
    id_shared_ptr_map_t<unnamed_game::object_t> objects;
    id_shared_ptr_map_t<unnamed_game::armature_t> armatures;
    id_shared_ptr_map_t<unnamed_game::camera_t> cameras;
    id_shared_ptr_map_t<unnamed_game::scene_t> scenes;
};

}


#endif //UNNAMED_GAME_DATA_H
