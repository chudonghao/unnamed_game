//
// Created by chudonghao on 17-11-28.
//

#ifndef UNTITLED_GAME_RENDER_H
#define UNTITLED_GAME_RENDER_H

#include "config.h"
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShader>
#include <glm/glm.hpp>

namespace untitled_game {

    class render_t {
    public:
        render_t();

        ~render_t() {}

        void resize(int w, int h);

        void paint();

        void initialize();

    private:

    };

}


#endif //UNTITLED_GAME_RENDER_H
