cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Hatrix)

set(
    SOURCES
    src/main.cpp
    src/renderer.cpp
    src/updater.cpp
    src/world.cpp
    src/controller.cpp

    src/actions/action.cpp
    src/actions/null.cpp
    src/actions/idle.cpp
    src/actions/move.cpp

    src/entities/entity.cpp

    src/utils/timer.cpp
)

add_executable(main ${SOURCES})

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

target_include_directories(main PRIVATE include)

target_include_directories(main PRIVATE ${CURSES_INCLUDE_DIRS})
target_compile_options(main PRIVATE ${CURSES_CFLAGS})
target_link_libraries(main PRIVATE ${CURSES_LIBRARIES})

