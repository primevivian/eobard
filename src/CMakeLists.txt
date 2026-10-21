# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

# Set project to RP2040/OG Pico
set(PICO_BOARD pico)

include(pico_sdk_import.cmake)

project(eobard_firmware C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

# Build one firmware per half, eobard_left.uf2 and eobard_right.uf2
foreach(SIDE left right)
        set(TARGET eobard_${SIDE})

        add_executable(${TARGET})

        # PIO UART programs from pico-examples, used for the split link
        pico_generate_pio_header(${TARGET} ${CMAKE_CURRENT_LIST_DIR}/uart_tx.pio)
        pico_generate_pio_header(${TARGET} ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)

        target_sources(${TARGET} PRIVATE
                ${CMAKE_CURRENT_LIST_DIR}/main.c
                ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
                ${CMAKE_CURRENT_LIST_DIR}/matrix.c
                ${CMAKE_CURRENT_LIST_DIR}/split.c
                ${CMAKE_CURRENT_LIST_DIR}/keyboard.c
                ${CMAKE_CURRENT_LIST_DIR}/keymap.c
                )

        # Let TinyUSB find tusb_config.h
        target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

        target_link_libraries(${TARGET} PRIVATE
                pico_stdlib
                pico_unique_id
                pico_bootrom
                hardware_pio
                tinyusb_device
                )

        # Tell the code which half it's built for
        if (SIDE STREQUAL "left")
                target_compile_definitions(${TARGET} PRIVATE EOBARD_LEFT=1)
        else()
                target_compile_definitions(${TARGET} PRIVATE EOBARD_LEFT=0)
        endif()

        target_compile_definitions(${TARGET} PRIVATE CFG_TUSB_OS=OPT_OS_PICO)

        target_compile_options(${TARGET} PRIVATE -Wall)

        # Keep debug out of USB since project uses it
        pico_enable_stdio_usb(${TARGET} 0)
        pico_enable_stdio_uart(${TARGET} 1)

        # .uf2 .bin .hex .map extra outputs
        pico_add_extra_outputs(${TARGET})
endforeach()
