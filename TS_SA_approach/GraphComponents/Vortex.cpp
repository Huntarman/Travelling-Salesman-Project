//
// Created by micha on 18.11.2023.
//

#include "Vortex.h"
#include "iostream"
Vortex::Vortex(int index) {
    this->index = index;
    this->edgeAmountVortex = 0;
    this->edges = nullptr;
}

void Vortex::addEdge(Edge edge) {
    if (edgeAmountVortex == 0){
        edgeAmountVortex++;
        delete[] edges;
        edges = new Edge;
        edges[0] = edge;
    }
    else{
        Edge* pom = new Edge[++edgeAmountVortex];
        for (int i = 0; i < edgeAmountVortex - 1; ++i) {
            pom[i] = edges[i];
        }
        pom[edgeAmountVortex - 1] = edge;
        delete[] edges;
        edges = pom;
    }
}
void Vortex::printEdges() {
    std::cout<<"V:"<<index<<"[";
    for (int i = 0; i<edgeAmountVortex; i++){
        if(i != edgeAmountVortex-1) std::cout<<edges[i].destination<<":"<<edges[i].capacity<<", ";
        else std::cout<<edges[i].destination<<":"<<edges[i].capacity;
    }
    std::cout<<"]";
}

void Vortex::deleteEdges() {
    delete[] edges;
    edges = nullptr;
    edgeAmountVortex = 0;
}

long Vortex::getEdgeAmount() {
    return edgeAmountVortex;
}

Edge Vortex::getEdge(int indexW) {
    if (indexW >= edgeAmountVortex) return {-1,-1,-1};
    else return edges[indexW];
}

Vortex::~Vortex() {
    if (edges != nullptr)
        delete[] edges;
}

Vortex::Vortex() {

}
