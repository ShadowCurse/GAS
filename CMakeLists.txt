cmake_minimum_required(VERSION 3.17)
project(GameAssetsDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(GAS src/main.cpp
        src/database/db_connector.cpp
        src/database/db_connector.hpp
        src/sql/query.hpp
        src/sql/table.hpp
        src/core/core.cpp
        src/core/core.hpp src/core/tables.hpp src/core/query_collection.hpp)

target_include_directories(GAS PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(GAS PRIVATE pqxx pq)