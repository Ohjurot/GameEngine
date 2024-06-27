cmake_minimum_required(VERSION 3.22)

set(TARGET_NAME EmptyGame)

file(GLOB_RECURSE PROJECT_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
file(GLOB_RECURSE PROJECT_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

add_executable(${TARGET_NAME} ${PROJECT_HDRS} ${PROJECT_SRCS})
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${TARGET_NAME} PRIVATE EngineCore EngineWindows)

set_target_properties(${TARGET_NAME} PROPERTIES CMAKE_CXX_STANDARD 20)