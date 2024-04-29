//
// Created by micha on 02.01.2024.
//

#ifndef PEA3_VORTEX_H
#define PEA3_VORTEX_H


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


#endif //PEA3_VORTEX_H
