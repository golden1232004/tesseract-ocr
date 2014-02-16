cmake_minimum_required(VERSION 2.8)

#cmake_policy(SET CMP0015 NEW)
project(TESSERACT-OCR)

# compile code , which is position independent 
if (CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_GNUCXX)
 # add_definitions(-Wall -Wunused-function)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  set(CMAKE_C_FLAGs "${CMAKE_C_FLAGS} -fPIC")
  if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    string(REGEX REPLACE "(-g |-g$)" "-g3 " CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REGEX REPLACE "(-g |-g$)" "-g3 " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  endif ()
endif ()

# set output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# USE_STD_NAMESPACE
option(USE_STD_NAMESPACE "use std namespace" ON)
if (USE_STD_NAMESPACE)
  add_definitions(-DUSE_STD_NAMESPACE)
endif()

# use 3rd part library
option(USE_LIB_LEPTONIC "enable leptonic lib" ON)
option(USE_LIB_OPENCV   "enable opencv lib" OFF)
if (USE_LIB_LEPTONIC)
  add_definitions(-DUSE_LIB_LEPTONIC)
endif ()
if (USE_LIB_OPENCV)
  add_definitions(-DUSE_LIB_OPENCV)
endif ()
##----------------------------------------------------------------
option(ENABLE_SAVE_TEMPORARY_IMAGE "to save temporary image" OFF)
if (ENABLE_SAVE_TEMPORARY_IMAGE)
  add_definitions(-DENABLE_SAVE_TEMPORARY_IMAGE)
endif ()
option(ENABLE_SHOW_TEMPORARY_IMAGE "to show temporary image" OFF)
if (ENABLE_SHOW_TEMPORARY_IMAGE)
  add_definitions(-DENABLE_SHOW_TEMPORARY_IMAGE)
endif()
##----------------------------------------------------------------

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
if (USE_LIB_LEPTONIC)
  include_directories(/usr/local/include/leptonica/)
  find_library(leptLib lept)  #liblept.so
else ()

endif ()

include_directories(/usr/include/)
find_library(pthreadLib pthread) #libpthread.so
if (NOT CMAKE_CROSSCOMPILING)
 # message(${OpenCV_INCLUDE_DIRS})
  include_directories(/usr/local/include)
  find_package(OpenCV REQUIRED)
endif ()

set(targetLib tesseractocr)
file (GLOB lib_srcs ./ccmain/*.cpp ./ccstruct/*.cpp ./ccutil/*.cpp ./classify/*.cpp ./cube/*.cpp ./cutil/*.cpp ./dict/*.cpp ./image/*.cpp ./image/*.c ./neural_networks/runtime/*.cpp ./viewer/*.cpp ./wordrec/*.cpp ./textord/*.cpp)
###
# ToDo: I do not know that static lib will lead to compile error, which is "recompile with -fPIC".
###
add_library(${targetLib} ${lib_srcs}) #to generate static lib libtesseractocr.a
if (NOT CMAKE_CROSSCOMPILING)
  target_link_libraries(${targetLib} ${leptLib} ${pthreadLib})
else ()
  target_link_libraries(${targetLib} ${pthreadLib})
endif ()

################### Apps ########################
add_subdirectory(api)
