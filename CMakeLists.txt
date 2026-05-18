cmake_minimum_required(VERSION 3.16)

project(HotelBookingQt_MVP VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(HotelBookingQt_MVP
    main.cpp
    mainwindow.h
    mainwindow.cpp
    room.h
    room.cpp
    booking.h
    booking.cpp
    hotelbookingsystem.h
    hotelbookingsystem.cpp
)

target_link_libraries(HotelBookingQt_MVP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
