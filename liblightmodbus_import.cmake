include(FetchContent)

set(FETCHCONTENT_BASE_DIR_SAVE ${FETCHCONTENT_BASE_DIR})

if (LIBLIGHTMODBUS_FROM_GIT_PATH)
        get_filename_component(FETCHCONTENT_BASE_DIR "${LIBLIGHTMODBUS_FROM_GIT_PATH}" REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}")
endif ()

FetchContent_Declare(
        liblightmodbus
        GIT_REPOSITORY https://github.com/Jacajack/liblightmodbus.git
        GIT_TAG v3.0
)


if (NOT liblightmodbus)
        # set(FETCHCONTENT_BASE_DIR "${LIBLIGHTMODBUS_FROM_GIT_PATH}")
        message("Downloading Liblightmodbus")
        message(${FETCHCONTENT_BASE_DIR})
        FetchContent_Populate(liblightmodbus)
        set(LIBLIGHTMODBUS_PATH ${liblightmodbus_SOURCE_DIR})
endif ()

set(FETCHCONTENT_BASE_DIR ${FETCHCONTENT_BASE_DIR_SAVE})