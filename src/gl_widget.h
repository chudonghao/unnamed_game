//
// Created by chudonghao on 17-11-28.
//

#ifndef UNTITLED_GAME_GL_WIDGET_H
#define UNTITLED_GAME_GL_WIDGET_H

#include "config.h"
#include <QtCore/QArgument>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include "render.h"

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
        GLuint attr_pos;
        GLuint attr_col;
        GLuint uniform_matrix;
        int width, height;
    };
}
#endif //UNTITLED_GAME_GL_WIDGET_H
