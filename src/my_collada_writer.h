//
// Created by chudonghao on 17-12-4.
//

#ifndef UNTITLED_GAME_MY_COLLADA_WRITER_H
#define UNTITLED_GAME_MY_COLLADA_WRITER_H

#include <map>
#include <boost/any.hpp>
#include <COLLADAFW.h>
#include "primitive_data.h"
#include "context.h"

namespace untitled_game {
    class my_collada_writer_t : public COLLADAFW::IWriter {
        // member declarations
    private:
        std::map<COLLADAFW::UniqueId, primitive_data::geometry_t*> id_geometry_map;
        std::map<COLLADAFW::UniqueId, primitive_data::joint_t*> id_joint_map;
        std::map<COLLADAFW::UniqueId, primitive_data::controller_t*> id_controller_map;
        std::map<COLLADAFW::UniqueId, primitive_data::skeleton_controller_data_t*> id_skeleton_controller_data_map;
        std::map<COLLADAFW::UniqueId, boost::any> id;
        std::map<primitive_data::controller_t*,std::vector<COLLADAFW::UniqueId>> controller_joint_ids_map;
        primitive_data::virsual_scene_t &primitive_visual_scene;
        untitled_game::context_t::shared_ptr context;
        // public function declarations
    public:
        explicit my_collada_writer_t(primitive_data::virsual_scene_t &primitive_visual_scene,
                                     untitled_game::context_t::shared_ptr context);

        ~my_collada_writer_t() override = default;

        /** This method will be called if an error in the loading process occurred and the loader cannot
        continue to to load. The writer should undo all operations that have been performed.
        @param errorMessage A message containing informations about the error that occurred.
        */
        void cancel(const COLLADAFW::String &errorMessage) override;

        /** This is the method called. The writer hast to prepare to receive data.*/
        void start() override;

        /** This method is called after the last write* method. No other methods will be called after this.*/
        void finish() override;

        /** When this method is called, the writer must write the global document asset.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeGlobalAsset(const COLLADAFW::FileInfo *asset) override;

        /** When this method is called, the writer must write the entire visual scene.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeScene(const COLLADAFW::Scene *scene) override;

    private:
        void process_node(primitive_data::node_t &primitive_node, COLLADAFW::Node *node, int deepth);

    public:
        /** When this method is called, the writer must write the entire visual scene.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeVisualScene(const COLLADAFW::VisualScene *visualScene) override;

        /** When this method is called, the writer must handle all nodes contained in the
        library nodes.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeLibraryNodes(const COLLADAFW::LibraryNodes *libraryNodes) override;

        /** When this method is called, the writer must write the geometry.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeGeometry(const COLLADAFW::Geometry *geometry) override;

        /** When this method is called, the writer must write the material.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeMaterial(const COLLADAFW::Material *material) override;

        /** When this method is called, the writer must write the effect.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeEffect(const COLLADAFW::Effect *effect) override;

        /** When this method is called, the writer must write the camera.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeCamera(const COLLADAFW::Camera *camera) override;

        /** When this method is called, the writer must write the image.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeImage(const COLLADAFW::Image *image) override;

        /** When this method is called, the writer must write the light.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeLight(const COLLADAFW::Light *light) override;

        /** Writes the animation.
        @return True on succeeded, false otherwise.*/
        bool writeAnimation(const COLLADAFW::Animation *animation) override;

        /** When this method is called, the writer must write the AnimationList.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeAnimationList(const COLLADAFW::AnimationList *animationList) override;

        /** When this method is called, the writer must write the skin controller data.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeSkinControllerData(const COLLADAFW::SkinControllerData *skinControllerData) override;

        /** When this method is called, the writer must write the controller.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeController(const COLLADAFW::Controller *controller) override;

        /** When this method is called, the writer must write the formula.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeFormulas(const COLLADAFW::Formulas *formulas) override;

        /** When this method is called, the writer must write the kinematics scene.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        bool writeKinematicsScene(const COLLADAFW::KinematicsScene *kinematicsScene) override;

/** Disable default copy ctor. */
        my_collada_writer_t(const my_collada_writer_t &pre) = delete;

/** Disable default assignment operator. */
        const my_collada_writer_t &operator=(const my_collada_writer_t &pre) = delete;

    };
}


#endif //UNTITLED_GAME_MY_COLLADA_WRITER_H
