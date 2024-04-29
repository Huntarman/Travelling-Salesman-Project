//
// Created by micha on 07.10.2023.
//

#include "iostream"
#include "Vortex.h"
#include <Windows.h>
Vortex::Vortex(int index) {
    this->index = index;
    edgeAmountVortex = 0;
    edges = nullptr;
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
    delete[] edges;;
    edgeAmountVortex = 0;
}

long Vortex::getEdgeAmount() {
    return edgeAmountVortex;
}

Edge Vortex::getEdge(int indexW) {
    if (indexW >= edgeAmountVortex) return {-1,-1,-1};
    else return edges[indexW];
}

void Vortex::sortEdgesByDest() {
    for (int i = 0; i < edgeAmountVortex; ++i) {
        for (int j = i; j < edgeAmountVortex; ++j) {
            if (edges[i].destination > edges[j].destination){
                auto temp = edges[j];
                edges[j] = edges[i];
                edges[i] = temp;
            }
        }
    }
}

Vortex::Vortex() {

}
