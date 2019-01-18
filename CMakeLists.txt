project(Orbital)
cmake_minimum_required(VERSION 2.8.5)

file(COPY WorkingDirectory/ DESTINATION ${CMAKE_BINARY_DIR}/Source/bin)

add_subdirectory(Source)
