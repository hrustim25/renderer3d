cmake_minimum_required(VERSION 3.12)

project(renderer-3d)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

find_package(SFML 2.5
    COMPONENTS
        system window graphics REQUIRED)

include_directories(src)
add_subdirectory(src)

add_executable(test_cube test/test_cube.cpp)
target_link_libraries(test_cube files primitives sfml-graphics sfml-window sfml-system)

add_executable(test_triangles test/test_triangles.cpp)
target_link_libraries(test_triangles files primitives sfml-graphics sfml-window sfml-system)
