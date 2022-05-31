cmake_minimum_required(VERSION 3.0.0)
project(c_reginst VERSION 0.1.0)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

set(SOURCES_LIST opcOperand.cpp ibeSet.cpp)
set(INCLUDE_LIST
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/bgrep_c)
if(ARCH STREQUAL "arm64" OR ARCH STREQUAL "arm64e")
    list(APPEND SOURCES_LIST arm64/opcOp_arch.cpp)
    list(APPEND INCLUDE_LIST ${ANDROID_TOOL_BASE}/common/hde_armv8)
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(capstone)
    target_include_directories(${PROJECT_NAME} PRIVATE ${capstone_INCLUDE_DIRS})
endif()
