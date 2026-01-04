cmake_minimum_required(VERSION 3.31)
project(fontRenderer)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_SOURCES
        externalLibs/imgui/imgui.cpp
        externalLibs/imgui/imgui_draw.cpp
        externalLibs/imgui/imgui_tables.cpp
        externalLibs/imgui/imgui_widgets.cpp
)

include_directories(fontRenderer
        externalLibs/raylib/raylib-5.5_linux_amd64/include
        externalLibs/raylib/rlImGui
        externalLibs/imgui
)

add_executable(fontRenderer
        main.cpp
        externalLibs/raylib/rlImGui/rlImGui.cpp
        ${IMGUI_SOURCES}
)

target_link_libraries(fontRenderer
        ${CMAKE_SOURCE_DIR}/externalLibs/raylib/raylib-5.5_linux_amd64/lib/libraylib.a   # raylib
        m        # math library (libm)
        pthread  # threading
        dl       # dynamic loading
        rt       # real-time extensions
        X11      # X Window System
)