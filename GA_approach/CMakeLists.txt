cmake_minimum_required(VERSION 3.24)
project(Pea3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pea3 main.cpp Graph/Graph.cpp Graph/Graph.h GraphComponents/Edge.cpp GraphComponents/Edge.h GraphComponents/Vortex.cpp GraphComponents/Vortex.h tinyxml2.cpp tinyxml2.h)
