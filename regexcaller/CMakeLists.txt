set(TARGET_INCLUDES
    # this is just the simple stuff that the parent has, including
    # local_util and bgrep_c
    ${INCLUDE_LIST_BASE}
    ${ANDROID_TOOL_BASE}/common/hde_x86
    ${ANDROID_TOOL_BASE}/common/hde_armv8
    # for now this is because arm64 and amd64 are relative to 
    # the home directory
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set(TARGET_LINKDIRS )

set(TARGET_LIBS
    localUtil hdeA64 hde_x86 bgrep_e
    ${PROJECT_NAME}_A64 ${PROJECT_NAME}_x86
    )

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(getopt_for_windows)
    list(APPEND TARGET_INCLUDES ${getopt_for_windows_INCLUDE_DIR})
    list(APPEND TARGET_LINKDIRS ${getopt_for_windows_LIB_DIR})
    list(APPEND TARGET_LIBS getopt_for_windows_static)
endif()

# command line utility for regex instructions
add_executable(regexcaller regexcaller.cpp regexparse.cpp)

target_compile_definitions(regexcaller PRIVATE SUPPORT_X86_64=1 SUPPORT_AARCH64=1)

target_include_directories(regexcaller PRIVATE ${TARGET_INCLUDES})

target_link_directories(regexcaller PRIVATE ${TARGET_LINKDIRS})

target_link_libraries(regexcaller PRIVATE ${TARGET_LIBS})

# if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#     find_package(capstone)
#     target_include_directories(${PROJECT_NAME} PRIVATE ${capstone_INCLUDE_DIRS})
# endif()
