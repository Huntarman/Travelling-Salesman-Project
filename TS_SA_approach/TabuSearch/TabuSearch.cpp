//
// Created by micha on 18.11.2023.
//

#include "TabuSearch.h"
#include <random>
#include "iostream"
#include "algorithm"
#include <Windows.h>


void TabuSearch::searchTSP(double time_in_Seconds) {
    findInitialResults();
    int* currPath = new int[size];
    copyPath(currPath,best_path);
    best_path_global = new int[size];
    best_localP = new int[size];
    copyPath(best_path_global,best_path);
    best_weight_global = best_weight;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, size-1);
    StartCounter();

    int cA,cB;
    for (int k = 1;; k++) {
        if (GetCounter() > time_in_Seconds) {
            break;
        }
        cA=0;
        cB=0;
        bool flagChange = false;
        for (int i = 1; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                /*for (int l = 0; l < size; ++l) {
                    std::cout<< currPath[l] <<" ";
                }std::cout<<"\n";*/
                switch (neighborhoodMode) {
                    case 1:
                        swap(i,j,currPath);
                        break;
                    case 2:
                        insert(i,j,currPath);
                        break;
                    case 3:
                        invert(i,j,currPath);
                        break;
                }
                long new_weight = calculateWeight(currPath);
                if (tabuList.isTabu(i,j)){ //tabo action
                    if(new_weight >= best_weight_global) {// aspiration
                        switch (neighborhoodMode) {
                            case 1:
                                swap(j,i,currPath);
                                break;
                            case 2:
                                reverseInsert(i,j,currPath);
                                break;
                            case 3:
                                invert(j,i,currPath);
                                break;
                        }
                        continue;
                    }

                }
                if (j == i + 1){
                    copyPath(best_localP, currPath);
                    best_localW = new_weight;
                    cA = i; cB = j;
                    if (best_localW < best_weight_global){
                        copyPath(best_path_global, best_localP);
                        best_weight_global = best_localW;
                        flagChange = true;
                    }
                }
                else{
                    if (best_localW > new_weight){
                        copyPath(best_localP, currPath);
                        best_localW = new_weight;
                        cA = i; cB = j;
                        if (best_localW < best_weight_global){
                            copyPath(best_path_global, best_localP);
                            best_weight_global = best_localW;
                            flagChange = true;
                            time_found = GetCounter();
                        }
                    }
                }
                switch (neighborhoodMode) {
                    case 1:
                        swap(j,i,currPath);
                        break;
                    case 2:
                        reverseInsert(i,j,currPath);
                        break;
                    case 3:
                        invert(j,i,currPath);
                        break;
                }
            }
        }
        //std::cout<<"ENDDD_!_!_!_@_#_!#!_!$_!$_!__#$!_)$@$)&(!^$@&&)!(@%*$!&)@!#\n";
        copyPath(currPath, best_localP);

        tabuList.AddTabu(cA,cB, 5*(int)sqrt(size));
        tabuList.decrementTabu();


        if(flagChange) {
            diversificationMeter = 0;
            continue;
        }
        diversificationMeter++;
        if (diversificationMeter == diversificationReq){
            int a = distribution(gen), b = distribution(gen);
            while (a==b && a==0 && b == 0){
                a = distribution(gen);
                b = distribution(gen);
            }
            insert(a,b,currPath);
            diversificationMeter = 0;
        }
    }
}
void TabuSearch::swap(int vortex1, int vortex2 , int* path){
    int pom = path[vortex1];
    path[vortex1] = path[vortex2];
    path[vortex2] = pom;
}
void TabuSearch::insert(int vortex1, int vortex2 , int* path) {

    int biggerIndex,smallerIndex;
    if (vortex1 > vortex2){
        biggerIndex = vortex1;
        smallerIndex = vortex2;
    }
    else{
        biggerIndex = vortex2;
        smallerIndex = vortex1;
    }

    int pom = path[biggerIndex];
    for (int i = biggerIndex; i > smallerIndex; i--) {
        path[i] = path[i-1];
    }
    path[smallerIndex] = pom;
}

void TabuSearch::reverseInsert(int vortex1, int vortex2, int *path) {

    int biggerIndex,smallerIndex;
    if (vortex1 > vortex2){
        biggerIndex = vortex1;
        smallerIndex = vortex2;
    }
    else{
        biggerIndex = vortex2;
        smallerIndex = vortex1;
    }

    int pom = path[smallerIndex];
    for (int i = smallerIndex; i < biggerIndex; i++) {
        path[i] = path[i+1];
    }
    path[biggerIndex] = pom;
}

void TabuSearch::invert(int vortex1, int vortex2 , int* path) {
    int biggerIndex,smallerIndex;
    if (vortex1 > vortex2){
        biggerIndex = vortex1;
        smallerIndex = vortex2;
    }
    else{
        biggerIndex = vortex2;
        smallerIndex = vortex1;
    }

    while (smallerIndex < biggerIndex) {
        int pom = path[smallerIndex];
        path[smallerIndex] = path[biggerIndex];
        path[biggerIndex] = pom;
        smallerIndex++;
        biggerIndex--;
    }
}
void TabuSearch::copyPath(int* path1, int* path2){
    for (int i = 0; i < size; ++i) {
        path1[i] = path2[i];
    }
}
void TabuSearch::findInitialResults() {
    best_path[0] = 0;
    matrixVisit[0] = false;
    best_weight = 0;
    for (int i = 1; i < size; ++i) {
        int index_best = -1;
        int weight_best = INT32_MAX;
        for (int j = 0; j < size; ++j) {
            if (!matrixVisit[j]) continue;

            if ( matrixAdj[ best_path[i-1] ] [j] < weight_best){
                index_best = j;
                weight_best = matrixAdj[best_path[i-1]][j];
            }
        }
        best_path[i] = index_best;
        matrixVisit[index_best] = false;
        best_weight += weight_best;
    }
    best_weight += matrixAdj[best_path[size-1]][0];
}

void TabuSearch::tabuPrep(){
    matrixVisit = new bool [size];
    for (int i = 0; i <size; i ++){
        matrixVisit[i] = true;
    }
    best_path = new int[size];
    tabuList = *new TabuList(size, (int) sqrt(size));
}

void TabuSearch::setCadence(int cadence_new) {
    this->cadence = cadence_new;
}

int TabuSearch::getBestWeight() {
    return best_weight_global;
}

int *TabuSearch::getBestPath() {
    return best_path_global;
}

void TabuSearch::setNeighborhoodMode(int mode) {
    this->neighborhoodMode = mode;
}

TabuSearch::TabuSearch() {
}

TabuSearch::TabuSearch(int** graph, int size) {
    this->matrixAdj = new int* [size];
    for (int i = 0; i < size; ++i) {
        this->matrixAdj[i] = new int [size];
        for (int j = 0; j < size; ++j) {
            this->matrixAdj[i][j] = graph[i][j];
            //std::cout<< this->matrixAdj[i][j]<<" ";
        }//std::cout<<"\n";
    }
 this->size = size;
}

int TabuSearch::calculateWeight(int *path) {
    int weight = 0;
    for (int i = 1; i < size; ++i) {
        weight += matrixAdj[path[i-1]][path[i]];
    }
    weight += matrixAdj[path[size-1]][0];
    return weight;
}

void TabuSearch::diversification(int *path) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 1; i < size; ++i) {
        std::uniform_int_distribution<int> distribution(1, i);
        int randomIndex = distribution(gen);
        int pom = path[randomIndex];
        path[randomIndex] = path[i];
        path[i] = pom;
    }
}

void TabuSearch::StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";
    PCFreq = double(li.QuadPart);
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double TabuSearch::GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

void TabuSearch::findGreedy() {
    matrixVisit = new bool [size];
    for (int i = 0; i <size; i ++){
        matrixVisit[i] = true;
    }
    greedyResult = new int[size];
    greedyResult[0] = 0;
    matrixVisit[0] = false;
    greedy = 0;
    for (int i = 1; i < size; ++i) {
        int index_best = -1;
        int weight_best = INT32_MAX;

        for (int j = 0; j < size; ++j) {
            if (!matrixVisit[j]) continue;
            //if (i == 1) std::cout<< matrixAdj[ 0 ] [j]<<"\n";
            if ( matrixAdj[ greedyResult[i-1] ] [j] < weight_best){
                index_best = j;
                weight_best = matrixAdj[greedyResult[i-1]][j];
            }
        }
        greedyResult[i] = index_best;

        matrixVisit[index_best] = false;
        greedy += weight_best;
    }
    greedy += matrixAdj[greedyResult[size-1]][0];
}

TabuSearch::~TabuSearch() {
    delete[] greedyResult;
    delete[] best_path;
    delete[] matrixVisit;
    delete[] matrixAdj;
}
