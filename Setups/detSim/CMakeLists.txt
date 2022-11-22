cmake_minimum_required(VERSION 3.10)
project(DetSim)
set(DetSim VERSION MAJOR 1)
set(DetSim VERSION MINOR 0)
set(DESTINATION $ENV{SWDIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX $ENV{SWDIR})

add_subdirectory(../../DetSim/SimData SimData)
add_subdirectory(../../DetSim/G4Example1 G4Example1)
