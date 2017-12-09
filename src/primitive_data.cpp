//
// Created by chudonghao on 17-12-4.
//

#include "primitive_data.h"


namespace untitled_game::primitive_data {

node_t::node_t(type_e type) : type(type),/* location(), rotation_z(), rotation_y(), rotation_x(), scale(),*/
                              transformation_matrix(), child_nodes(), camera_instance(), geometry_instance(),
                              light_instance(), controller_instance() {}

node_t::node_t()
        : transformation_matrix(), type(), camera_instance(), geometry_instance(), light_instance(),
          controller_instance(), child_nodes() {}

virsual_scene_t::virsual_scene_t() : geometries(), nodes(), controllers(), skeleton_controller_datas() {}

geometry_t::geometry_t() : mesh() {}

mesh_t::mesh_t() : positions(), normals(), uvcoords(), triangles_list() {}

controller_t::controller_t() : skin_controller() {}

skeleton_controller_data_t::skeleton_controller_data_t()
        : bind_shape_matrix(), weights(), joint_indices(), weight_indices(), joint_count_per_position(),
          inverse_bind_matrices() {}
}