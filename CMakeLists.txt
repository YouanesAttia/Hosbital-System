cmake_minimum_required(VERSION 3.10)
project(HospitalSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(HospitalSystem
    main.cpp
    manager.cpp
    mainwindow.cpp
)

target_link_libraries(HospitalSystem PRIVATE Qt6::Widgets)

