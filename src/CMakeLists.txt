cmake_minimum_required(VERSION 3.20)

set(ASSETS_PATH ${PROJECT_SOURCE_DIR}/assets)

set(HEADER_FILES
    card.h
    clickable_element.h
    res/config.h
    cursor.h
    res/font.h
    game.h
    menu.h
    my_timer.h
    render.h
    res/sound.h
    stack.h
    res/textbox.h
    res/texture.h
    scenes/game_over_menu.h
    scenes/gameplay.h
    scenes/macro_settings.h
    scenes/main_menu.h
    scenes/option_settings.h
    scenes/settings.h
    res/res.h
    scenes/render_helper.h
)

set(SOURCE_FILES
    card.c
    clickable_element.c
    res/config.c
    cursor.c
    res/load_field.c
    res/font.c
    game.c
    main.c
    menu.c
    my_timer.c
    render.c
    res/sound.c
    res/textbox.c
    res/texture.c
    scenes/game_over_menu.c
    scenes/gameplay.c
    scenes/gameplay_event.c
    scenes/gameplay_render.c
    scenes/macro_settings.c
    scenes/main_menu.c
    scenes/option_settings.c
    scenes/scores.c
    scenes/settings.c
    res/res.c
    scenes/render_helper.c
)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES} ${ASSETS_PATH}/icon.rc)
else ()
    add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
endif ()

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif ()


# EXPORT_BUILD=ON  -> distributable build into <repo>/output
# EXPORT_BUILD=OFF -> normal dev build, binary stays in the CMake build dir
#                     (cmake-build-debug/, cmake-build-release/, build/, ...)
option(EXPORT_BUILD "Build the distributable release into output/" OFF)

if (EXPORT_BUILD)
    set(RUNTIME_DIR "${CMAKE_SOURCE_DIR}/output")
else ()
    set(RUNTIME_DIR "${CMAKE_BINARY_DIR}")
endif ()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${RUNTIME_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RUNTIME_DIR}")

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
