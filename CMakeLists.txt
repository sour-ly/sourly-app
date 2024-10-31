CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(NAME "sourly")

# set the project name
PROJECT(${NAME})

set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # Add debug symbols and disable optimizations

FILE(GLOB SOURCES "src/*.cpp", "src/*.h", "src/**/*.cpp", "src/**/*.h")

# add the executable
ADD_EXECUTABLE(${NAME} ${SOURCES})

# install script (on make install)
INSTALL(TARGETS ${NAME} DESTINATION bin)


