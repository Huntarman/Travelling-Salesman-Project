//
// Created by micha on 18.11.2023.
//

#ifndef PEA_PROJECT_2_TABUSEARCH_H
#define PEA_PROJECT_2_TABUSEARCH_H


#include "TabuList.h"

class TabuSearch {
private:
    TabuList tabuList;
    int* best_path;
    int best_weight;
    int* best_path_global;
    int best_weight_global;
    int* best_localP;
    int best_localW;

    int** matrixAdj;
    bool* matrixVisit;
    int size;
    int neighborhoodMode = 1;
    int cadence;
    int diversificationMeter = 0;
    int diversificationReq = 15000;

    long long CounterStart = 0;
    double PCFreq = 0.0;

    void StartCounter();
    double GetCounter();
    void swap(int vortex1, int vortex2 , int* path);
    void invert(int vortex1, int vortex2 , int* path);
    void insert(int vortex1, int vortex2 , int* path);
    void diversification(int* path);
    void reverseInsert(int vortex1, int vortex2 , int* path);

public:
    int * greedyResult;
    int greedy;
    double time_found = 0.0;

    void copyPath(int* path1, int* path2);
    void findInitialResults();
    int calculateWeight(int* path);
    TabuSearch();
    ~TabuSearch();
    TabuSearch(int** graph, int size);
    void findGreedy();
    void tabuPrep();
    void searchTSP(double time);
    void setCadence(int cadence_new);
    void setNeighborhoodMode(int mode);
    int* getBestPath();
    int getBestWeight();

};


#endif //PEA_PROJECT_2_TABUSEARCH_H
