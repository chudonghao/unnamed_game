//
// Created by chudonghao on 17-11-28.
//

#ifndef UNNAMED_GAME_GL_WIDGET_H
#define UNNAMED_GAME_GL_WIDGET_H

#include <QtCore/QArgument>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include "render.h"
#include "scene.h"
#include "context.h"

namespace unnamed_game {
    class gl_widget_t : public QOpenGLWidget{
    Q_OBJECT
    public:
        explicit gl_widget_t(QWidget *parent = nullptr);

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    protected:
        void initializeGL() override;

        void resizeGL(int w, int h) override;

        void paintGL() override;

    private:
        QOpenGLShaderProgram *opengl_shader_program;
        std::shared_ptr<unnamed_game::context_t> context;

        int attr_pos;
        int attr_col;
        int uniform_matrix;
        int width, height;
    };
}
#endif //UNNAMED_GAME_GL_WIDGET_H
