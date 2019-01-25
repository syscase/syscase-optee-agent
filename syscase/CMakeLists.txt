cmake_minimum_required(VERSION 3.0)

project(syscase C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/core
)

set(APP_SOURCES src/app/main.c)
file(GLOB_RECURSE LIB_SOURCES src/core/**.c)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

set(LIBRARY_NAME syscase_core)
add_library(${LIBRARY_NAME} STATIC ${LIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME})
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
add_definitions(-D SYSCASE_DUMMY)
