//
// Created by chudonghao on 17-11-28.
//
#include <QtGui/QOpenGLContext>
#include <COLLADAFW.h>
#include <COLLADASaxFWLLoader.h>
#include <boost/filesystem.hpp>
#include <QtGui/QOpenGLFunctions_3_0>
#include <glm/gtx/transform.hpp>
#include "gl_widget.h"
#include "my_collada_writer.h"

using namespace boost;
using namespace std;
namespace untitled_game {
    gl_widget_t::gl_widget_t(QWidget *parent) : QOpenGLWidget(parent) {
        QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setProfile(QSurfaceFormat::CoreProfile);
        setFormat(format);
        context = context_t::shared_ptr(new context_t());
    }

    void gl_widget_t::initializeGL() {
        filesystem::path dae_file_path = "second.dae";
        if (filesystem::is_regular_file(dae_file_path)) {
            std::string dae_string;
            filesystem::load_string_file(dae_file_path, dae_string);
            COLLADASaxFWL::Loader loader(nullptr);
            primitive_data::virsual_scene_t virsual_scene;
            my_collada_writer_t writer(virsual_scene, context);
            COLLADAFW::Root root(&loader, &writer);
            root.loadDocument(dae_file_path.string());
            scene.input_scene(virsual_scene);
        } else {
            LOG_W << "Con't find dae file.\n";
        }
        auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
        f->glClearColor(1.f, 1.f, 1.f, 1.f);
        f->glEnable(GL_DEPTH_TEST);
        opengl_shader_program = new QOpenGLShaderProgram();
        static const char *vertex_shader_source =
                "attribute highp vec4 attr_pos;\n"
                        "attribute lowp vec4 attr_col;\n"
                        "varying lowp vec4 col;\n"
                        "uniform highp mat4 uniform_matrix;\n"
                        "void main() {\n"
                        "   col = attr_col;\n"
                        "   gl_Position = uniform_matrix * attr_pos;\n"
                        "}\n";

        static const char *fragment_shader_source =
                "varying lowp vec4 col;\n"
                        "void main() {\n"
                        "   gl_FragColor = col;\n"
                        "}\n";
        opengl_shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader_source);
        opengl_shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader_source);
        opengl_shader_program->link();
        attr_pos = opengl_shader_program->attributeLocation("attr_pos");
        attr_col = opengl_shader_program->attributeLocation("attr_col");
        uniform_matrix = opengl_shader_program->uniformLocation("uniform_matrix");
    }

    void gl_widget_t::resizeGL(int w, int h) {
        auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
        width = w;
        height = h;
        f->glViewport(0, 0, w, h);
    }

    void gl_widget_t::paintGL() {
        auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render();
        static float tmp = 0.f;
        if (tmp >= 1.f)
            tmp = -1.f;
        else
            tmp += 0.01f;
        opengl_shader_program->bind();
        QMatrix4x4 matrix = QMatrix4x4();
        matrix.translate(tmp, 0.5f, 0.f);
        opengl_shader_program->setUniformValue(uniform_matrix, matrix);

        //Test
        static bool init = false;
        static GLuint vao_id;
        GLuint position_buffer_id;
        GLuint normal_buffer_id;
        GLuint element_buffer_id;
        if(!init){
            vector <glm::vec3> positions;
            vector <glm::vec3> normals;
            vector<unsigned int> elements;
            positions.emplace_back(0.1f, 0.1f, 0.0f);
            positions.emplace_back(-0.1f, -0.1f, 0.0f);
            positions.emplace_back(-0.1f, 0.1f, 0.0f);
            normals.emplace_back(1.f, 0.f, 0.f);
            normals.emplace_back(0.f, 1.f, 0.f);
            normals.emplace_back(0.f, 0.f, 1.f);
            elements.push_back(0);
            elements.push_back(1);
            elements.push_back(2);
            f->glGenBuffers(1, &position_buffer_id);
            f->glBindBuffer(GL_ARRAY_BUFFER, position_buffer_id);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);
            f->glGenBuffers(1, &normal_buffer_id);
            f->glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);

            f->glGenVertexArrays(1, &vao_id);
            f->glBindVertexArray(vao_id);
            f->glGenBuffers(1, &element_buffer_id);
            f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
            f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), &elements[0],
                            GL_STATIC_DRAW);
            f->glBindBuffer(GL_ARRAY_BUFFER, position_buffer_id);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, nullptr);
            f->glEnableVertexAttribArray(0);
            f->glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
            f->glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, nullptr);
            f->glEnableVertexAttribArray(1);
            f->glBindVertexArray(0);
            init = true;
        }

        //GLfloat vertices[] = {
        //        0.0f, 0.707f,
        //        -0.5f, -0.5f,
        //        0.5f, -0.5f
        //};
        //
        //GLfloat colors[] = {
        //        1.f, 0.0f, 0.0f,
        //        0.0f, 1.f, 0.0f,
        //        0.0f, 0.0f, 1.f
        //};
        //static bool init = false;
        //static GLuint vao_id;
        //if (init == false) {
        //    f->glGenVertexArrays(1, &vao_id);
        //    f->glBindVertexArray(vao_id);
        //    GLuint i;
        //    f->glGenBuffers(1, &i);
        //    f->glBindBuffer(GL_ARRAY_BUFFER, i);
        //    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //    f->glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, 0);
        //    f->glEnableVertexAttribArray(0);
        //    f->glGenBuffers(1, &i);
        //    f->glBindBuffer(GL_ARRAY_BUFFER, i);
        //    f->glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        //    f->glVertexAttribPointer(attr_col, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //    f->glEnableVertexAttribArray(1);
        //    f->glBindVertexArray(0);
        //    init = true;
        //}

        f->glBindVertexArray(vao_id);

        //f->glDrawArrays(GL_TRIANGLES, 0, 3);
        f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        f->glBindVertexArray(0);
        opengl_shader_program->release();
        update();
    }
}