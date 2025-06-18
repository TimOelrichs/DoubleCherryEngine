cmake_minimum_required(VERSION 3.15)
project(DoubleCherry LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB_RECURSE PROJECT_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.c
  ${CMAKE_SOURCE_DIR}/include/*.hpp
  ${CMAKE_SOURCE_DIR}/include/*.h
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  PREFIX ""
  OUTPUT_NAME "doublecherry_libretro"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cores
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/external/cpp-httplib
  ${CMAKE_SOURCE_DIR}/external/json/nlohmann/include
)


if(MSVC)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
