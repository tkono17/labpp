cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME ExPythia)

project(${PROJECT_NAME})
set(${PROJECT_NAME} VERSION MAJOR 1)
set(${PROJECT_NAME} VERSION MINOR 0)
set(DESTINATION $ENV{SWDIR})

set(CMAKE_INSTALL_PREFIX ${DESTINATION})

# set(CMAKE_MODULE_PATH /nfs/space1/tkohno/sw/etc/cmake ${CMAKE_MODULE_PATH})

configure_file(
  "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.h.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.h"
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(../McData)

file(GLOB LIB_SRCS src/[a-zA-Z]*.cxx)
file(GLOB EXE_SRCS src/main/*.cxx)
# file(GLOB DICT_HEADERS ${PROJECT_NAME}/[a-zA-Z].hxx)
set(DICT_HEADERS )

#####-------------------------------------------------------------
##### Package dependencies
#####-------------------------------------------------------------
# ROOT stuffs
set(ROOT_COMPONENTS_TO_USE Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)
find_package(ROOT 
  COMPONENTS ${ROOT_COMPONENTS_TO_USE} REQUIRED)
include(${ROOT_USE_FILE})
list(APPEND LIBS ${ROOT_LIBRARIES})

include_directories(${DESTINATION}/include 
  ${PROJECT_SOURCE_DIR}/McData
  ${DESTINATION}/MG5_aMC_v3_5_1/ExRootAnalysis )
link_directories($ENV{SWDIR}/lib)
list(APPEND LIBS 
  ${DESTINATION}/lib/libfastjet.so
  ${DESTINATION}/lib/libfastjetcontribfragile.so
  ${DESTINATION}/lib/libpythia8.so
  ${DESTINATION}/MG5_aMC_v3_5_1/ExRootAnalysis/libExRootAnalysis.so
  ${PROJECT_BINARY_DIR}/../McData/libMcData.so
  libHepMC3.so
  )

#####-------------------------------------------------------------
##### ROOT dictionary
#####-------------------------------------------------------------
if (DICT_HEADERS)
  include_directories(${PROJECT_SOURCE_DIR}/${PROJECT_NAME})
  ROOT_GENERATE_DICTIONARY(${PROJECT_NAME}Dict 
    ${DICT_HEADERS}
    LINKDEF ${PROJECT_NAME}/${PROJECT_NAME}LinkDef.hxx
  )
  set(root_dict_src ${PROJECT_NAME}Dict)
else()
  set(root_dict_src )
endif()

#####-------------------------------------------------------------
##### Shared library
#####-------------------------------------------------------------
add_library(${PROJECT_NAME} SHARED 
  ${LIB_SRCS} ${root_dict_src}
  )
target_link_libraries(${PROJECT_NAME} ${LIBS})

#####-------------------------------------------------------------
##### Executables
#####-------------------------------------------------------------
set(EXE_SRCS src/main/runPythiaJet.cxx)
foreach(progsrc ${EXE_SRCS})
  string(REGEX REPLACE ".*src/main/(.*).cxx" "\\1.exe" progexe ${progsrc})
  list(APPEND EXES ${progexe})
  add_executable(${progexe} ${progsrc})
  target_link_libraries(${progexe} ${PROJECT_NAME} ${LIBS})
endforeach()

#####-------------------------------------------------------------
##### Installation
#####-------------------------------------------------------------
install(DIRECTORY ${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.hxx"
  )
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
foreach(progexe ${EXES})
  install(TARGETS ${progexe} DESTINATION bin)
endforeach()

install(PROGRAMS scripts/run_mgpj.sh DESTINATION bin)
#install(FILES share/${PROJECT_NAME}Config.cmake DESTINATION share/cmake)

