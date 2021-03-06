CMAKE_MINIMUM_REQUIRED (VERSION 2.4.7 FATAL_ERROR)
project (baker)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_definitions (-Wl,-z,defs)

SET (CMAKE_C_FLAGS "-Wall -Winvalid-pch -ffunction-sections -fdata-sections")
SET (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
SET (CMAKE_C_FLAGS_RELEASE "-Wall -Winvalid-pch -ffunction-sections -fdata-sections -ffast-math -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c++11")

SET (CMAKE_VERBOSE_MAKEFILE off)

#find_package (Boost REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})
#link_directories(${Boost_LIBRARY_DIRS})
#message (${Boost_INCLUDE_DIRS})

include (FindPkgConfig)
#pkg_check_modules (GLIB REQUIRED "glib-2.0")
#INCLUDE_DIRECTORIES (${GLIB_INCLUDE_DIRS})
#link_directories(${GLIB_LIBRARY_DIRS})

pkg_check_modules (LIBUSBX REQUIRED "libusb-1.0")
INCLUDE_DIRECTORIES (${LIBUSBX_INCLUDE_DIRS})
link_directories(${LIBUSBX_LIBRARY_DIRS})

INCLUDE_DIRECTORIES ("src/")
INCLUDE_DIRECTORIES ("../common/")
LIST (APPEND APP_SOURCES "src/Exception.h")
LIST (APPEND APP_SOURCES "src/main.cc")
LIST (APPEND APP_SOURCES "src/UsbService.h")
LIST (APPEND APP_SOURCES "src/UsbService.cc")
LIST (APPEND APP_SOURCES "../common/reflow.h")

add_executable (${PROJECT_NAME} ${APP_SOURCES})
#TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${GLIB_LIBRARIES})
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${LIBUSBX_LIBRARIES})
#TARGET_LINK_LIBRARIES (${PROJECT_NAME} boost_timer)
#TARGET_LINK_LIBRARIES (${PROJECT_NAME} boost_system) # Timer requires that
