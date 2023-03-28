cmake_minimum_required(VERSION 3.0.0)
project(c_reginst VERSION 0.1.0)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
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

set(SOURCES_LIST_BASE opcOperand.cpp ibeSet.cpp)
set(INCLUDE_LIST_BASE
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/bgrep_c)

set(LIBRARY_LIST_BASE
    bgrep_e)
    
if(ARCH MATCHES "arm64" OR ARCH MATCHES "arm64e" OR ARCH MATCHES "aarch64")
    set(SOURCES_LIST ${SOURCES_LIST_BASE} arm64/opcOp_arch.cpp)
    set(INCLUDE_LIST ${INCLUDE_LIST_BASE} ${ANDROID_TOOL_BASE}/common/hde_armv8)
    set(LIBRARY_LIST ${LIBRARY_LIST_BASE} hdeA64)
elseif(ARCH MATCHES "AMD64" OR ARCH MATCHES "i386" OR ARCH MATCHES "x86_64")
    set(SOURCES_LIST ${SOURCES_LIST_BASE} amd64/opcOp_arch.cpp)
    set(INCLUDE_LIST ${INCLUDE_LIST_BASE} ${ANDROID_TOOL_BASE}/common/hde_x86)
    set(LIBRARY_LIST ${LIBRARY_LIST_BASE} hde_x86)
endif()

# architecture libraries
add_library(${PROJECT_NAME}_x86 SHARED amd64/opcOp_arch.cpp )
target_include_directories(${PROJECT_NAME}_x86 PRIVATE ${INCLUDE_LIST_BASE} ${ANDROID_TOOL_BASE}/common/hde_x86)
target_link_libraries(${PROJECT_NAME}_x86 PRIVATE ${LIBRARY_LIST_BASE} hde_x86)
target_compile_definitions(${PROJECT_NAME}_x86 PRIVATE SUPPORT_X86_64=1)

add_library(${PROJECT_NAME}_A64 SHARED arm64/opcOp_arch.cpp)
target_include_directories(${PROJECT_NAME}_A64 PRIVATE ${INCLUDE_LIST_BASE} ${ANDROID_TOOL_BASE}/common/hde_armv8)
target_link_libraries(${PROJECT_NAME}_A64 PRIVATE ${LIBRARY_LIST_BASE} hdeA64)
target_compile_definitions(${PROJECT_NAME}_A64 PRIVATE SUPPORT_AARCH=1)

add_library(${PROJECT_NAME} SHARED ${SOURCES_LIST_BASE})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST_BASE}
    ${ANDROID_TOOL_BASE}/common/hde_armv8 ${ANDROID_TOOL_BASE}/common/hde_x86)
target_link_libraries(${PROJECT_NAME} ${LIBRARY_LIST_BASE} ${PROJECT_NAME}_x86 ${PROJECT_NAME}_A64)
target_compile_definitions(${PROJECT_NAME} PRIVATE SUPPORT_AARCH64 SUPPORT_X86_64)

# command line utility for regex instructions
add_executable(regexcaller regexcaller.cpp regexparse.cpp)
target_include_directories(regexcaller PRIVATE
    ${INCLUDE_LIST_BASE}
    ${ANDROID_TOOL_BASE}/common/hde_x86
    ${ANDROID_TOOL_BASE}/common/hde_armv8
    
    )
target_link_libraries(regexcaller ${LIBRARY_LIST_BASE} ${PROJECT_NAME}_x86 ${PROJECT_NAME}_A64 ${PROJECT_NAME})
# if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#     find_package(capstone)
#     target_include_directories(${PROJECT_NAME} PRIVATE ${capstone_INCLUDE_DIRS})
# endif()
