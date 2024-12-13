cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

add_executable(sensor ${SOURCES} ${HEADERS})
target_compile_options(sensor PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(sensor PRIVATE src src/layers src/layers/contracts)
target_link_libraries(sensor PRIVATE semf)

