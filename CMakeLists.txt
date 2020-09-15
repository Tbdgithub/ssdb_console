cmake_minimum_required(VERSION 3.16)
project(ssdb_console)

set(CMAKE_CXX_STANDARD 14)

add_executable(ssdb_console main.cpp tool/Hello.cpp client/util/strings.h client/util/strings.cpp client/util/bytes.h client/util/bytes.cpp
        client/net/link.h client/net/link.cpp client/net/link_redis.h client/net/link_redis.cpp

        client/SSDB_client.h
        client/SSDB_client.cpp
        client/SSDB_impl.h
        client/SSDB_impl.cpp
        )