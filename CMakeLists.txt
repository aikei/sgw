cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(sgw)
# config
# version number
set (SGW_VERSION_MAJOR 0)
set (SGW_VERSION_MINOR 1)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set (CMAKE_BUILD_TYPE RELEASE)

find_library(glutlib "glut")
find_library(GLlib "GL")
find_library(GLEWlib "GLEW")
find_library(glfwlib "glfw")

# set(PNG_LIBRARIES /home/aikei/mylibs/libpng/build/libpng.a)
# set(PNG_INCLUDE_DIRS /home/aikei/mylibs/libpng)
include("FindPNG")
include("FindZLIB")

message("ZLIB_FOUND: " ${ZLIB_FOUND})
message("PNG_FOUND: " ${PNG_FOUND})
message("ZLIB_LIBRARIES: " ${ZLIB_LIBRARIES})
message("PNG_LIBRARIES: " ${PNG_LIBRARIES})

set (LIBS ${glutlib} ${GLlib} ${GLEWlib} ${glfwlib} ${PNG_LIBRARIES} ${ZLIB_LIBRARIES})
message("glut lib: " ${glutlib})
message("GL lib: " ${GLlib})
message("GLEW lib: " ${GLEWlib})
message("glfw3 lib: " ${glfwlib})
message("libpng lib: " ${libpng})
message("zlib: " ${zlib})
# set (LIBS "glut" "GL" "GLEW")

# add_definitions(-DPRINT_DEBUG)
add_definitions(-DSGW_RENDERER_OPENGL)
add_definitions(-DSGW_COMPILER_GCC)
add_definitions(-DSGW_PLATFORM_LINUX)
# add_definitions(-DSGW_VIRTUAL_RENDERER_INTERFACE)

# find_package(PkgConfig REQUIRED)
# pkg_search_module(GLFW REQUIRED glfw3)

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
    "${PNG_INCLUDE_DIRS}"
    "${ZLIB_INCLUDE_DIRS}"
)

# include_files("${PROJECT_SOURCE_DIR}/cmakeConfig.h")
# install (FILES "${PROJECT_SOURCE_DIR}/cmakeConfig.h", DESTINATION include)


add_library(sgw 
    src/utils/utils.cpp
    src/app/App.cpp src/app/BaseApp.cpp src/app/GlutApp.cpp src/app/GlfwApp.cpp
    src/renderer/OpenGLRenderer.cpp src/renderer/BaseRenderer.cpp
    src/math/math.cpp src/math/transform.cpp src/math/camera.cpp
    src/renderer/image/Texture.cpp src/renderer/image/Image.cpp
    src/utils/Logger.cpp)
# target_link_libraries(sgw ${GLFW_LIBRARIES} ${LIBS})
target_link_libraries(sgw ${GLFW_STATIC_LIBRARIES} ${LIBS})

add_executable(example example/example.cpp)
target_link_libraries(example sgw)

add_executable(runtest test/test.cpp test/check.cpp)
target_link_libraries(runtest sgw)

set_property(TARGET sgw PROPERTY CXX_STANDARD 11)
set_property(TARGET runtest PROPERTY CXX_STANDARD 11)
set_property(TARGET example PROPERTY CXX_STANDARD 11)
