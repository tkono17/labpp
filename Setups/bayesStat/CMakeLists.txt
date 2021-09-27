cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME bayseStat)

project(${PROJECT_NAME})

set(DESTINATION $ENV{SWDIR})
set(CMAKE_INSTALL_PREFIX ${DESTINATION})

add_subdirectory(../../Waves Waves)
add_subdirectory(../../Statistics/Bayes Bayes)

