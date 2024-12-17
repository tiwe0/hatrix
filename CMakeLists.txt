cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
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

    src/core/the_core.cpp

    src/entities/entity.cpp
    src/entities/wall.cpp
    src/entities/player.cpp

    src/utils/timer.cpp
)

add_executable(main ${SOURCES})

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

target_include_directories(main PRIVATE include)

find_package(Lua REQUIRED)
target_include_directories(main PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(main PRIVATE ${LUA_LIBRARIES})

target_include_directories(main PRIVATE vcpkg_installed/arm64-osx/include)

target_include_directories(main PRIVATE ${CURSES_INCLUDE_DIRS})
target_compile_options(main PRIVATE ${CURSES_CFLAGS})
target_link_libraries(main PRIVATE ${CURSES_LIBRARIES})
target_link_libraries(main PRIVATE panel)

