# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

list(APPEND EXTRA_COMPONENT_DIRS "../../drivers")
list(APPEND EXTRA_COMPONENT_DIRS "../../modules")
list(APPEND EXTRA_COMPONENT_DIRS "../../data")

include_directories(${PROJECT_NAME} ../../drivers)
include_directories(${PROJECT_NAME} ../../modules)
include_directories(${PROJECT_NAME} ../../data)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(aplicacion)
