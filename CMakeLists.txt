cmake_minimum_required(VERSION 4.0)
project(microsimulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        REQUIRED)

add_executable(microsimulation src/main.cpp
        src/shared/svg_loader.cpp
        src/shared/svg_loader.h
        src/shared/constants.h
        src/ui/views/msim_main.cpp
        src/ui/views/msim_main.h
        src/ui/views/msim_main.ui)

target_link_libraries(microsimulation
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg # Svg Renderer
)

