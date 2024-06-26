cmake_minimum_required(VERSION 3.15)
project(game)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./ SRC)
add_library(game STATIC ${SRC}
        Room.h
        Room.cpp
        RoomList.cpp
        RoomList.h
)
target_link_libraries(game mycrypto common hiredis redis++)