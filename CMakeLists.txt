cmake_minimum_required(VERSION 4.0)
project(microsimulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Allow users to override Qt and QScintilla locations via -D or environment vars
if(NOT DEFINED QTDIR)
    if(DEFINED ENV{QT_DIR})
        set(QTDIR $ENV{QT_DIR})
    elseif(DEFINED ENV{QTDIR})
        set(QTDIR $ENV{QTDIR})
    endif()
endif()

# Platform-aware suggestions for CMAKE_PREFIX_PATH. Users may still override with -DCMAKE_PREFIX_PATH or -DQT_DIR.
if(NOT DEFINED CMAKE_PREFIX_PATH OR CMAKE_PREFIX_PATH STREQUAL "")
    if(APPLE)
        # Common Homebrew Qt locations
        if(NOT DEFINED QTDIR)
            if(EXISTS "/opt/homebrew/opt/qt")
                set(QTDIR "/opt/homebrew/opt/qt")
            elseif(EXISTS "/usr/local/opt/qt")
                set(QTDIR "/usr/local/opt/qt")
            endif()
        endif()
    elseif(WIN32)
        # Windows default Qt location - can be overridden by -DQTDIR or the QT_DIR env var
        if(NOT DEFINED QTDIR)
            set(QTDIR "C:/Qt/6.9.1/mingw_64")
        endif()
    else()
        # Linux common prefix hints
        if(NOT DEFINED QTDIR)
            set(QTDIR "/usr/lib/qt6")
        endif()
    endif()
    if(DEFINED QTDIR AND NOT QTDIR STREQUAL "")
        list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
        message(STATUS "Auto-added QTDIR to CMAKE_PREFIX_PATH: ${QTDIR}")
    endif()
endif()

message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        SvgWidgets
        REQUIRED)

option(USE_QSCINTILLA "Enable QScintilla editor support" ON)

# Allow user to specify QSCINTILLA_DIR via -DQSCINTILLA_DIR or env QSCINTILLA_DIR
if(NOT DEFINED QSCINTILLA_DIR)
    if(DEFINED ENV{QSCINTILLA_DIR})
        set(QSCINTILLA_DIR $ENV{QSCINTILLA_DIR})
    endif()
endif()

# Optional QScintilla detection. Users can provide -DQSCINTILLA_DIR=/path or set USE_QSCINTILLA=OFF to skip.
if(USE_QSCINTILLA)
    if(DEFINED QSCINTILLA_DIR AND QSCINTILLA_DIR)
        message(STATUS "Looking for QScintilla in: ${QSCINTILLA_DIR}")
        find_path(QSCINTILLA_INCLUDE_DIR
                NAMES Qsci/qsciglobal.h
                PATHS "${QSCINTILLA_DIR}/include" "${QSCINTILLA_DIR}/include/Qsci"
                NO_DEFAULT_PATH
        )

        find_library(QSCINTILLA_LIBRARY
                NAMES qscintilla2_qt6 qscintilla2
                PATHS "${QSCINTILLA_DIR}/lib" "${QSCINTILLA_DIR}/bin"
                NO_DEFAULT_PATH
        )
    else()
        # Try common system locations (Homebrew, /usr/local, Windows Qt)
        find_path(QSCINTILLA_INCLUDE_DIR
                NAMES Qsci/qsciglobal.h
                PATHS
                    /opt/homebrew/opt/qscintilla2/include
                    /usr/local/opt/qscintilla2/include
                    /usr/local/include
                    /usr/include
                    "${QTDIR}/include"
        )
        find_library(QSCINTILLA_LIBRARY
                NAMES qscintilla2_qt6 qscintilla2
                PATHS
                    /opt/homebrew/opt/qscintilla2/lib
                    /usr/local/lib
                    /usr/lib
                    "${QTDIR}/lib"
        )
    endif()

    if(QSCINTILLA_INCLUDE_DIR AND QSCINTILLA_LIBRARY)
        set(QSCINTILLA_FOUND TRUE)
        message(STATUS "Found QScintilla: include=${QSCINTILLA_INCLUDE_DIR} lib=${QSCINTILLA_LIBRARY}")
    else()
        set(QSCINTILLA_FOUND FALSE)
        message(WARNING "QScintilla not found. Editor features will be limited. Provide -DQSCINTILLA_DIR=... or set USE_QSCINTILLA=OFF to skip.")
    endif()
else()
    set(QSCINTILLA_FOUND FALSE)
endif()

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
        src/ui/views/details_view.cpp
        src/ui/views/details_view.h
        src/ui/views/details_view.ui
        src/ui/views/simulator_area.cpp
        src/ui/views/simulator_area.h
        src/ui/views/simulator_area.ui
        src/ui/views/snippet_picker.cpp
        src/ui/views/snippet_picker.h
        src/ui/views/snippet_picker.ui
        src/ui/drag_and_drop/drag_item_model.cpp
        src/ui/drag_and_drop/drag_item_model.h
        src/ui/drag_and_drop/drop_target.cpp
        src/ui/drag_and_drop/drop_target.h
        src/ui/components/debug_proxy.h
        src/ui/components/msim_alu_widget.h
        src/ui/components/msim_ar_widget.h
        src/ui/components/msim_alu_widget.cpp
        src/ui/components/msim_ar_widget.cpp
        src/ui/components/msim_bit_widget.cpp
        src/ui/components/msim_bus_widget.cpp
        src/ui/components/msim_bus_widget.h
        src/ui/components/msim_clock_widget.cpp
        src/ui/components/msim_clock_widget.h
        src/ui/components/msim_component_svg_widget.cpp
        src/ui/components/msim_component_view.cpp
        src/ui/components/msim_component_widget.cpp
        src/ui/components/msim_connector_widget.cpp
        src/ui/components/msim_connector_widget.h
        src/ui/components/msim_cop_widget.cpp
        src/ui/components/msim_cop_widget.h
        src/ui/components/msim_enable_bit_widget.cpp
        src/ui/components/msim_enable_bit_widget.h
        src/ui/components/msim_line_widget.cpp
        src/ui/components/msim_line_widget.h
        src/ui/components/msim_ram_widget.cpp
        src/ui/components/msim_ram_widget.h
        src/ui/components/msim_register_widget.cpp
        src/ui/components/msim_register_widget.h
        src/ui/components/msim_rom_widget.cpp
        src/ui/components/msim_rom_widget.h
        src/ui/controller/components_factory.cpp
        src/ui/controller/components_factory.h
        src/ui/controller/components_picker_controller.cpp
        src/ui/controller/components_picker_controller.h
        src/ui/controller/connector_factory.cpp
        src/ui/controller/connector_factory.h
        src/ui/controller/editor_controller.cpp
        src/ui/controller/editor_controller.h
        src/ui/controller/line_factory.cpp
        src/ui/controller/line_factory.h
        src/ui/controller/msim_components_manager.cpp
        src/ui/controller/msim_components_manager.h
        src/ui/controller/simulator_controller.cpp
        src/ui/controller/simulator_controller.h
        src/shared/component_info.h
        src/shared/architecture_ids.h
        src/core/common/id_reader.cpp
        src/core/common/id_reader.h
        src/core/components/msim_alu.cpp
        src/core/components/msim_alu.h
        src/core/components/msim_ar.cpp
        src/core/components/msim_ar.h
        src/core/components/msim_bit.cpp
        src/core/components/msim_bit.h
        src/core/components/msim_bus.cpp
        src/core/components/msim_bus.h
        src/core/components/msim_clock.cpp
        src/core/components/msim_clock.h
        src/core/components/msim_component.cpp
        src/core/components/msim_component.h
        src/core/components/msim_connector.cpp
        src/core/components/msim_connector.h
        src/core/components/msim_cop.cpp
        src/core/components/msim_cop.h
        src/core/components/msim_cpu.cpp
        src/core/components/msim_cpu.h
        src/core/components/msim_enable_bit.cpp
        src/core/components/msim_enable_bit.h
        src/core/components/msim_instruction_line.h
        src/core/components/msim_line.cpp
        src/core/components/msim_line.h
        src/core/components/msim_observable_component.cpp
        src/core/components/msim_observable_component.h
        src/core/components/msim_ram.cpp
        src/core/components/msim_ram.h
        src/core/components/msim_register.cpp
        src/core/components/msim_register.h
        src/core/components/msim_rom.cpp
        src/core/components/msim_rom.h
        src/core/microcode/inst_word.cpp
        src/core/microcode/inst_word.h
        src/core/microcode/msim_wrd.cpp
        src/core/microcode/msim_wrd.h
        src/ui/views/details_tabs.cpp
        src/ui/views/details_tabs.h
        src/ui/styles.h
        src/core/parser/msim_parser.cpp
        src/core/parser/msim_scanner.cpp
        res/qrc/assets.qrc
        src/core/components/msim_decoder.cpp
        src/core/components/msim_decoder.h
        src/ui/controller/decoder_factory.cpp
        src/ui/controller/decoder_factory.h
#        src/test/inst_word_tests.cpp
#        src/test/inst_word_tests.h
        src/ui/highlight_label.h
        src/ui/controller/details_controller.cpp
)

# Link Qt libraries. QScintilla is linked only if found.
target_link_libraries(microsimulation PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg # Svg Renderer
        Qt6::SvgWidgets # QGraphicsSvgItem
)

if(QSCINTILLA_FOUND)
    target_link_libraries(microsimulation PRIVATE ${QSCINTILLA_LIBRARY})
endif()

# Include directories: always include project src. Add QScintilla include dir if available.
target_include_directories(microsimulation PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

if(QSCINTILLA_FOUND)
    target_include_directories(microsimulation PUBLIC
            ${QSCINTILLA_INCLUDE_DIR}
            ${QSCINTILLA_INCLUDE_DIR}/Qsci
    )
endif()


message("Build type: ${CMAKE_BUILD_TYPE}")

# Windows deployment helper (optional): windeployqt
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QTDIR}/bin" "C:/Qt/*/bin")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET microsimulation POST_BUILD
                # --compiler-runtime grabs essential MinGW files like libstdc++-6.dll
                COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-translations
                --compiler-runtime
                "$<TARGET_FILE:microsimulation>"

                # Manually copy the QScintilla DLL since windeployqt doesn't track 3rd party libs
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QSCINTILLA_DIR}/lib/qscintilla2_qt6.dll"
                "$<TARGET_FILE_DIR:microsimulation>"

                COMMENT "Creating standalone deployment folder..."
        )
    endif()
endif()
