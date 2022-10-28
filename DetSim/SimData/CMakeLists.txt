cmake_minimum_required(VERSION 3.10)
project(SimData)
set(SimData VERSION MAJOR 1)
set(SimData VERSION MINOR 0)
set(DESTINATION $ENV{SWDIR})
set(CMAKE_INSTALL_PREFIX $ENV{SWDIR})

#set(CMAKE_MODULE_PATH /nfs/space1/tkohno/sw/etc/cmake ${CMAKE_MODULE_PATH})
#set(CMAKE_MODULE_PATH $ENV{SWDIR}/lib64/Geant4-9.6.4 ${CMAKE_MODULE_PATH})

configure_file(
  "${PROJECT_SOURCE_DIR}/SimData/SimDataConfig.h.in"
  "${PROJECT_BINARY_DIR}/SimData/SimDataConfig.h"
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

# ROOT stuffs
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
set(LIBS ${LIBS} ${ROOT_LIBRARIES})

file(GLOB LIB_SRCS src/[a-zA-Z]*.cxx)
file(GLOB DICT_HEADERS SimData/[a-zA-Z].hxx)
file(GLOB EXE_SRCS src/main/*.cxx)
set(DICT_HEADERS SimParticle.hxx SimVertex.hxx SimEvent.hxx)

include($ENV{SWDIR}/share/cmake/TkMacros_common.cmake)

#add_library(SimDataDict SHARED
#  ${CMAKE_CURRENT_BINARY_DIR}/SimDataDict.cxx
#  )

add_library(SimData SHARED 
  ${LIB_SRCS} ${root_dict_src}
  )
#target_link_libraries(SimData SimDataDict ${LIBS})

add_executable(test_simdata1.exe src/main/test_simdata1.cxx)
target_link_libraries(test_simdata1.exe SimData ${LIBS})

install(DIRECTORY SimData
  DESTINATION include
  FILES_MATCHING PATTERN "*.hxx"
  )

install(TARGETS SimData DESTINATION lib)
install(FILES ${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}Dict_rdict.pcm
  ${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}Dict.rootmap
  DESTINATION lib)

#install(TARGETS SimDataDict DESTINATION lib)
install(TARGETS test_simdata1.exe DESTINATION bin)
#install(FILES SimDataConfig.cmake DESTINATION share/cmake)
