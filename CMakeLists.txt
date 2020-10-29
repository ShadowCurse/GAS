cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)

project(GameAssetsDB)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(GAS src/main.cpp
        src/utility/db_connector.hpp
        src/utility/query.hpp
        src/utility/table.hpp
        src/core/core.cpp
        src/core/core.hpp
        src/core/tables.hpp
        src/core/query_collection.hpp)

target_include_directories(GAS PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(GAS PRIVATE pqxx pq ${CONAN_LIBS})