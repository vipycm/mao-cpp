cmake_minimum_required(VERSION 3.6)
project(mao_server_win)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.cpp server/MaoServer.cpp commons/MaoLog.cpp)
add_executable(mao_server_win ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(mao_server_win wsock32 ws2_32)
endif ()