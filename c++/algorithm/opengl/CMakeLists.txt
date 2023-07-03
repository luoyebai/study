cmake_minimum_required ( VERSION 3.15 )

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")

project(test)


find_package(OpenCV REQUIRED)

include_directories(
	imgui/
	imgui/backends
)

file(GLOB SRC
    imgui/*.h
    imgui/*.cpp
)

# add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

include_directories(
        imgui/examples/libs/gl3w # for GL/gl3w.h
    )
file (GLOB PLATFORM_SRC
        imgui/examples/libs/gl3w/GL/gl3w.*
        imgui/backends/imgui_impl_glfw.*
        imgui/backends/imgui_impl_opengl3.*
        # main.cpp
    )


add_executable(${PROJECT_NAME} main.cpp ${SRC} ${PLATFORM_SRC})
add_library(imguigl3 STATIC ${PLATFORM_SRC})

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
        GL
        glfw # use this lib name
        dl
    )
