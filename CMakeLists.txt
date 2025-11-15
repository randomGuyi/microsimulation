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
        src/ui/views/msim_main.ui
        src/ui/editor/editor.cpp
        src/ui/editor/editor.h
        src/ui/editor/editor.ui
        src/ui/editor/microcodelexer.cpp
        src/ui/editor/microcodelexer.h
        src/ui/views/component_picker.cpp
        src/ui/views/component_picker.h
        src/ui/views/component_picker.ui
        src/ui/views/details_tabs.cpp
        src/ui/views/details_tabs.h
        src/ui/views/details_view.cpp
        src/ui/views/details_view.h
        src/ui/views/details_view.ui
        src/ui/views/simulator_area.cpp
        src/ui/views/simulator_area.h
        src/ui/views/simulator_area.ui
)

target_link_libraries(microsimulation
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg # Svg Renderer
)

target_include_directories(microsimulation PUBLIC
        /opt/homebrew/opt/qscintilla2/include
        /opt/homebrew/opt/qscintilla2/include/Qsci
)
