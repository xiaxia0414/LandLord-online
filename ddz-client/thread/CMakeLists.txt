cmake_minimum_required(VERSION 3.5)
project(thread)

aux_source_directory(./ SRC)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
add_library(thread STATIC ${SRC} ${HEADERS} ${UIS}
    communication.h communication.cpp
    taskqueue.h taskqueue.cpp)
target_link_libraries(thread PRIVATE Qt${QT_VERSION_MAJOR}::Core
)
