cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)

project(GAS)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(GAS src/main.cpp
        src/utility/connector.hpp
        src/utility/query.hpp
        src/core/core.cpp
        src/core/core.hpp
        src/core/tables.hpp
        src/utility/notification.hpp src/utility/settings.hpp)

target_include_directories(GAS PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(GAS PRIVATE ${CONAN_LIBS})

add_executable(Tests tests/main.cpp
        tests/connector_tests.hpp)

target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Tests PRIVATE ${CONAN_LIBS})