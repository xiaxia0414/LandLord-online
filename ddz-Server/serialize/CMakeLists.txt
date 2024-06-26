cmake_minimum_required(VERSION 3.15)
project(serialize)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./ SRC)
add_library(serialize STATIC ${SRC})