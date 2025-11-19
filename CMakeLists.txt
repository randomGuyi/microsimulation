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
        SvgWidgets
        REQUIRED)

link_directories(
        /opt/homebrew/opt/qscintilla2/lib
)

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
        src/core/common/architecture_ids.h
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
        src/core/microcode/addrr_word.cpp
        src/core/microcode/addrr_word.h
        src/core/microcode/decode_word.cpp
        src/core/microcode/decode_word.h
        src/core/microcode/exec_word.cpp
        src/core/microcode/exec_word.h
        src/core/microcode/fetch_word.cpp
        src/core/microcode/fetch_word.h
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
        src/ui/components/msim_decoder_widget.cpp
        src/ui/components/msim_decoder_widget.h
)

target_link_libraries(microsimulation
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg # Svg Renderer
        Qt6::SvgWidgets # QGraphicsSvgItem
        qscintilla2_qt6
)

target_include_directories(microsimulation PUBLIC
        /opt/homebrew/opt/qscintilla2/include
        /opt/homebrew/opt/qscintilla2/include/Qsci
        ${CMAKE_SOURCE_DIR}/src
)
