cmake_minimum_required(VERSION 3.15)
project(ddz_Server)
set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/http
        ${PROJECT_SOURCE_DIR}/reactor
        ${PROJECT_SOURCE_DIR}/tcp
        ${PROJECT_SOURCE_DIR}/thread
        ${PROJECT_SOURCE_DIR}/test
        "/home/xiaxia0414/openssl/include"
        ${PROJECT_SOURCE_DIR}/crypto
        ${PROJECT_SOURCE_DIR}/serialize
        ${PROJECT_SOURCE_DIR}/game
        ${PROJECT_SOURCE_DIR}/database
        "/usr/include/mysql"
        "/usr/local/include/json"
        "/usr/local/include/sw"
)

link_directories(/usr/lib/x86_64-linux-gnu/)
link_directories(/home/xiaxia0414/openssl/)
link_directories(/usr/local/lib/)

#添加子目录
add_subdirectory(common)
add_subdirectory(http)
add_subdirectory(reactor)
add_subdirectory(tcp)
add_subdirectory(thread)
add_subdirectory(serialize)
add_subdirectory(crypto)
add_subdirectory(game)
add_subdirectory(database)
#测试用子目录
add_subdirectory(test)


link_libraries( tcp thread http  reactor common test serialize mycrypto game database)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(ddz_Server main.cpp

)
