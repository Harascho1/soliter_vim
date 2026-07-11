cmake_minimum_required(VERSION 3.20)

set(ASSETS_PATH ${PROJECT_SOURCE_DIR}/assets)

set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/textbox.h
    ${CMAKE_SOURCE_DIR}/src/sound.h
    ${CMAKE_SOURCE_DIR}/src/field.h
    ${CMAKE_SOURCE_DIR}/src/menu.h
    ${CMAKE_SOURCE_DIR}/src/settings.h
    ${CMAKE_SOURCE_DIR}/src/option_settings.h
    ${CMAKE_SOURCE_DIR}/src/macro_settings.h
    ${CMAKE_SOURCE_DIR}/src/config.h
    ${CMAKE_SOURCE_DIR}/src/cursor.h
    ${CMAKE_SOURCE_DIR}/src/font.h
    ${CMAKE_SOURCE_DIR}/src/game.h
    ${CMAKE_SOURCE_DIR}/src/texture.h
    ${CMAKE_SOURCE_DIR}/src/card.h
    ${CMAKE_SOURCE_DIR}/src/my_timer.h
    ${CMAKE_SOURCE_DIR}/src/render.h
        stack.h
)

file(GLOB_RECURSE soliter_src 
    ${CMAKE_SOURCE_DIR}/src/textbox.c
    ${CMAKE_SOURCE_DIR}/src/sound.c
    ${CMAKE_SOURCE_DIR}/src/field.c
    ${CMAKE_SOURCE_DIR}/src/render.c
    ${CMAKE_SOURCE_DIR}/src/option_settings.c
    ${CMAKE_SOURCE_DIR}/src/macro_settings.c
    ${CMAKE_SOURCE_DIR}/src/config.c
    ${CMAKE_SOURCE_DIR}/src/font.c
    ${CMAKE_SOURCE_DIR}/src/cursor.c
    ${CMAKE_SOURCE_DIR}/src/texture.c
    ${CMAKE_SOURCE_DIR}/src/main_menu.c
    ${CMAKE_SOURCE_DIR}/src/scores.c
    ${CMAKE_SOURCE_DIR}/src/gameplay_event.c
    ${CMAKE_SOURCE_DIR}/src/gameplay_render.c
    ${CMAKE_SOURCE_DIR}/src/gameplay.c
    ${CMAKE_SOURCE_DIR}/src/settings.c
    ${CMAKE_SOURCE_DIR}/src/game_over_menu.c
    ${CMAKE_SOURCE_DIR}/src/menu.c
    ${CMAKE_SOURCE_DIR}/src/game.c
    ${CMAKE_SOURCE_DIR}/src/card.c
    ${CMAKE_SOURCE_DIR}/src/my_timer.c
    ${CMAKE_SOURCE_DIR}/src/main.c
)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${soliter_src} ${ASSETS_PATH}/icon.rc)
else()
    add_executable(${PROJECT_NAME} ${HEADER_FILES} ${soliter_src})
endif()

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()


# EXPORT_BUILD=ON  -> distributable build into <repo>/output
# EXPORT_BUILD=OFF -> normal dev build, binary stays in the CMake build dir
#                     (cmake-build-debug/, cmake-build-release/, build/, ...)
option(EXPORT_BUILD "Build the distributable release into output/" OFF)

if(EXPORT_BUILD)
    set(RUNTIME_DIR "${CMAKE_SOURCE_DIR}/output")
else()
    set(RUNTIME_DIR "${CMAKE_BINARY_DIR}")
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY            "${RUNTIME_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG      "${RUNTIME_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE    "${RUNTIME_DIR}")

set(ASSETS_PATH ${CMAKE_SOURCE_DIR}/assets)

find_package(Threads REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)

target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/pch/pch.h)

# Copy assets next to the actual binary, wherever it lands, so relative
# asset paths work both in dev build dirs and in the exported output/.
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_PATH}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets")
