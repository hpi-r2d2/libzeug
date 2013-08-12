# Target
set(target signalzeug)
message(STATUS "lib ${target}")

# Find externals

# Include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/../../  # for Meta.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/signalzeug)

# Libraries
set(libs
)

# Definitions
add_definitions("-DSIGNALZEUG_EXPORTS")

# Sources
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/../../include/signalzeug")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(headers
    ${header_path}/signalzeug.h
    ${header_path}/AbstractSignal.h
    ${header_path}/Signal.hpp
)

set(sources
    ${source_path}/AbstractSignal.cpp
)

# Group source files
set(signalzeug_header_group "Header Files (API)")
set(signalzeug_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${signalzeug_header_group} ${headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${signalzeug_source_group} ${sources})

# Build library
if(OPTION_BUILD_STATIC)
    add_library(${target} ${headers} ${sources})
else()
    add_library(${target} SHARED ${headers} ${sources})
endif()
target_link_libraries(${target} ${libs})
set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH                "${CMAKE_CURRENT_SOURCE_DIR}/../../include")

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ DESTINATION ${INSTALL_INCLUDE})