cmake_minimum_required(VERSION 2.8)

#cmake_policy(SET CMP0015 NEW)
project(TESSERACT-OCR)

# compile code , which is position independent 
if (CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#  set(CMAKE_LD_FLAGS "-lpthread")
endif ()


# USE_STD_NAMESPACE
option(USE_STD_NAMESPACE "use std namespace" ON)
if (USE_STD_NAMESPACE)
  add_definitions(-DUSE_STD_NAMESPACE)
endif()

# tesserocr include
include_directories(./ccmain/)
include_directories(./ccstruct/)
include_directories(./ccutil/)
include_directories(./classify/)
include_directories(./cube/)
include_directories(./cutil/)
include_directories(./dict/)
include_directories(./image/)
include_directories(./neural_networks/runtime/)
include_directories(./viewer/)
include_directories(./wordrec/)
include_directories(./textord/)
# leptonic include
include_directories(/usr/local/include/leptonica/)
include_directories(/usr/include/)

link_directories(/usr/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


#find_library(mLib m) # libm.so
find_library(leptLib lept)  #liblept.so
find_library(pthreadLib pthread) #libpthread.so

set(targetLib tesseractocr)
file (GLOB lib_srcs ./ccmain/*.cpp ./ccstruct/*.cpp ./ccutil/*.cpp ./classify/*.cpp ./cube/*.cpp ./cutil/*.cpp ./dict/*.cpp ./image/*.cpp ./neural_networks/runtime/*.cpp ./viewer/*.cpp ./wordrec/*.cpp ./textord/*.cpp)
add_library(${targetLib} ${lib_srcs}) #to generate static lib libtesseractocr.a
target_link_libraries(${targetLib} ${leptLib} ${pthreadLib})

################### Apps ########################
add_subdirectory(api)
