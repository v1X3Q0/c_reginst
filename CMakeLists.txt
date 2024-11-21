cmake_minimum_required(VERSION 3.5.0)
project(c_reginst VERSION 0.1.0)

if(NOT DEFINED ANDROID_TOOL_BASE)
    set(ANDROID_TOOL_BASE ${CMAKE_CURRENT_SOURCE_DIR})
    message(WARNING "set ANDROID_TOOL_BASE to ${ANDROID_TOOL_BASE}")

    add_subdirectory(common/hde_armv8)
    add_subdirectory(common/hde_x86)
    add_subdirectory(common/local_util)
    add_subdirectory(common/bgrep_c)
endif()

# ===========================================
# ============ toolchain arch ===============
# ===========================================
# mac and android use arm64 and arm64e, linux uses aarch64 
# if (ARCH MATCHES "arm64" OR ARCH MATCHES "arm64e" OR ARCH MATCHES "aarch64")
#     add_definitions(-DSUPPORT_AARCH64=1)
# # I believe windows does AMD64 and linux does x86_64
# elseif(ARCH MATCHES "AMD64" OR ARCH MATCHES "i386" OR ARCH MATCHES "x86_64")
#     add_definitions(-DSUPPORT_X86_64=1)
# endif()

set(SOURCES_LIST_BASE
    # opcOperand.cpp
    # ibeSet.cpp
    )
set(INCLUDE_LIST_BASE
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/bgrep_c)

set(LIBRARY_LIST_BASE
    bgrep_e)
    
set(SOURCES_LIST_A64 ${SOURCES_LIST_BASE}
    # arm64/opcOp_arch.cpp
    )
set(INCLUDE_LIST_A64 ${INCLUDE_LIST_BASE} ${ANDROID_TOOL_BASE}/common/hde_armv8)
set(LIBRARY_LIST_A64 ${LIBRARY_LIST_BASE} hdeA64)

set(SOURCES_LIST_X86 ${SOURCES_LIST_BASE}
    # amd64/opcOp_arch.cpp
    )
set(INCLUDE_LIST_X86 ${INCLUDE_LIST_BASE} ${ANDROID_TOOL_BASE}/common/hde_x86)
set(LIBRARY_LIST_X86 ${LIBRARY_LIST_BASE} hde_x86)

# architecture libraries
add_library(${PROJECT_NAME}_x86 STATIC amd64/opcOp_arch.cpp)
target_include_directories(${PROJECT_NAME}_x86 PRIVATE ${INCLUDE_LIST_X86})
target_link_libraries(${PROJECT_NAME}_x86 PRIVATE ${LIBRARY_LIST_X86})
target_compile_definitions(${PROJECT_NAME}_x86 PRIVATE SUPPORT_X86_64=1)

add_library(${PROJECT_NAME}_A64 STATIC arm64/opcOp_arch.cpp)
target_include_directories(${PROJECT_NAME}_A64 PRIVATE ${INCLUDE_LIST_A64})
target_link_libraries(${PROJECT_NAME}_A64 PRIVATE ${LIBRARY_LIST_A64})
target_compile_definitions(${PROJECT_NAME}_A64 PRIVATE SUPPORT_AARCH64=1)

# add_library(${PROJECT_NAME} SHARED ${SOURCES_LIST_BASE})
# target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST_BASE}
#     ${ANDROID_TOOL_BASE}/common/hde_armv8 ${ANDROID_TOOL_BASE}/common/hde_x86)
# target_link_libraries(${PROJECT_NAME} ${LIBRARY_LIST_BASE} ${PROJECT_NAME}_x86 ${PROJECT_NAME}_A64)
# target_compile_definitions(${PROJECT_NAME} PRIVATE SUPPORT_AARCH64 SUPPORT_X86_64)

add_subdirectory(regexcaller)