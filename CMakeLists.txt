cmake_minimum_required(VERSION 3.7)

project(untitled_game)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

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
        src/mesh.h)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${OPENCOLLADA_INCLUDE_DIRS}
)

add_executable(untitled_game ${SOURCE_FILES})
target_link_libraries(untitled_game
        Qt5::Widgets
        boost_system
        boost_thread
        boost_filesystem
        boost_log_setup
        boost_log
        pthread
        OpenCOLLADAFramework_static
        OpenCOLLADASaxFrameworkLoader_static
)
