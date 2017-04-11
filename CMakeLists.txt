#project(ROIProc)
#cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})


cmake_minimum_required (VERSION 2.8.12)
project(ROIProc)
find_package( OpenCV REQUIRED )
find_package( Boost COMPONENTS system thread filesystem REQUIRED)

###
# make sure we use c++11
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)


INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
file(GLOB SRCS
    "*.h"
    "*.cpp"
    "*.hpp"
)
add_executable(ROIProc main.cpp ${SRCS})
target_link_libraries (ROIProc libCam colorCorrector_lib ${Boost_LIBRARIES} ${OpenCV_LIBS})



#set(COLORCORRECTOR_SRCS
#histequalizer.cpp  lightfieldcorrector.cpp  main.cpp        rgbhsvstretch.cpp
#histeqwidget.cpp   lightfieldwidget.cpp     mainwindow.cpp  rgbhsvwidget.cpp
#histequalizer.h  histeqwidget.h   lightfieldcorrector.h  lightfieldwidget.h
#mainwindow.h     rgbhsvstretch.h  rgbhsvwidget.h
#)


