cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME trackVertex)

project(${PROJECT_NAME})

set(DESTINATION $ENV{SWDIR})
set(CMAKE_INSTALL_PREFIX ${DESTINATION})

add_subdirectory(../../Patterns/TrackVertex TrackVertex)

