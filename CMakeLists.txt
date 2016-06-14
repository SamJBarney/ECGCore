cmake_minimum_required(VERSION 3.5)

project(ecg_core CXX)
include_directories(src)

set(SOURCES main.cpp)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h")

add_library(ecg_core STATIC ${SOURCES})
install(TARGETS ecg_core DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/ecg)
set_property(TARGET ecg_core PROPERTY CXX_STANDARD 11)
