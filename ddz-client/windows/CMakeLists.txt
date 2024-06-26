cmake_minimum_required(VERSION 3.5)
project(windows)

aux_source_directory(./ SRC)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
add_library(windows STATIC ${SRC} ${HEADERS} ${UIS}
    login.h login.cpp login.ui
    gamemode.h gamemode.cpp gamemode.ui
    joinroom.h joinroom.cpp joinroom.ui)
target_link_libraries(windows PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)
