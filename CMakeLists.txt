cmake_minimum_required(VERSION 3.20)

project(soliter)
set(CMAKE_C_STANDARD 17)

set(ASSETS_PATH ${PROJECT_SOURCE_DIR}/assets)

set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/menu.h
    ${CMAKE_SOURCE_DIR}/src/cursor.h
    ${CMAKE_SOURCE_DIR}/src/font.h
    ${CMAKE_SOURCE_DIR}/src/game.h
    ${CMAKE_SOURCE_DIR}/src/texture.h
    ${CMAKE_SOURCE_DIR}/src/card.h
)

link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB lib_files "${PROJECT_SOURCE_DIR}/lib/*")

file(GLOB_RECURSE soliter_src 
    ${CMAKE_SOURCE_DIR}/src/font.c
    ${CMAKE_SOURCE_DIR}/src/cursor.c
    ${CMAKE_SOURCE_DIR}/src/texture.c
    ${CMAKE_SOURCE_DIR}/src/main_menu.c
    ${CMAKE_SOURCE_DIR}/src/gameplay.c
    ${CMAKE_SOURCE_DIR}/src/game_over_menu.c
    ${CMAKE_SOURCE_DIR}/src/menu.c
    ${CMAKE_SOURCE_DIR}/src/game.c
    ${CMAKE_SOURCE_DIR}/src/card.c
    ${CMAKE_SOURCE_DIR}/src/main.c
)

add_executable(soliter ${HEADER_FILES} ${soliter_src})

target_precompile_headers(soliter PRIVATE pch/pch.h)

target_include_directories(soliter PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(soliter SDL3 SDL3_image SDL3_ttf)