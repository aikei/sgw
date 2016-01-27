cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(sgw)
# config
# version number
set (SGW_VERSION_MAJOR 0)
set (SGW_VERSION_MINOR 1)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

set (LIBS "glfw3" "glut" "GL" "GLEW")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# add_definitions(-DPRINT_DEBUG)
add_definitions(-DSGW_RENDERER_OPENGL)
# add_definitions(-DSGW_VIRTUAL_RENDERER_INTERFACE)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/cmakeConfig.h.in"
  "${PROJECT_BINARY_DIR}/cmakeConfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find cmakeConfig.h
include_directories(
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/include"
    "${GLFW_INCLUDE_DIRS}"
)

# include_files("${PROJECT_SOURCE_DIR}/cmakeConfig.h")
# install (FILES "${PROJECT_SOURCE_DIR}/cmakeConfig.h", DESTINATION include)


add_library(sgw 
    src/utils/utils.cpp
    src/app/App.cpp src/app/BaseApp.cpp src/app/GlutApp.cpp src/app/GlfwApp.cpp
    src/renderer/OpenGLRenderer.cpp src/renderer/BaseRenderer.cpp
    src/math/math.cpp src/math/transform.cpp src/math/camera.cpp)
target_link_libraries(sgw ${GLFW_STATIC_LIBRARIES} ${LIBS})

add_executable(example example/example.cpp)
target_link_libraries(example sgw)

add_executable(runtest test/test.cpp test/check.cpp)
target_link_libraries(runtest sgw)

set_property(TARGET sgw PROPERTY CXX_STANDARD 11)
set_property(TARGET runtest PROPERTY CXX_STANDARD 11)
