cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/;${CMAKE_MODULE_PATH}")
find_package(raspicam REQUIRED)
find_package(OpenCV)

add_library(Actuator actuator.cpp actuator.h)
include_directories ("$(PROJECT_SOURCE_DIR)/")
link_libraries(apriltags bcm2835)

IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)

add_executable(apriltags_demo apriltags_demo.cpp Serial.cpp actuator.cpp actuator.h)
target_link_libraries (apriltags_demo ${raspicam_CV_LIBS} bcm2835 Actuator)
pods_install_executables(apriltags_demo)

# add_executable(imu imu.cpp Serial.cpp)
# pods_install_executables(imu)

ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()