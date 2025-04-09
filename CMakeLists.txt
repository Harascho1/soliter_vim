cmake_minimum_required(VERSION 3.20)

project(soliter)
set(CMAKE_C_STANDARD 17)

add_subdirectory(sdl/SDL)
add_subdirectory(sdl/SDL_ttf)
add_subdirectory(sdl/SDL_image)
add_subdirectory(src)