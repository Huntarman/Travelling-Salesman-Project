//
// Created by micha on 18.11.2023.
//

#include <random>
#include <iostream>
#include "Graph.h"

std::random_device rand_dev;
std::mt19937 generator(rand_dev());
std::uniform_int_distribution<int> distribution(1, 100);

Graph::Graph() {
    this->next_ID = 0;
    this->vortices = nullptr;
    this->WeightMatrix = nullptr;
}

void Graph::addVortex() {
    auto newVortex = Vortex(next_ID++);
    Vortex* pom = new Vortex[next_ID];
    for (int i = 0; i < next_ID-1; ++i) {
        pom[i] = vortices[i];
    }
    delete[] vortices;
    pom[next_ID-1] = newVortex;
    vortices = pom;
}

void Graph::addMultipleVortices(int amount) {
    for (int i = 0; i<amount; i++) {
        addVortex();
    }
}

void Graph::addEdge(Edge edge) {
    if (edge.source > next_ID-1 || edge.destination > next_ID-1){
        std::cout<<"Can't add edge!\n";
        return;
    }
    vortices[edge.source].addEdge(edge);
}

void Graph::fillGraphWithEdgesRandom() {
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i != j) {
                int weight = distribution(generator);
                vortices[i].addEdge(Edge(i, j, weight));
            }
        }
    }

}

void Graph::printGraphList() {
    if (next_ID == 0){
        std::cout<<"Graph Empty\n";
        return;
    }
    for (int i = 0; i < next_ID; i++) {
        vortices[i].printEdges();
        std::cout<<"\n";
    }
    std::cout<<"\n";
}

void Graph::printGraphMatrix() {
    if (next_ID == 0){
        std::cout<<"Graph Empty\n";
        return;
    }
    std::cout<<"\t";
    for (int i = 0; i < next_ID; ++i) {
        std::cout<<i<<"\t";
    }
    std::cout<<"\n";
    for (int i = 0; i < next_ID; ++i) {
        std::cout<<i<<"\t";
        for (int j = 0; j < next_ID; ++j) {
            if (i == j) std::cout<<"-1\t";
            else std::cout<<vortices[i].getEdge((j<i)? j:j-1).capacity<<"\t";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";
}

Graph::~Graph() {
    for (int i = 0; i < next_ID; ++i) {
        vortices[i].deleteEdges();
    }
    delete[] WeightMatrix;
    delete[] vortices;
    next_ID = 0;
}

int Graph::getVortexAmount() {
    return next_ID;
}

int **Graph::getGraph() {
    return WeightMatrix;
}

void Graph::makeMatrix() {
    WeightMatrix = new int*[next_ID];
    for (int i = 0; i < next_ID; ++i) {
        WeightMatrix[i] = new int[next_ID];

    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i==j) WeightMatrix[i][j] = INT32_MAX;
            //else if (vortices[i].getEdge((j<i)? j-1:j).capacity == 0) WeightMatrix[i][j] = INT32_MAX;
            else WeightMatrix[i][j] = vortices[i].getEdge((j<i)? j:j-1).capacity;
        }
    }

}

void Graph::setGraph(int **newGraph) {
    this->WeightMatrix = newGraph;
}
