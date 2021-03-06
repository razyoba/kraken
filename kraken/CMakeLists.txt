project(kraken_console)
cmake_minimum_required(VERSION 2.8.9)
aux_source_directory(. SRC_LIST)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release mode")
else()
    message(STATUS "Debug mode")
endif()

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

# Forces BUILD_SHARED_LIBS to OFF as Google Mock currently does not support
# working in a DLL.
set(BUILD_SHARED_LIBS OFF)

########################################################################
#
# This has to be before config_compiler_and_linker() to avoid huge file size due to static linking.
# Small size is required so that it is easier to check decompiling results.
add_subdirectory("simple_exe")

########################################################################
#
# Config compiler

include(cmake/internal_utils.cmake)

# Although Google Test's CMakeLists.txt calls this function, the
# changes there don't affect the current scope.  Therefore we have to
# call it again here.
config_compiler_and_linker()  # from ${gtest_dir}/cmake/internal_utils.cmake

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()

########################################################################
#
# Paths to external projects

set(SOLUTION_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# A directory to find Google Test sources.
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gmock/CMakeLists.txt")
  set(gmock_dir gmock)
else()
  set(gmock_dir "${CMAKE_CURRENT_SOURCE_DIR}/../gmock")
endif()

if(NOT EXISTS "${gmock_dir}/CMakeLists.txt")
    message(SEND_ERROR "gmock is necessary")
    return()
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/beaengine/CMakeLists.txt")
  set(beaengine_dir beaengine)
else()
  set(beaengine_dir "${CMAKE_CURRENT_SOURCE_DIR}/../beaengine")
endif()

if(NOT EXISTS "${beaengine_dir}/CMakeLists.txt")
    message(SEND_ERROR "beaengine is necessary")
    return()
endif()

#=======================================================================
# External libraries
#=======================================================================

#######################################
# Boost
add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    ON)
find_package(Boost REQUIRED COMPONENTS program_options system filesystem thread log log_setup)

IF( NOT Boost_LOG_FOUND)
    message(SEND_ERROR "Boost log is required.")
ENDIF()

include_directories(${Boost_INCLUDE_DIRS})

#######################################
# BeaEngine
add_definitions( -DBEA_ENGINE_STATIC )
add_definitions( -DBEA_USE_STDCALL )

include_directories("${beaengine_dir}/include")

if(NOT EXISTS "${beaengine_dir}/CMakeLists_origin")
    file(COPY "${beaengine_dir}/CMakeLists.txt" DESTINATION "${beaengine_dir}/CMakeLists_origin")
    file(COPY "cmake/beaengine/CMakeLists.txt" DESTINATION "${beaengine_dir}")
endif()
set(optBUILD_STDCALL ON)
add_subdirectory("${beaengine_dir}" "${QtTest_BINARY_DIR}/beaengine" EXCLUDE_FROM_ALL)

#######################################
# kraken project
include_directories("kraken/include")
add_subdirectory(kraken)


#######################################
# Main project

file(GLOB HEADERS_LIST *.h)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS_LIST})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} BeaEngine)
target_link_libraries(${PROJECT_NAME} kraken)

set(PIN_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../pin-windows")
set(PIN_BIN "${PIN_BIN_DIR}/pin.exe")

if(NOT EXISTS "${PIN_BIN}")
    message(SEND_ERROR "Please, place pin tool in ${PIN_BIN_DIR}")
    return()
endif()

set(PIN_TRACE_BIN_TOOL_SOLUTION "${CMAKE_CURRENT_SOURCE_DIR}/pin-trace-bin")
set(PIN_TRACE_BIN_TOOL "${PIN_TRACE_BIN_TOOL_SOLUTION}/bin-x86/TraceBin.dll")

if(NOT EXISTS "${PIN_TRACE_BIN_TOOL}")
    message(SEND_ERROR "Please, build pin tool instr address tool: ${PIN_TRACE_BIN_TOOL_SOLUTION}")
    return()
endif()

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E
                    copy "${PIN_BIN}"
                         "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E
                    copy "${PIN_TRACE_BIN_TOOL}"
                         "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

#######################################
# Kraken gui
add_subdirectory(kraken_gui)

#######################################
# Krakens' tests
add_subdirectory(test)
