cmake_minimum_required(VERSION 3.24)
project(PEAProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEAProject main.cpp
        Graph/Graph.cpp
        Graph/Graph.h
        GraphComponents/Vortex.cpp
        GraphComponents/Vortex.h
        GraphComponents/Edge.cpp
        GraphComponents/Edge.h
        )
