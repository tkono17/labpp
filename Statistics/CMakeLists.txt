cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME Statistics)

project(${PROJECT_NAME})

set(DESTINATION $ENV{SWDIR})
set(CMAKE_INSTALL_PREFIX ${DESTINATION})

install(DIRECTORY python/ppstat DESTINATION python FILES_MATCHING PATTERN "*.py"
)
install(PROGRAMS python/bayesMain.py DESTINATION bin)

