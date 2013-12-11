cmake_minimum_required(VERSION 2.8)
project (CHARACTER-RECOGNITION)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
file(GLOB lib_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(target characterrecognition)
add_library(${target} ${LIB_TYPE} ${lib_srcs})
set(deps tesseractocr)

#link libraries
target_link_libraries(${target} ${deps})

#add_executable(characterrecognition_example systest/tesseractmain.cpp)
#target_link_libraries(characterrecognition_example ${target})
add_executable(tesseractmain systest/tesseractmain.cpp)
target_link_libraries(tesseractmain ${target})

