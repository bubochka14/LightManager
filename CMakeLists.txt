cmake_minimum_required(VERSION 3.16)

project(LightManager VERSION 0.1 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Quick SerialPort Concurrent Test)

qt_standard_project_setup(REQUIRES 6.5)
add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(tests)
