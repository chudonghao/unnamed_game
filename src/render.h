//
// Created by chudonghao on 17-11-28.
//

#ifndef UNNAMED_GAME_RENDER_H
#define UNNAMED_GAME_RENDER_H

#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShader>
#include <glm/glm.hpp>

namespace unnamed_game {

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


#endif //UNNAMED_GAME_RENDER_H
