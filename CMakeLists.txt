cmake_minimum_required(VERSION 3.31)
project(socketstest)

set(CMAKE_CXX_STANDARD 23)

if(MINGW OR WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

add_executable(ChatServer
        server_main.cpp
        Socket.cpp
        Server.cpp
)

add_executable(ChatClient
        client_main.cpp
        Socket.cpp
        Client.cpp
)

target_link_libraries(ChatServer ws2_32)
target_link_libraries(ChatClient ws2_32)