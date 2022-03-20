cmake_minimum_required(VERSION 3.0.0)
project(c_reginst VERSION 0.1.0)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

add_library(${PROJECT_NAME} STATIC opcOperand.cpp ibeSet.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/bgrep_c)

if (DEFINED LIVE_KERNEL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        LIVE_KERNEL=${LIVE_KERNEL})
    if (ARCH STREQUAL "arm64")
        target_include_directories(${PROJECT_NAME} PRIVATE
            ${ANDROID_TOOL_BASE}/common/hde_armv8)
    endif()
endif()
