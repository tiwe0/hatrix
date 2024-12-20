cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_definitions(D_XOPEN_SOURCE_EXTENDED=1)
add_compile_definitions(NCURSES_WIDECHAR=1)
add_compile_definitions(HATRIX_DEBUG=1)

project(Hatrix)

set(
    SOURCES
    src/main.cpp
    src/renderer.cpp
    src/updater.cpp
    src/world.cpp
    src/controller.cpp
    src/gamemap.cpp

    src/ui/editor.cpp

    src/actions/action.cpp
    src/actions/null.cpp
    src/actions/idle.cpp
    src/actions/move.cpp
    src/actions/open.cpp
    src/actions/close.cpp

    src/core/the_core.cpp

    src/entities/entity.cpp
    src/entities/wall.cpp
    src/entities/player.cpp
    src/entities/door.cpp

    src/utils/timer.cpp
    src/utils/pathfinder.cpp
)

add_executable(main ${SOURCES})
 
find_package(Lua REQUIRED)
target_include_directories(main PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(main PRIVATE ${LUA_LIBRARIES})
 
target_include_directories(main PRIVATE ${LUABRIDGE3_INCLUDE_DIRS})

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

target_include_directories(main PRIVATE ${CURSES_INCLUDE_DIRS})
target_compile_options(main PRIVATE ${CURSES_CFLAGS})
target_link_libraries(main PRIVATE ncurses panel)

target_include_directories(main PRIVATE include)