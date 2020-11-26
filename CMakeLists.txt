cmake_minimum_required(VERSION 3.17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)

project(GAS LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(GAS
        src/core/core.hpp
        src/core/tables.hpp
        src/core/cache.hpp
        src/core/connector.hpp
        src/core/query.hpp
        src/core/notification.hpp
        src/core/settings.hpp
        src/core/storage.hpp

        src/gui/gas_gui.cpp
        src/gui/gas_gui.hpp
        src/gui/mainwindow.ui
        src/gui/images.qrc

        src/gas_core.hpp
        src/gas_core.cpp
        src/main.cpp
        )

target_include_directories(GAS PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(GAS PRIVATE ${CONAN_LIBS} Qt5::Widgets)

add_executable(Tests
        tests/main.cpp
        tests/connector_tests.hpp tests/storage_tests.hpp tests/cache_tests.hpp)

target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Tests PRIVATE ${CONAN_LIBS})