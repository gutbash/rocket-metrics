cmake_minimum_required(VERSION 3.16)
project(RocketMetricsPlugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BAKKESMOD_SDK_PATH "$ENV{BAKKESMOD_SDK_PATH}" CACHE PATH "Path to the BakkesMod SDK root")

if(NOT BAKKESMOD_SDK_PATH)
    message(FATAL_ERROR "BAKKESMOD_SDK_PATH is not set. Point it to your BakkesMod SDK folder.")
endif()

if(NOT EXISTS "${BAKKESMOD_SDK_PATH}/include")
    message(FATAL_ERROR "Invalid BAKKESMOD_SDK_PATH: missing include/ at ${BAKKESMOD_SDK_PATH}")
endif()

if(NOT EXISTS "${BAKKESMOD_SDK_PATH}/lib")
    message(FATAL_ERROR "Invalid BAKKESMOD_SDK_PATH: missing lib/ at ${BAKKESMOD_SDK_PATH}")
endif()

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
