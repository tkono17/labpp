cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME McData)

project(${PROJECT_NAME})
set(${PROJECT_NAME} VERSION MAJOR 1)
set(${PROJECT_NAME} VERSION MINOR 0)
set(DESTINATION $ENV{SWDIR})

set(CMAKE_INSTALL_PREFIX ${DESTINATION})

configure_file(
  "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.h.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.h"
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

file(GLOB LIB_SRCS src/[a-zA-Z]*.cxx)
file(GLOB EXE_SRCS src/main/*.cxx)
# file(GLOB DICT_HEADERS ${PROJECT_NAME}/[a-zA-Z].hxx)
set(DICT_HEADERS 
  McData/McEvent.hxx
  McData/McParticle.hxx
  McData/McVertex.hxx
  McData/Jet.hxx
  McData/LargeRJet.hxx
  McData/IndexPair.hxx
  McData/PJEvent.hxx
  )

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
  ${DESTINATION}/MG5_aMC_v3_5_1/ExRootAnalysis)
list(APPEND LIBS 
  ${DESTINATION}/lib/libfastjet.so
  ${DESTINATION}/lib/libfastjetcontribfragile.so
  ${DESTINATION}/lib/libpythia8.so
  ${DESTINATION}/MG5_aMC_v3_5_1/ExRootAnalysis/libExRootAnalysis.so
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
target_link_libraries(${PROJECT_NAME} ${LIBS}
    ${DESTINATION}/lib/libEnergyCorrelator.a
)

#####-------------------------------------------------------------
##### Executables
#####-------------------------------------------------------------
set(EXES )
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

if (${ROOT_VERSION} VERSION_GREATER "6.0")
   install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/libMcDataDict_rdict.pcm
      ${CMAKE_CURRENT_BINARY_DIR}/libMcDataDict.rootmap
      DESTINATION lib)
endif (${ROOT_VERSION} VERSION_GREATER "6.0")

#install(FILES share/${PROJECT_NAME}Config.cmake DESTINATION share/cmake)

