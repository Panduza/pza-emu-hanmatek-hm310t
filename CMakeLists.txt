# cmake_minimum_required(VERSION 3.16)
cmake_minimum_required(VERSION 3.17)

#Import pico-sdk librairie
set(FETCHCONTENT_QUIET FALSE)
set(PICO_SDK_FETCH_FROM_GIT ON)
set(PICO_SDK_FETCH_FROM_GIT_PATH ${CMAKE_CURRENT_LIST_DIR}/pico-sdk)
set(PICO_TOOLCHAIN_PATH "${PICO_TOOLCHAIN_PATH}" CACHE INTERNAL "")

# Import liblightmodbus librairie
set (LIBLIGHTMODBUS_FETCH_FROM_GIT ON)
set(LIBLIGHTMODBUS_FROM_GIT_PATH ${CMAKE_CURRENT_LIST_DIR}/liblightmodbus)

include(pico_sdk_import.cmake)
include(liblightmodbus_import.cmake)
include(${LIBLIGHTMODBUS_PATH}/lightmodbusConfig.cmake)
project(firmware C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# to use the sdk
pico_sdk_init()

# if (TARGET tinyusb_device)
add_executable(firmware
    firmware.c
)

# pull in common dependencies
target_link_libraries(firmware pico_stdlib lightmodbus::lightmodbus)

# enable usb output, disable uart output
pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 1)

# create binaries
pico_add_extra_outputs(firmware)

# add url via pico_set_program_url
# example_auto_set_url(firmware)

# elseif(PICO_ON_DEVICE)
#     message(WARNING "not building firmware because TinyUSB submodule is not initialized in the SDK")
# endif()

add_definitions(-DUSBD_VID=0x01A86) # ID_VENDOR_ID
add_definitions(-DUSBD_PID=0x07523) # ID_MODEL_ID 7523
add_definitions(-DUSBD_MANUFACTURER="QinHeng Electronics") # ID_VENDOR
add_definitions(-DUSBD_PRODUCT="USB Serial") # ID_MODEL
