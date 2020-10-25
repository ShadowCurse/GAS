cmake_minimum_required(VERSION 3.17)
project(GameAssetsDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(GameAssetsDB src/main.cpp
                            src/database/db_connector.cpp
                            src/database/db_connector.hpp
                            src/sql/queries.hpp
                            src/sql/tables.hpp
                            src/core/core.cpp
                            src/core/core.hpp)
target_link_libraries(GameAssetsDB PRIVATE pqxx pq)