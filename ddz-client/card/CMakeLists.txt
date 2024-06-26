cmake_minimum_required(VERSION 3.5)
project(card)
aux_source_directory(./ SRC)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
add_library(card STATIC ${SRC} ${HEADERS} ${UIS})
target_link_libraries(card PRIVATE Qt${QT_VERSION_MAJOR}::Core
)
