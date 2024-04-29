cmake_minimum_required(VERSION 3.24)
project(PEA_Project_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEA_Project_2 main.cpp GraphComponents/Edge.cpp GraphComponents/Edge.h GraphComponents/Vortex.cpp GraphComponents/Vortex.h Graph/Graph.cpp Graph/Graph.h TabuSearch/TabuSearch.cpp TabuSearch/TabuSearch.h TabuSearch/TabuList.cpp TabuSearch/TabuList.h tinyxml2.h tinyxml2.cpp SimulatedAnnealing/SimulatedAnnealing.cpp SimulatedAnnealing/SimulatedAnnealing.h)
