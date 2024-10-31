CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(NAME "sourly")
# set the project name
PROJECT(${NAME})



set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # Add debug symbols and disable optimizations

FILE(GLOB SOURCES "src/*.cpp", "src/*.h", "src/*.hpp" "src/**/*.cpp", "src/**/*.h", "src/**/*.hpp")

# add the executable
ADD_EXECUTABLE(${NAME} ${SOURCES})


#include JSON
set(JSON_INCLUDE_PATH "src/lib/json/include/")
include_directories(${JSON_INCLUDE_PATH})

# SDL2 please check if this is a linux or mac machine
if (APPLE OR UNIX)
		find_package(SDL2 REQUIRED)
		target_include_directories(${NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
# Find SDL2_ttf (adjust the path to your specific SDL2_ttf install location)
		set(SDL2_TTF_PATH /opt/homebrew/opt/sdl2_ttf)  # Adjust this path if needed
		find_library(SDL2_TTF_LIB SDL2_ttf PATHS ${SDL2_TTF_PATH}/lib)
		include_directories(${SDL2_TTF_PATH}/include)

		target_link_libraries(${NAME} ${SDL2_LIBRARIES} ${SDL2_TTF_LIB})
endif()

# install script (on make install)
INSTALL(TARGETS ${NAME} DESTINATION bin)


