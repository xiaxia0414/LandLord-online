cmake_minimum_required(VERSION 3.5)
project(crypto)

aux_source_directory(./ SRC)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
add_library(crypto STATIC ${SRC} ${HEADERS} ${UIS}
    aescrypto.h aescrypto.cpp
    rsacrypto.h rsacrypto.cpp)
target_link_libraries(crypto PRIVATE Qt${QT_VERSION_MAJOR}::Core
    "D:\\Program Files\\OpenSSL-Win64\\lib\\VC\\x64\\MD\\libcrypto.lib"
)
