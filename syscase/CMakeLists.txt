cmake_minimum_required(VERSION 3.0)

project(syscase C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/core
  ${PROJECT_SOURCE_DIR}/include/linux
)

set(APP_SOURCES src/app/main.c)
file(GLOB_RECURSE CORE_LIB_SOURCES src/core/**.c)
file(GLOB_RECURSE LINUX_LIB_SOURCES src/linux/**.c)
file(GLOB_RECURSE LINUX_COMMON_LIB_SOURCES src/linux_common/**.c)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

set(CORE_LIBRARY_NAME syscase_core)
add_library(${CORE_LIBRARY_NAME} STATIC ${CORE_LIB_SOURCES})

set(LINUX_COMMON_LIBRARY_NAME syscase_linux_common)
add_library(${LINUX_COMMON_LIBRARY_NAME} STATIC ${LINUX_COMMON_LIB_SOURCES})
target_link_libraries(${LINUX_COMMON_LIBRARY_NAME} ${CORE_LIBRARY_NAME})

set(LINUX_LIBRARY_NAME syscase_linux)
add_library(${LINUX_LIBRARY_NAME} STATIC ${LINUX_LIB_SOURCES})
target_link_libraries(${LINUX_LIBRARY_NAME} ${CORE_LIBRARY_NAME} ${LINUX_COMMON_LIBRARY_NAME})

target_link_libraries(${PROJECT_NAME} ${CORE_LIBRARY_NAME} ${LINUX_COMMON_LIBRARY_NAME} ${LINUX_LIBRARY_NAME})

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
add_definitions(-D SYSCASE_DUMMY)
