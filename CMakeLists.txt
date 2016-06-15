cmake_minimum_required(VERSION 3.5)

project(ecg_core CXX)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "default install path" FORCE )
endif()

include_directories(src)

set(SOURCES main.cpp)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h")

add_library(ecg_core STATIC ${SOURCES})
install(TARGETS ecg_core DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/ecg)
set_property(TARGET ecg_core PROPERTY CXX_STANDARD 11)
