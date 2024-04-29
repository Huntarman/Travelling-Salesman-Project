//
// Created by micha on 18.11.2023.
//

#ifndef PEA_PROJECT_2_VORTEX_H
#define PEA_PROJECT_2_VORTEX_H


#include "Edge.h"

class Vortex {
private:
    int index;
    Edge* edges;
    long edgeAmountVortex;

public:
    bool visited = false;
    explicit Vortex(int index);
    ~Vortex();
    Vortex();
    void addEdge(Edge edge);
    void deleteEdges();
    void printEdges();
    long getEdgeAmount();
    Edge getEdge(int index);
};


#endif //PEA_PROJECT_2_VORTEX_H
