cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/;${CMAKE_MODULE_PATH}")
find_package(raspicam REQUIRED)
find_package(OpenCV)

link_libraries(apriltags)

IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)

add_executable(apriltags_demo apriltags_demo.cpp Serial.cpp)
target_link_libraries (apriltags_demo ${raspicam_CV_LIBS})
pods_install_executables(apriltags_demo)

add_executable(imu imu.cpp Serial.cpp)
pods_install_executables(imu)

ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()