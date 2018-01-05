//
// Created by chudonghao on 17-11-28.
//

#include "config.h"
#include <QtGui/QOpenGLContext>
#include <COLLADAFW.h>
#include <COLLADASaxFWLLoader.h>
#include <boost/filesystem.hpp>
#include <QtGui/QOpenGLFunctions_3_0>
#include <glm/gtx/transform.hpp>
#include "gl_widget.h"
#include "my_collada_writer.h"
#include "my_collada_writer1.h"
#include "log.h"

using namespace boost;
using namespace std;
namespace unnamed_game {

static void test(const glm::mat4 &m) {
    LOG_N << m[0][0] << " "
          << m[1][0] << " "
          << m[2][0] << " "
          << m[3][0];
    LOG_N << m[0][1] << " "
          << m[1][1] << " "
          << m[2][1] << " "
          << m[3][1];
    LOG_N << m[0][2] << " "
          << m[1][2] << " "
          << m[2][2] << " "
          << m[3][2];
    LOG_N << m[0][3] << " "
          << m[1][3] << " "
          << m[2][3] << " "
          << m[3][3];
}

gl_widget_t::gl_widget_t(QWidget *parent) : QOpenGLWidget(parent) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    context = std::make_shared<unnamed_game::context_t>();
}

void gl_widget_t::initializeGL() {
    filesystem::path dae_file_path = "second.dae";
    if (filesystem::is_regular_file(dae_file_path)) {
        COLLADASaxFWL::Loader loader(nullptr);

        //primitive_data::virsual_scene_t virsual_scene;
        //my_collada_writer_t writer(virsual_scene);
        //COLLADAFW::Root root(&loader, &writer);
        //root.loadDocument(dae_file_path.string());
        //
        //scene.input_scene(virsual_scene);

        my_collada_writer1_t writer1(context);
        COLLADAFW::Root root1(&loader, &writer1);
        root1.loadDocument(dae_file_path.string());

        context->active_scene = context->data->scenes.begin()->second;
        context->active_scene->active_camera = context->data->objects["Camera"];

        for (auto &&mesh: context->data->meshes) {
            LOG_N << mesh.first;
        }
        for (auto &&object: context->data->objects) {
            LOG_N << object.first << "(id=" << object.second->id << ",type=" << (int) object.second->type << ")";
            if (object.second->parent)
                LOG_N << "  parent=object(id=" << object.second->parent->id << ")";
        }
        for (auto &&armature:context->data->armatures) {
            LOG_N << armature.first << "(id=" << armature.second->id << ")";
            for (auto &&bone:armature.second->bones) {
                LOG_N << bone.first << " @" << bone.second;
                if (bone.second->parent)
                    LOG_N << "  parent:@" << bone.second->parent;
            }
        }
        for (auto &&camera:context->data->cameras) {
            LOG_N << camera.first;
            if (!camera.second)
                continue;
            LOG_N << "id=" << camera.second->id << ",type=" << (int) camera.second->type;
            LOG_N << camera.second->x_fov;
            LOG_N << camera.second->aspect_ratio;
            LOG_N << camera.second->near_clipping_plane;
            LOG_N << camera.second->far_clipping_plane;
        }
        auto &camera_object = context->data->objects["Camera"];
        test(camera_object->matrix_local);
    } else {
        LOG_W << "Con't find dae file.\n";
    }
    //Test
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

    //Test
    static float tmp = 0.f;
    if (tmp >= 1.f)
        tmp = -1.f;
    else
        tmp += 0.01f;
    opengl_shader_program->bind();
    QMatrix4x4 matrix = QMatrix4x4();
    matrix.translate(tmp, 0.5f, 0.f);
    opengl_shader_program->setUniformValue(uniform_matrix, matrix);
    static bool init = false;
    static GLuint vao_id;
    GLuint position_buffer_id;
    GLuint normal_buffer_id;
    GLuint element_buffer_id;
    if (!init) {
        vector<glm::vec3> positions;
        vector<glm::vec3> normals;
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
    f->glBindVertexArray(vao_id);
    f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    f->glBindVertexArray(0);
    opengl_shader_program->release();

    unnamed_game::render(context->active_scene);
    update();
}

void gl_widget_t::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    LOG_N << "key press event.";
    //debug
    auto camera = context->active_scene->active_camera;
    switch (event->key()) {
        case Qt::Key_W:
            //camera->matrix_local = glm::translate(glm::vec3(1.f, 0.f, 0.f)) * camera->matrix_local;
            camera->matrix_local = glm::translate(camera->matrix_local, glm::vec3(0.f, 0.f, -1.f));
            break;
        case Qt::Key_S:
            //camera->matrix_local = glm::translate(glm::vec3(-1.f, 0.f, 0.f)) * camera->matrix_local;
            camera->matrix_local = glm::translate(camera->matrix_local, glm::vec3(0.f, 0.f, 1.f));
            break;
        case Qt::Key_A:
            //camera->matrix_local = glm::translate(glm::vec3(1.f, 0.f, 0.f)) * camera->matrix_local;
            camera->matrix_local = glm::translate(camera->matrix_local, glm::vec3(-1.f, 0.f, 0.f));
            break;
        case Qt::Key_D:
            //camera->matrix_local = glm::translate(glm::vec3(-1.f, 0.f, 0.f)) * camera->matrix_local;
            camera->matrix_local = glm::translate(camera->matrix_local, glm::vec3(1.f, 0.f, 0.f));
            break;
        default:
            break;
    }
}

}
