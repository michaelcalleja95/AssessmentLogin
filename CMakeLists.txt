cmake_minimum_required (VERSION 3.12)

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "vcpkg_installed/x64-windows/include")
#SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "vcpkg_installed/x64-windows/lib")

find_package(boost REQUIRED beast)

project ("AssessmentLogin")

add_executable (LoginAssessment "AssessmentLogin.cpp" "AssessmentLogin.h")

include_directories( ${Boost_INCLUDE_DIR} )

set_property(TARGET LoginAssessment PROPERTY CXX_STANDARD 20)