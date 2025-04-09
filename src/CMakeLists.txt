cmake_minimum_required(VERSION 3.20)

set(ASSETS_PATH ${PROJECT_SOURCE_DIR}/assets)

set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/menu.h
    ${CMAKE_SOURCE_DIR}/cursor.h
    ${CMAKE_SOURCE_DIR}/font.h
    ${CMAKE_SOURCE_DIR}/game.h
    ${CMAKE_SOURCE_DIR}/texture.h
    ${CMAKE_SOURCE_DIR}/card.h
)

file(GLOB_RECURSE soliter_src 
    ${CMAKE_SOURCE_DIR}/font.c
    ${CMAKE_SOURCE_DIR}/cursor.c
    ${CMAKE_SOURCE_DIR}/texture.c
    ${CMAKE_SOURCE_DIR}/main_menu.c
    ${CMAKE_SOURCE_DIR}/gameplay.c
    ${CMAKE_SOURCE_DIR}/game_over_menu.c
    ${CMAKE_SOURCE_DIR}/menu.c
    ${CMAKE_SOURCE_DIR}/game.c
    ${CMAKE_SOURCE_DIR}/card.c
    ${CMAKE_SOURCE_DIR}/main.c
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${soliter_src})

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3_ttf SDL3_image SDL3)
