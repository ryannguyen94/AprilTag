cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/;${CMAKE_MODULE_PATH}")
find_package(raspicam REQUIRED)
find_package(OpenCV)

add_library(Actuator actuator.cpp actuator.h)
include_directories ("$(PROJECT_SOURCE_DIR)/")
#target_link_libraries(bcm2835)
link_libraries(Actuator apriltags bcm2835)

IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)

add_executable(apriltags_demo apriltags_demo.cpp Serial.cpp)
target_link_libraries (apriltags_demo ${raspicam_CV_LIBS} bcm2835 Actuator)

pods_install_executables(apriltags_demo)

add_executable(calibrate calibrate.cpp Serial.cpp)
target_link_libraries (calibrate ${raspicam_CV_LIBS} bcm2835 Actuator)
pods_install_executables(calibrate)

ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()
