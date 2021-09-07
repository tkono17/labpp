cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME wsim)

project(${PROJECT_NAME})

set(DESTINATION $ENV{SWDIR})
set(CMAKE_INSTALL_PREFIX ${DESTINATION})

install(DIRECTORY python/wsim DESTINATION python FILES_MATCHING PATTERN "*.py"
)
install(PROGRAMS scripts/singleSlit.py DESTINATION bin)
install(PROGRAMS scripts/doubleSlit.py DESTINATION bin)
install(PROGRAMS scripts/plotInterference.py DESTINATION bin)

