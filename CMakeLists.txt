cmake_minimum_required(VERSION 3.21)
project(redis_demo C)

set(CMAKE_C_STANDARD 99)

add_executable(redis_demo main.c list.h dict.h sds.h ae.h net.h sds.c list.c)
