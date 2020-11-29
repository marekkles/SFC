# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(SFC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set (SRCS
    include/ACO/aco_ant.hpp
    include/ACO/aco_as.hpp
    include/ACO/aco_nodes.hpp
    include/ACO/cost_matrix_interface.hpp
    include/ACO/sample_tree.hpp
    include/ACO/test_input.hpp
    include/ACO/tree_nodes.hpp
    include/UI/MainWindow.hpp
    include/UI/MapWidget.hpp
    src/main.cpp
    src/MainWindow.cpp
    src/MapWidget.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include/)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
