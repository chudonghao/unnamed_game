cmake_minimum_required(VERSION 3.7)

project(unnamed_game)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

find_package(Boost 1.60.0
        COMPONENTS
        system
        filesystem
        thread
        log
        log_setup
        program_options)

find_package(Qt5 COMPONENTS Widgets)

find_package(OpenCOLLADA)

option(LOG_FILE_LINE "log file and line" ON)
option(BOOST_LOG_DYN_LINK "dynamic link boost log library" ON)

configure_file(src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h)

set(SOURCE_FILES
        src/main.cpp
        src/main_widget.ui
        src/main_widget.cpp
        src/main_widget.h
        src/gl_widget.cpp
        src/gl_widget.h
        src/render.cpp
        src/render.h
        src/log.cpp
        src/log.h
        src/config.h.in
        src/visual_scenes.cpp
        src/visual_scenes.h
        src/primitive_data.cpp
        src/primitive_data.h
        src/my_collada_writer.cpp
        src/my_collada_writer.h
        src/scene.cpp
        src/scene.h
        src/context.cpp
        src/context.h
        src/data.cpp
        src/data.h
        src/mesh.cpp
        src/mesh.h
        src/my_collada_writer1.cpp
        src/my_collada_writer1.h
        src/node.cpp
        src/node.h
        src/data_block.cpp
        src/data_block.h
        src/lamp.cpp
        src/lamp.h
        src/object.cpp
        src/object.h
        src/armature.cpp
        src/armature.h
        src/id_shared_ptr_map.cpp
        src/id_shared_ptr_map.h
        src/bone.cpp
        src/bone.h
        src/joint.cpp
        src/joint.h
        src/has_id.cpp
        src/has_id.h
        src/name_shared_ptr_map.cpp
        src/name_shared_ptr_map.h
        src/material.cpp
        src/material.h
        src/camera.cpp
        src/camera.h
        src/triangles_group.cpp
        src/triangles_group.h
        src/modifier.cpp
        src/modifier.h
        src/armature_modifier.cpp
        src/armature_modifier.h
        src/main.h)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${OPENCOLLADA_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
)

add_executable(unnamed_game ${SOURCE_FILES})
target_link_libraries(unnamed_game
        Qt5::Widgets
        Boost::system
        #        Boost::thread
        Boost::filesystem
        Boost::log_setup
        Boost::log
        Boost::program_options
        pthread
        OpenCOLLADAFramework_static
        OpenCOLLADASaxFrameworkLoader_static
        )
