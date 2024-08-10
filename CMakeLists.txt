cmake_minimum_required(VERSION 3.24)
project(MomidController C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the ViGEmClient library
add_library(ViGEmClient STATIC IMPORTED receiver.c receiver.h TcpServer.c)

set_target_properties(ViGEmClient PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/ViGEmClient.lib)

add_executable(MomidController main.c udp_broadcast_receiver.c receiver.h TcpServer.c TcpServer.c)

target_link_libraries(MomidController ViGEmClient setupapi.lib)
