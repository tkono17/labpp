cmake_minimum_required(VERSION 3.10)
project(G4Example1)
set(G4Example1 VERSION MAJOR 1)
set(G4Example1 VERSION MINOR 0)
set(DESTINATION $ENV{SWDIR})
set(CMAKE_INSTALL_PREFIX $ENV{SWDIR})

#set(CMAKE_MODULE_PATH /nfs/space1/tkohno/sw/etc/cmake ${CMAKE_MODULE_PATH})
#set(CMAKE_MODULE_PATH $ENV{SWDIR}/lib64/Geant4-9.6.4 ${CMAKE_MODULE_PATH})
#set(CMAKE_MODULE_PATH $ENV{SWDIR}/share/cmake ${CMAKE_MODULE_PATH})

set(CMAKE_PREFIX_PATH $ENV{SWDIR}/share/cmake ${CMAKE_PREFIX_PATH})

configure_file(
  "${PROJECT_SOURCE_DIR}/G4Example1/G4Example1Config.h.in"
  "${PROJECT_BINARY_DIR}/G4Example1/G4Example1Config.h"
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

include_directories(../SimData)
set(LIBS ${LIBS} SimData)

include_directories($ENV{SWDIR}/include)

# ROOT stuffs
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
set(LIBS ${LIBS} ${ROOT_LIBRARIES})

# Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
set(LIBS ${LIBS} ${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

file(GLOB LIB_SRCS src/[a-zA-Z]*.cxx)
#file(GLOB DICT_HEADERS G4Example1/[a-zA-Z].hxx)
file(GLOB EXE_SRCS src/main/*.cxx)

message(ROOT_USE_FILE=${ROOT_USE_FILE})

add_library(G4Example1 SHARED 
  ${LIB_SRCS}
  )

add_executable(g4ex1.exe src/main/g4ex1.cxx)

target_link_libraries(g4ex1.exe G4Example1 ${LIBS})

install(FILES 
  G4Example1/ExG4DetectorConstruction01.hxx
  DESTINATION include/G4Example1
  )

install(TARGETS G4Example1 DESTINATION lib)
install(TARGETS g4ex1.exe DESTINATION bin)
