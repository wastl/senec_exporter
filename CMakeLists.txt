cmake_minimum_required(VERSION 3.21)
project(senec_exporter)

set(CMAKE_CXX_STANDARD 14)

add_executable(senec_exporter main.cpp senec_client.cpp senec_client.h senec_influx.cpp senec_influx.h)
target_link_libraries(senec_exporter PUBLIC cpprest pthread ssl crypto yaml-cpp)
