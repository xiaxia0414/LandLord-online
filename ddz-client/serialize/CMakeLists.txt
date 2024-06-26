cmake_minimum_required(VERSION 3.5)
project(serialize)
include_directories("D:\\Protob\\include")
aux_source_directory(./ SRC)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
file(GLOB PRO ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)
add_library(serialize STATIC ${SRC} ${HEADERS} ${UIS} ${PRO}
    codec.h codec.cpp)
target_link_libraries(serialize PRIVATE Qt${QT_VERSION_MAJOR}::Core protobufd
)
