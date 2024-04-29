//
// Created by micha on 07.10.2023.
//

#ifndef PEAPROJECT_GRAPH_H
#define PEAPROJECT_GRAPH_H


#include "../GraphComponents/Vortex.h"
#include "vector"

class Graph {
private:
    int next_ID;//ilosc wierzcholkow
    Vortex* vortices;//tablica wierzcholkow
    bool TSPcreated = false;//pomocnicza zmienna - do wyswietlania TSP
    int* TSP_path;//tablica przechowujaca aktualna sciezke
    int* TSP_path_best;//tablica przechowujaca najlepsza sciezke sciezke
    int** WeightMatrix;//macierz sasiedztwa
    int TSP_path_weight_best;
    // normal TSP - recurence, complete inspection
    //pomocnicze zmienne

    int TSP_path_index;
    int TSP_path_weight;
    int origin_index;

    /*/TSP, B&B method
    int* TSP_path_BnB;
    int* TSP_path_BnB_best;
    int lowerBound = 0, upperBound = INT32_MAX;
    int** WeightMatrix; // BnB ver
    int** ZerosOnWeightMatrix;
    int findMinInRow(int index);
    int findMinInColumn(int index);
    int sumOfArray(int* array, int size);
    void printWeightMatrix();*/

    //TSP DP method
    long all_visited;
    int** memoArr;
    void findPath(int position, long mask);

public:
    Graph();
    ~Graph();
    void addVortex();//funkcja dodajaca wierzcholek
    void addMultipleVortices(int amount);//funkcja dodajaca wiele wierzcolkow
    void addEdge(Edge edge);//funkcja dodajaca krawedz do wierzcholka
    void fillGraphWithEdgesRandom();//funkcja wypelniajaca wierzcholki krawedziami
    void printGraphList();//wyswietlanie grafu
    void printGraphMatrix();//wyswietlanie grafu
    void TSP(int start_index);//funkcja wlasciwa przegladu zupelnego
    void TSP();// funkcja zaczynajaca przeglad zupelny
    void TSP_prep();//funkcja przygotowawcza do przegladu zupelnego
    /*void TSP_BNB();
    void TSP_BNB_starter();*/
    int TSP_DP(int position, int mask);//funkcja wlasciwa algorytmu DP
    void TSP_DP(int starting_index);//funkcja zaczynająca algorytm DP
    void TSP_DP_prep();//funkcja przygotowawcza do algorytmu DP
    void printTSP();
    int getVortexAmount();
};


#endif //PEAPROJECT_GRAPH_H
