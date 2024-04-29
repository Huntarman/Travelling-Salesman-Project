//
// Created by micha on 02.01.2024.
//

#ifndef PEA3_GRAPH_H
#define PEA3_GRAPH_H


#include "../GraphComponents/Edge.h"
#include "../GraphComponents/Vortex.h"

class Graph {
private:
    int next_ID;//ilosc wierzcholkow
    Vortex* vortices;//tablica wierzcholkow
    int** WeightMatrix;//macierz sasiedztwa

    int** population;
    int** new_population;

    int* globalBestP;
    int globalBestW;

    int* currChild;
    double * fitnessValues;
    bool* tourPair;



    long long CounterStart = 0;
    double PCFreq = 0.0;

    void StartCounter();
    double GetCounter();
    void swap(int vortex1, int vortex2 , int* path);
    void invert(int vortex1, int vortex2 , int* path);
    void insert(int vortex1, int vortex2 , int* path);
    void reverseInsert(int vortex1, int vortex2 , int* path);
    void shuffle(int* path);

    bool orig = true;
    bool created = false;

public:
    double crossoverRate = 0.8;
    double mutationRate = 0.01;
    int mutationType = 2;
    int populationSize = 10;
    int * greedyResult;
    int greedy;
    double time_found = 0.0;


    Graph();
    ~Graph();
    void copyPath(int* path1, int* path2);
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
    void deleteGraph();
    void printTSP();

    void setMutRate(double newRate);
    void setCrossRate(double newRate);
    void setMutType(int newType);
    void setPopSize (int newPopSize);
    void genPrep();//funkcja przygotowujaca do algorytmu
    void GeneticAlgorithm(double time_in_Seconds);// funkcja wykonywujaca algorytm

    void swapMutation(int* path);//mutacja swap
    void insertMutation(int* path);//mutacja insert
    void invertMutation(int* path);//mutacja invert
    void Crossover(int parent1Index, int parent2Index, int** population);//krzyzowanie OX
    int calculateWeight(int* path);
    void generateStartPopulation();//funkcja generujaca losowa populacje startowa
    void generateStartPopulationGreedy();//funkcja generujaca generacje startowa bazowana na rozwiazaniu zachlannym
    void generateNewPopulation();//funkcja generujaca nowa populacje
    void createGenotype(int** population);//funkcja obliczajaca dopasowanie nowej generacji
    bool compareParents(int* parent1, int* parent2);
    int getParent();//selekcja turniejowa indeksu rodzica
    int* getBestPath();
    int getBestWeight();
    void findGreedy();
};


#endif //PEA3_GRAPH_H
