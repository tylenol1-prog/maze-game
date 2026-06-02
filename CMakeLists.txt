cmake_minimum_required(VERSION 3.12)
project(MazeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/game.cpp
)

set(HEADERS
    src/game.hpp
)

# Create executable
add_executable(maze_game ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(maze_game ${SDL2_LIBRARIES})

# Compiler flags for better warnings
if(MSVC)
    target_compile_options(maze_game PRIVATE /W4)
else()
    target_compile_options(maze_game PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set output directory
set_target_properties(maze_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
