//
// Created by micha on 05.12.2023.
//

#ifndef PEA_PROJECT_2_SIMULATEDANNEALING_H
#define PEA_PROJECT_2_SIMULATEDANNEALING_H


class SimulatedAnnealing {

    int* best_path;
    int* best_path_global;
    int best_weight_global;
    int best_weight;
    int** matrixAdj;
    bool* matrixVisit;
    int size;
    double temperature = 30000.0;
    double coefficient = 0.001;
    int cooling_schedule = 1;
    /**
     * assume 0 < c < 1
     * cooling schedule modes:
     * 1 - Exponential: temp = temp * (1-c)
     * 2 - Inverse Logarithmic: temp = temp / ( 1 + temp * c)
     * 3 - Logarithmic, based on starting temp: temp = temp(0)* c/ln(1 + iterations)
     */
    long long CounterStart = 0;
    double PCFreq = 0.0;
    void StartCounter();
    double GetCounter();
    void swap(int vortex1, int vortex2 , int* path);
    void insert(int vortex1, int vortex2 , int* path);
    void reverseInsert(int vortex1, int vortex2 , int* path);

public:
    int * greedyResult;
    int greedy;
    double time_found = 0.0;

    void copyPath(int* path1, int* path2);
    void findInitialResults();
    int calculateWeight(int* path);
    SimulatedAnnealing();
    ~SimulatedAnnealing();
    SimulatedAnnealing(int** graph, int size);
    void findGreedy();
    void annealPrep();
    void searchTSP(double time_in_Seconds);
    void setCoolingSchedule(int schedule);
    int* getBestPath();
    int getBestWeight();
};


#endif //PEA_PROJECT_2_SIMULATEDANNEALING_H
