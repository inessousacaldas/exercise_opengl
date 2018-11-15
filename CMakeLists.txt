cmake_minimum_required(VERSION 3.4)
project(program)

find_package(OpenGL REQUIRED)

FIND_PACKAGE( Boost REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
	message(STATUS ">>> Build in Linux")
endif()

if (WIN32)
	message(STATUS ">>> Build in Windows")
endif (WIN32)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(LINUX)
	set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
	set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
endif(LINUX)

set(SOURCES src/main.cpp)

###############################
# External libs configuration #
###############################

# GLFW build configuration

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw)

# end


# ------------------------
# GLEW build configuration

add_subdirectory(libs/glew)
include_directories(libs/glew/include)
add_library(glew STATIC libs/glew/src/glew.c)
add_definitions(-DGLEW_STATIC)

# end



# ------------------------
# OpenAL build configuration

add_subdirectory(libs/openal-soft)
include_directories(libs/openal-soft/include)

# end
# ------------------------

# Header-only libraries
include_directories(libs/glm)
include_directories(libs/stb)

# end

include_directories(include)

###############################
#         Libraries           #
###############################

add_library(audio
	src/audio/audio_manager.cpp
)

add_library(game
	src/game/ball.cpp
	src/game/brick.cpp
	src/game/game.cpp
	src/game/game_level.cpp
	src/game/game_object.cpp
	src/game/player.cpp
	src/game/power_up.cpp
	src/game/menu.cpp
	src/game/button.cpp
)

add_library(graphics
	src/graphics/particle_emitter.cpp
	src/graphics/post_processor.cpp
	src/graphics/shader.cpp
	src/graphics/shader_data.cpp
	src/graphics/sprite_renderer.cpp
	src/graphics/text_renderer.cpp
	src/graphics/texture.cpp
)

add_library(input
	src/input/input_manager.cpp
)

add_library(physics
	src/physics/collision_detector.cpp
)

add_library(resources
	src/resources/assets_loader.cpp
	src/resources/resource_manager.cpp
	src/resources/window.cpp
	src/resources/window_manager.cpp
)

add_library(utils
	src/utils/file_manager.cpp
	src/utils/random.cpp
)


add_executable(program ${SOURCES})

if (LINUX)
	SET(EXTRA_LIBS -lGL -lGLU -lglfw )
endif (LINUX)


if (WIN32)
	SET(EXTRA_LIBS )
endif (WIN32)

target_link_libraries(program glfw glew OpenAL ${OPENGL_LIBRARY} ${EXTRA_LIBS})


target_link_libraries(audio
	game
	OpenAL
)

target_link_libraries(program
	game
)

target_link_libraries(game
	audio
	resources
	input
	graphics
	physics
)

target_link_libraries(graphics
	utils
	glfw
	glew
	${EXTRA_LIBS}	
)

target_link_libraries(input
	glfw
	glew
)

target_link_libraries(resources
	glfw
	glew
)
