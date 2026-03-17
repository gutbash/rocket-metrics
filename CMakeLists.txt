cmake_minimum_required(VERSION 3.16)
project(RocketMetricsPlugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BAKKESMOD_SDK_DEFAULT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/BakkesModSDK")
set(BAKKESMOD_SDK_PATH "$ENV{BAKKESMOD_SDK_PATH}" CACHE PATH "Path to the BakkesMod SDK root")
option(ROCKETMETRICS_REQUIRE_SDK "Fail CMake configure when BakkesMod SDK is missing" OFF)

if(NOT BAKKESMOD_SDK_PATH AND EXISTS "${BAKKESMOD_SDK_DEFAULT_PATH}")
    set(BAKKESMOD_SDK_PATH "${BAKKESMOD_SDK_DEFAULT_PATH}" CACHE PATH "Path to the BakkesMod SDK root" FORCE)
endif()

set(ROCKETMETRICS_HAS_SDK OFF)
if(BAKKESMOD_SDK_PATH
   AND EXISTS "${BAKKESMOD_SDK_PATH}/include"
   AND EXISTS "${BAKKESMOD_SDK_PATH}/lib")
    set(ROCKETMETRICS_HAS_SDK ON)
endif()

if(ROCKETMETRICS_REQUIRE_SDK AND NOT ROCKETMETRICS_HAS_SDK)
    message(FATAL_ERROR "BakkesMod SDK not found. Set BAKKESMOD_SDK_PATH to a folder containing include/ and lib/.")
endif()

if(ROCKETMETRICS_HAS_SDK)
    file(GLOB PLUGIN_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

    add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES})

    # BakkesMod loads plugin DLLs without the "lib" prefix.
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" OUTPUT_NAME "RocketMetrics")

    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${BAKKESMOD_SDK_PATH}/include
    )

    target_link_directories(${PROJECT_NAME} PRIVATE ${BAKKESMOD_SDK_PATH}/lib)
    target_link_libraries(${PROJECT_NAME} PRIVATE bakkesmod)
else()
    message(STATUS "BakkesMod SDK not found; skipping plugin target creation. Configure-only mode is active.")
endif()
