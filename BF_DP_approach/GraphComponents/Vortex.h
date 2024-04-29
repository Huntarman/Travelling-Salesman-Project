//
// Created by micha on 07.10.2023.
//

#ifndef PEAPROJECT_VORTEX_H
#define PEAPROJECT_VORTEX_H


#include "Edge.h"

class Vortex {
private:
    int index;
    Edge* edges;
    long edgeAmountVortex;

public:
    bool visited = false;
    explicit Vortex(int index);
    Vortex();
    void addEdge(Edge edge);
    void deleteEdges();
    void printEdges();
    long getEdgeAmount();
    Edge getEdge(int index);
    void sortEdgesByDest();
};


#endif //PEAPROJECT_VORTEX_H
