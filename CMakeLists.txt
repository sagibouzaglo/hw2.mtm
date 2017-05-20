cmake_minimum_required(VERSION 3.7)
project(hw2.mtm)

set(CMAKE_C_STANDARD 99)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic-errors -Werror -DNDEBUG")
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(SOURCE_FILES challenge.c challenge.h challenge_room_system_fields.h challenge_system.c challenge_system.h challenge_system_banch_tests.c
        constants.h visitor_room.c visitor_room.h system_additional_types.h )

add_executable(hw2.mtm ${SOURCE_FILES})

