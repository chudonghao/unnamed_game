//
// Created by chudonghao on 17-11-28.
//

#ifndef UNTITLED_GAME_GL_WIDGET_H
#define UNTITLED_GAME_GL_WIDGET_H

#include <QtCore/QArgument>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include "render.h"
#include "scene.h"
#include "context.h"

namespace untitled_game {
    class gl_widget_t : public QOpenGLWidget{
    Q_OBJECT
    public:
        explicit gl_widget_t(QWidget *parent = nullptr);

    protected:
        void initializeGL() override;

        void resizeGL(int w, int h) override;

        void paintGL() override;

    private:
        QOpenGLShaderProgram *opengl_shader_program;
        untitled_game::scene_t scene;
        untitled_game::context_t::shared_ptr context;
        int attr_pos;
        int attr_col;
        int uniform_matrix;
        int width, height;
    };
}
#endif //UNTITLED_GAME_GL_WIDGET_H
