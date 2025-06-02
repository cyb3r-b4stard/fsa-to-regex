cmake_minimum_required(VERSION 3.14.0)

project(fsa_to_regexp VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(HEADER_FILES
    include/utils.h
    include/fsa.h
)

set(SOURCE_FILES
    src/utils.cpp
    src/fsa.cpp
)

add_executable(main ${HEADER_FILES} ${SOURCE_FILES} ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_directories(main PUBLIC ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)