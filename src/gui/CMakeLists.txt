cmake_minimum_required(VERSION 3.5)

project(gas_gui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(gas_gui SHARED
    main.cpp
    gas_gui.cpp
    gas_gui.hpp
    gui_core.hpp
    gui_core.cpp
    mainwindow.ui
    images.qrc
  )
else()
  add_executable(gas_gui
    main.cpp
    gas_gui.cpp
    gas_gui.hpp
    gui_core.hpp
    gui_core.cpp
    mainwindow.ui
    images.qrc
  )
endif()

target_link_libraries(gas_gui PRIVATE Qt5::Widgets)
target_compile_options(gas_gui PRIVATE -Og)
