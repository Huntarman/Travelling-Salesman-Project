//
// Created by micha on 18.11.2023.
//

#ifndef PEA_PROJECT_2_GRAPH_H
#define PEA_PROJECT_2_GRAPH_H


#include "../GraphComponents/Edge.h"
#include "../GraphComponents/Vortex.h"

class Graph {
private:
    int next_ID;//ilosc wierzcholkow
    Vortex* vortices;//tablica wierzcholkow
    int** WeightMatrix;//macierz sasiedztwa

public:
    Graph();
    ~Graph();
    void addVortex();//funkcja dodajaca wierzcholek
    void addMultipleVortices(int amount);//funkcja dodajaca wiele wierzcolkow]
    void makeMatrix();
    void addEdge(Edge edge);//funkcja dodajaca krawedz do wierzcholka
    void fillGraphWithEdgesRandom();//funkcja wypelniajaca wierzcholki krawedziami
    void printGraphList();//wyswietlanie grafu
    void printGraphMatrix();//wyswietlanie grafu
    int** getGraph();
    void setGraph(int** newGraph);
    int getVortexAmount();
};


#endif //PEA_PROJECT_2_GRAPH_H
