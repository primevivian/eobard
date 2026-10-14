cmake_minimum_required(VERSION 3.13)

# Set project to RP2040/OG Pico
set(PICO_BOARD pico) 

include(pico_sdk_import.cmake)

project(eobard_firmware C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_executable(eobard_firmware)

target_sources(eobard_firmware PRIVATE main.c usb_descriptors.c)

# Let TinyUSB find tusb_config.h
target_include_directories(eobard_firmware PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(eobard_firmware pico_stdlib tinyusb_device)

target_compile_options(eobard_firmware PRIVATE -Wall)

# Keep debug out of USB since project uses it
pico_enable_stdio_usb(eobard_firmware 0)
pico_enable_stdio_uart(eobard_firmware 1)

# .uf2 .bin .hex .map extra outputs
pico_add_extra_outputs(eobard_firmware)