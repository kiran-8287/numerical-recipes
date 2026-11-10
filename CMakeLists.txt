cmake_minimum_required(VERSION 3.16)

project(numerix VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB_RECURSE NUMERIX_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
list(FILTER NUMERIX_SOURCES EXCLUDE REGEX "/src/cli/main\\.c$")

if(WIN32)
    list(APPEND NUMERIX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/timing.c")
else()
    list(APPEND NUMERIX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/platform/linux/timing.c")
endif()

add_library(numerix STATIC ${NUMERIX_SOURCES})
set_target_properties(numerix PROPERTIES OUTPUT_NAME numerix)
target_include_directories(numerix
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/platform")

if(NOT WIN32 AND NOT MSVC)
    target_compile_definitions(numerix PRIVATE _POSIX_C_SOURCE=200809L)
endif()

if(NOT MSVC)
    target_link_libraries(numerix PUBLIC m)
endif()

add_executable(numerix_cli src/cli/main.c)
target_link_libraries(numerix_cli PRIVATE numerix)

foreach(example demo_root_finding demo_differentiation demo_interpolation)
    add_executable(${example} "${CMAKE_CURRENT_SOURCE_DIR}/examples/${example}.c")
    target_link_libraries(${example} PRIVATE numerix)
endforeach()

include(CTest)
if(BUILD_TESTING)
    file(GLOB NUMERIX_TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.c")
    foreach(test_source IN LISTS NUMERIX_TEST_SOURCES)
        get_filename_component(test_name "${test_source}" NAME_WE)
        if(test_name STREQUAL "test_allocation_failure")
            add_library(numerix_allocfail_interpolation OBJECT
                "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation.c")
            target_include_directories(numerix_allocfail_interpolation PRIVATE
                "${CMAKE_CURRENT_SOURCE_DIR}/include")
            target_compile_definitions(numerix_allocfail_interpolation PRIVATE
                malloc=nr_test_malloc)
            add_executable(${test_name} "${test_source}"
                $<TARGET_OBJECTS:numerix_allocfail_interpolation>)
            if(NOT MSVC)
                target_link_libraries(${test_name} PRIVATE m)
            endif()
        else()
            add_executable(${test_name} "${test_source}")
            target_link_libraries(${test_name} PRIVATE numerix)
        endif()
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
