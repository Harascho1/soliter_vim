cmake_minimum_required(VERSION 3.20)

set(ASSETS_PATH ${PROJECT_SOURCE_DIR}/assets)

set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/menu.h
    ${CMAKE_SOURCE_DIR}/src/cursor.h
    ${CMAKE_SOURCE_DIR}/src/font.h
    ${CMAKE_SOURCE_DIR}/src/game.h
    ${CMAKE_SOURCE_DIR}/src/texture.h
    ${CMAKE_SOURCE_DIR}/src/card.h
    ${CMAKE_SOURCE_DIR}/src/my_timer.h
)

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
    ${CMAKE_SOURCE_DIR}/src/my_timer.c
    ${CMAKE_SOURCE_DIR}/src/main.c
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${soliter_src})

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()


set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY            "${CMAKE_SOURCE_DIR}/output"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG      "${CMAKE_SOURCE_DIR}/output"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE    "${CMAKE_SOURCE_DIR}/output")

set(ASSETS_PATH ${CMAKE_SOURCE_DIR}/assets)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)

include_directories(${CMAKE_SOURCE_DIR}/sdl/SDL/include)
include_directories(${CMAKE_SOURCE_DIR}/sdl/SDL_ttf/include)
include_directories(${CMAKE_SOURCE_DIR}/sdl/SDL_image/include)

target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/pch/pch.h)

if (LINUX) 
    SET(SDL_IMAGE_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_image/libSDL3_image.so.0")
    SET(SDL_TTF_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_ttf/libSDL3_ttf.so.0")
    SET(SDL_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL/libSDL3.so.0")
else()
    SET(SDL_IMAGE_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_image/Release/SDL3_image.dll")
    SET(SDL_TTF_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_ttf/Release/SDL3_ttf.dll")
    SET(SDL_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL/Release/SDL3.dll")
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${SDL_SHARED_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMAND ${CMAKE_COMMAND} -E copy "${SDL_IMAGE_SHARED_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMAND ${CMAKE_COMMAND} -E copy "${SDL_TTF_SHARED_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMENT "Copying shared libraries")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_PATH}" "${CMAKE_SOURCE_DIR}/output/assets"
    COMMENT "Copying assets")