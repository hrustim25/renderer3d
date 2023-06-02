cmake_minimum_required(VERSION 3.12)

project(renderer-3d)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Release)

find_package(SFML 2.5
    COMPONENTS
        system window graphics REQUIRED)

include_directories(src)
add_subdirectory(src)

set(TESTS_INCLUDE_LIBRARIES files primitives sfml-graphics sfml-window sfml-system)

add_executable(test_cube test/test_cube.cpp)
target_link_libraries(test_cube ${TESTS_INCLUDE_LIBRARIES})

add_executable(test_triangles test/test_triangles.cpp)
target_link_libraries(test_triangles ${TESTS_INCLUDE_LIBRARIES})

add_executable(test_interpolation test/test_interpolation.cpp)
target_link_libraries(test_interpolation ${TESTS_INCLUDE_LIBRARIES})

add_executable(test_teapot test/test_teapot.cpp)
target_link_libraries(test_teapot ${TESTS_INCLUDE_LIBRARIES})

add_executable(test_naruto test/test_naruto.cpp)
target_link_libraries(test_naruto ${TESTS_INCLUDE_LIBRARIES})
