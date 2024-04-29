//
// Created by micha on 05.12.2023.
//

#include "SimulatedAnnealing.h"
#include <random>
#include "iostream"
#include "algorithm"
#include <Windows.h>

void SimulatedAnnealing::StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";
    PCFreq = double(li.QuadPart);
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double SimulatedAnnealing::GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

void SimulatedAnnealing::copyPath(int *path1, int *path2) {
    for (int i = 0; i < size; ++i) {
        path1[i] = path2[i];
    }
}

void SimulatedAnnealing::findInitialResults() {
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

int SimulatedAnnealing::calculateWeight(int *path) {
    int weight = 0;
    for (int i = 1; i < size; ++i) {
        weight += matrixAdj[path[i-1]][path[i]];
    }
    weight += matrixAdj[path[size-1]][0];
    return weight;
}

SimulatedAnnealing::SimulatedAnnealing() {

}

SimulatedAnnealing::SimulatedAnnealing(int **graph, int size) {
    this->matrixAdj = new int* [size];
    for (int i = 0; i < size; ++i) {
        this->matrixAdj[i] = new int [size];
        for (int j = 0; j < size; ++j) {
            this->matrixAdj[i][j] = graph[i][j];
        }
    }
    this->size = size;
}

void SimulatedAnnealing::findGreedy() {
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

void SimulatedAnnealing::annealPrep() {
    matrixVisit = new bool [size];
    for (int i = 0; i <size; i ++){
        matrixVisit[i] = true;
    }
    best_path = new int[size];
    best_path_global = new int[size];
    time_found = 0.0;
    temperature = 30000;
}

void SimulatedAnnealing::searchTSP(double time_in_Seconds) {
    findInitialResults();
    int* currPath = new int[size];
    copyPath(currPath,best_path);
    copyPath(best_path_global,best_path);
    best_weight_global = best_weight;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, size-1);
    std::random_device rd2;
    std::mt19937 e2(rd2());
    std::uniform_real_distribution<> dist(0, 1);
    //temperature = best_weight / log(1 + coefficient) / size;
    temperature = best_weight * (1 - coefficient);
    int start_temp = temperature;
    StartCounter();
    for (int k = 1;; ++k) {
        if (GetCounter() > time_in_Seconds) {
            break;
        }
        for (int i = 0; i < size*3; ++i) {
            int a = distribution(gen), b = distribution(gen);
            while (a == b && a == 0 && b == 0) {
                a = distribution(gen);
                b = distribution(gen);
            }
            swap(a, b, currPath);
            int weight_new = calculateWeight(currPath);
            if (weight_new <= best_weight) {
                best_weight = weight_new;
                copyPath(best_path, currPath);
                if (best_weight < best_weight_global) {
                    copyPath(best_path_global, best_path);
                    best_weight_global = best_weight;
                    time_found = GetCounter();
                }
            } else {
                double p = exp(((double) -(weight_new - best_weight)) / temperature);
                if (p >= dist(e2)) {
                    copyPath(best_path, currPath);
                    best_weight = weight_new;
                } else {
                    swap(b, a, currPath);
                }
            }
        }
        switch (cooling_schedule) {
            case 1:
                temperature *=  (1 -coefficient);
                break;

            case 2:
                temperature = temperature/( 1 + temperature * coefficient);
                break;

            case 3:
                temperature = temperature/(1 +log(1+k));
                break;
        }

    }
    delete[] currPath;
}

int *SimulatedAnnealing::getBestPath() {
    return best_path_global;
}

int SimulatedAnnealing::getBestWeight() {
    return best_weight_global;

}

void SimulatedAnnealing::swap(int vortex1, int vortex2, int *path) {
    int pom = path[vortex1];
    path[vortex1] = path[vortex2];
    path[vortex2] = pom;
}

void SimulatedAnnealing::setCoolingSchedule(int schedule) {
    switch(schedule){
        case 1:
        case 2:
        case 3:
            cooling_schedule = schedule;
            break;
        default:
            std::cout<<"Wrong schedule!\n";
    }
}

void SimulatedAnnealing::insert(int vortex1, int vortex2, int *path) {
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
void SimulatedAnnealing::reverseInsert(int vortex1, int vortex2, int *path) {

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

SimulatedAnnealing::~SimulatedAnnealing() {
    delete[] greedyResult;
    delete[] best_path;
    delete[] matrixVisit;
    delete[] matrixAdj;
    delete[] best_path_global;
}
