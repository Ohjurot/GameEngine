cmake_minimum_required(VERSION 3.22)

cmake_minimum_required(VERSION 3.22)

set(TARGET_NAME EngineWindows)

file(GLOB_RECURSE PROJECT_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
file(GLOB_RECURSE PROJECT_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

add_library(${TARGET_NAME} STATIC ${PROJECT_HDRS} ${PROJECT_SRCS})
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${TARGET_NAME} PUBLIC d3d12 dxgi dxguid)

set_target_properties(${TARGET_NAME} PROPERTIES CMAKE_CXX_STANDARD 20)