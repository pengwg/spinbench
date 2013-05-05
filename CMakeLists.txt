project(SpinBenchPlugins)

cmake_minimum_required(VERSION 2.6.0)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Script REQUIRED)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(SB2DPRGradient)
add_subdirectory(SBCathTrackGradient)
add_subdirectory(SB3DPRReadout)
