cmake_minimum_required(VERSION 3.15)
project(common)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./ SRC)
add_library(tcp STATIC ${SRC})