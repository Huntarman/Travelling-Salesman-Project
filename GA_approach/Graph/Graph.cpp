//
// Created by micha on 02.01.2024.
//

#include <iostream>
#include "Graph.h"
#include <random>
#include "algorithm"
#include <Windows.h>

std::random_device rd;
std::mt19937 gen(rd());

Graph::Graph() {
    this->next_ID = 0;
    this->vortices = nullptr;
    this->WeightMatrix = nullptr;
}

void Graph::addVortex() {
    auto newVortex = Vortex(next_ID++);
    Vortex *pom = new Vortex[next_ID];
    for (int i = 0; i < next_ID - 1; ++i) {
        pom[i] = vortices[i];
    }
    delete[] vortices;
    pom[next_ID - 1] = newVortex;
    vortices = pom;
}

void Graph::addMultipleVortices(int amount) {
    for (int i = 0; i < amount; i++) {
        addVortex();
    }
}

void Graph::addEdge(Edge edge) {
    if (edge.source > next_ID - 1 || edge.destination > next_ID - 1) {
        std::cout << "Can't add edge!\n";
        return;
    }
    vortices[edge.source].addEdge(edge);
}

void Graph::fillGraphWithEdgesRandom() {
    std::uniform_int_distribution<> dist(1, 100);
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i != j) {
                int weight = dist(gen);
                vortices[i].addEdge(Edge(i, j, weight));
            }
        }
    }

}

void Graph::printGraphList() {
    if (next_ID == 0) {
        std::cout << "Graph Empty\n";
        return;
    }
    for (int i = 0; i < next_ID; i++) {
        vortices[i].printEdges();
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Graph::printGraphMatrix() {
    if (next_ID == 0) {
        std::cout << "Graph Empty\n";
        return;
    }
    std::cout << "\t";
    for (int i = 0; i < next_ID; ++i) {
        std::cout << i << "\t";
    }
    std::cout << "\n";
    for (int i = 0; i < next_ID; ++i) {
        std::cout << i << "\t";
        for (int j = 0; j < next_ID; ++j) {
            if (i == j) std::cout << "-1\t";
            else std::cout << vortices[i].getEdge((j < i) ? j : j - 1).capacity << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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
    WeightMatrix = new int *[next_ID];
    for (int i = 0; i < next_ID; ++i) {
        WeightMatrix[i] = new int[next_ID];

    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i == j) WeightMatrix[i][j] = INT32_MAX;
                //else if (vortices[i].getEdge((j<i)? j-1:j).capacity == 0) WeightMatrix[i][j] = INT32_MAX;
            else WeightMatrix[i][j] = vortices[i].getEdge((j < i) ? j : j - 1).capacity;
        }
    }

}

void Graph::setGraph(int **newGraph) {
    this->WeightMatrix = newGraph;
}

void Graph::deleteGraph() {
    delete[] vortices;
    delete[] WeightMatrix;
    delete[] globalBestP;
    delete[] population;
    delete[] new_population;
    globalBestW = INT32_MAX;

    next_ID = 0;
    created = false;
}

void Graph::setMutRate(double newRate) {
    mutationRate = newRate;
}

void Graph::setCrossRate(double newRate) {
    crossoverRate = newRate;
}

void Graph::setMutType(int newType) {
    mutationType = newType;
}

void Graph::genPrep() {
    population = new int *[populationSize];
    new_population = new int *[populationSize];
    for (int i = 0; i < populationSize; ++i) {
        population[i] = new int[next_ID];
        new_population[i] = new int[next_ID];
    }
    fitnessValues = new double[populationSize];
    currChild = new int[next_ID];
    tourPair = new bool[next_ID];
    globalBestP = new int[next_ID];
    globalBestW = MAXINT32;
    orig = true;
}

void Graph::GeneticAlgorithm(double time_in_Seconds) {
    created = true;
    genPrep();
    generateStartPopulation();//wygenerowanie pierwszej populacji
    StartCounter();
    int k = 0;
    while(GetCounter()<=time_in_Seconds){
        generateNewPopulation();//generowanie nowej populacji
    }
    std::cout<<k<<"\n";
}

void Graph::StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";
    PCFreq = double(li.QuadPart);
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double Graph::GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

void Graph::swap(int vortex1, int vortex2, int *path) {
    int pom = path[vortex1];
    path[vortex1] = path[vortex2];
    path[vortex2] = pom;
}

void Graph::insert(int vortex1, int vortex2, int *path) {

    int biggerIndex, smallerIndex;
    if (vortex1 > vortex2) {
        biggerIndex = vortex1;
        smallerIndex = vortex2;
    } else {
        biggerIndex = vortex2;
        smallerIndex = vortex1;
    }

    int pom = path[biggerIndex];
    for (int i = biggerIndex; i > smallerIndex; i--) {
        path[i] = path[i - 1];
    }
    path[smallerIndex] = pom;
}

void Graph::reverseInsert(int vortex1, int vortex2, int *path) {

    int biggerIndex, smallerIndex;
    if (vortex1 > vortex2) {
        biggerIndex = vortex1;
        smallerIndex = vortex2;
    } else {
        biggerIndex = vortex2;
        smallerIndex = vortex1;
    }

    int pom = path[smallerIndex];
    for (int i = smallerIndex; i < biggerIndex; i++) {
        path[i] = path[i + 1];
    }
    path[biggerIndex] = pom;
}

void Graph::invert(int vortex1, int vortex2, int *path) {
    int biggerIndex, smallerIndex;
    if (vortex1 > vortex2) {
        biggerIndex = vortex1;
        smallerIndex = vortex2;
    } else {
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

void Graph::copyPath(int *path1, int *path2) {
    for (int i = 0; i < next_ID; ++i) {
        path1[i] = path2[i];
    }
}

int Graph::calculateWeight(int *path) {
    int weight = 0;

    for (int i = 1; i < next_ID; ++i) {
        weight += WeightMatrix[path[i - 1]][path[i]];
    }
    weight += WeightMatrix[path[next_ID - 1]][0];
    return weight;
}

bool Graph::compareParents(int *parent1, int *parent2) {
    for (int i = 0; i < next_ID; ++i) {
        if (parent1[i] != parent2[i]) return false;
    }
    return true;
}


//selekcja turniejowa
int Graph::getParent() {
    std::uniform_int_distribution<> dist(0, populationSize - 1);

    int k = 2;//liczba rund turnieju
    int index = dist(gen);

    for (int i = 0; i < k; i++) {
        int random = dist(gen);//generowanie losowego indeksu (poza 0)
        if (fitnessValues[random] > fitnessValues[index]) //porownanie dopasowania osobnikow na indekasch
            index = random;
    }
    return index;
}

void Graph::createGenotype(int **population) {
    for (int i = 0; i < populationSize; i++) {
        int d = calculateWeight(population[i]);

        if (d < globalBestW) {
            globalBestW = d;
            copyPath(globalBestP,population[i]);
        }

        fitnessValues[i] = 1 / d;//dopasowanie - im mniejsza sciezka tym jest wieksze
    }

    double sum = 0.0;
    for (int i = 0; i < populationSize; i++)
        sum += fitnessValues[i];

    for (int i = 0; i < populationSize; i++)
        fitnessValues[i] = fitnessValues[i] / sum;//normalizacja wartosi dopasowania
}

void Graph::generateNewPopulation() {

    int index1;
    int index2;
    //orig zmienia sie na zmiane, zmienia tablice zapisywania populacji
    //usprawnia szybkosc dzialania algorytmu
    if(orig) {

        for (int i = 1; i < populationSize; i++) {//genmerowanie kolejnych potomkow

            index1 = getParent();//generowanie indeksow rodzicow
            index2 = getParent();

            while (compareParents(population[index1], population[index2])) {
                index2 = getParent();//algorytm upewnia sie ze rodzice nie sa jednakowi
            }

            Crossover(index1, index2,population);//szansa na krzyzowanie
            copyPath(new_population[i], currChild);

            switch (mutationType) {//szansa na mutacje
                case 1:
                default:
                    swapMutation(new_population[i]);
                    break;

                case 2:
                    insertMutation(new_population[i]);
                    break;

                case 3:
                    invertMutation(new_population[i]);
                    break;

            }
            if (calculateWeight(new_population[i]) < globalBestW) {//ustawianie w wypadku znalezienia nowej najlepszej sciezki
                globalBestW = calculateWeight(new_population[i]);
                copyPath(globalBestP, new_population[i]);
                time_found = GetCounter();
                std::cout << "Znaleziono nowe najlepsze rozwiazanie!! Czas: "<<GetCounter()<<"\n";
            }
        }
        copyPath(new_population[0], globalBestP);//elityzm
        createGenotype(new_population);//obliczanie dopasowania nowej generacji
        orig = false;
    }else{

        for (int i = 1; i < populationSize; i++) {

            index1 = getParent();
            index2 = getParent();

            while (compareParents(new_population[index1], new_population[index2])) {
                index2 = getParent();
            }

            Crossover(index1, index2,new_population);
            copyPath(population[i], currChild);

            switch (mutationType) {
                case 1:
                default:
                    swapMutation(population[i]);
                    break;

                case 2:
                    insertMutation(population[i]);
                    break;

                case 3:
                    invertMutation(population[i]);
                    break;

            }
            if (calculateWeight(population[i]) < globalBestW) {
                globalBestW = calculateWeight(population[i]);
                copyPath(globalBestP, population[i]);
                time_found = GetCounter();
                std::cout << "Znaleziono nowe najlepsze rozwiazanie!! Czas: "<<GetCounter()<<"\n";
            }
        }
        copyPath(population[0], globalBestP);//elityzm
        createGenotype(population);
        orig = true;
    }

}

void Graph::generateStartPopulation() {

    int *basePath = new int[next_ID];
    int *shuffledPath = new int[next_ID];
    for (int i = 0; i < next_ID; i++) {
        basePath[i] = i;
    }

    for (int i = 0; i < populationSize; i++) {
        copyPath(shuffledPath, basePath);
        shuffle(shuffledPath);
        copyPath(population[i], shuffledPath);
    }//tworzenie osobnikow pierwszej populajcji

    createGenotype(population);//obliczenie dopasowania pierwszej generacji
}

void Graph::shuffle(int *path) {
    for (int i = 1; i < next_ID; ++i) {
        std::uniform_int_distribution<int> distribution(1, i);
        int randomIndex = distribution(gen);
        int pom = path[randomIndex];
        path[randomIndex] = path[i];
        path[i] = pom;
    }
}

//order crossover
void Graph::Crossover(int parent1Index, int parent2Index, int** population) {
    std::uniform_int_distribution<> dist(1, next_ID - 1);
    std::uniform_real_distribution<double> rnd(0.0, 1.0);

    if (rnd(gen) < crossoverRate) {
        int r1 = dist(gen);//wygenerowanie przedzialu z 1 rodzica, do przeniesienia do potomka w calosci
        int r2 = dist(gen);

        for (int i = 0; i < next_ID; ++i) {
            currChild[i] = -1;
            tourPair[i] = false;//poczatkowo nie sa uzyte zadne wierzcholki
        }
        currChild[0] = 0;

        if (r1 > r2) {
            int pom = r1;
            r1 = r2;
            r2 = pom;
        }

        for (int i = r1; i <= r2; ++i) {//przekopiowanie przedzialu do potomka
            currChild[i] = population[parent1Index][i];
            tourPair[currChild[i]] = true;
        }
        for (int i = 1; i < r1; ++i) {//przenoszenie wierzcholkow przed przedzialem
            if (!tourPair[population[parent2Index][i]]) {
                currChild[i] = population[parent2Index][i];
                tourPair[currChild[i]] = true;
            } else {
                int pos = i;
                while (tourPair[population[parent2Index][pos]]) {
                    pos++;
                    if (pos == next_ID) pos = 1;
                }
                currChild[i] = population[parent2Index][pos];
                tourPair[currChild[i]] = true;
            }
        }
        for (int i = r2 + 1; i < next_ID; ++i) {//przenoszenie wierzcholkow za przedzialem
            if (!tourPair[population[parent2Index][i]]) {
                currChild[i] = population[parent2Index][i];
                tourPair[currChild[i]] = true;
            } else {
                int pos = i;
                while (tourPair[population[parent2Index][pos]]) {
                    pos++;
                    if (pos == next_ID) pos = 1;
                }
                currChild[i] = population[parent2Index][pos];
                tourPair[currChild[i]] = true;
            }
        }


    } else {
        copyPath(currChild, population[parent1Index]);//jezeli nie zachodzi krzyzowanie, potomek bedzie identyczny do rodzica 1
    }
}

void Graph::swapMutation(int *path) {
    //std::cout<<"Mutation!\n";
    std::uniform_int_distribution<> dist(1, next_ID - 1);
    std::uniform_real_distribution<double> rnd(0.0, 1.0);
    if (rnd(gen) > mutationRate) return;
    int vortex1 = dist(gen);
    int vortex2 = dist(gen);
    while (vortex1 == vortex2) vortex2 = dist(gen);
    swap(vortex1, vortex2, path);
}

void Graph::insertMutation(int *path) {
    std::uniform_int_distribution<> dist(1, next_ID - 1);
    std::uniform_real_distribution<double> rnd(0.0, 1.0);
    if (rnd(gen) > mutationRate) return;
    int vortex1 = dist(gen);
    int vortex2 = dist(gen);
    while (vortex1 == vortex2) vortex2 = dist(gen);
    insert(vortex1, vortex2, path);
}

void Graph::invertMutation(int *path) {
    std::uniform_int_distribution<> dist(1, next_ID - 1);
    std::uniform_real_distribution<double> rnd(0.0, 1.0);
    if (rnd(gen) > mutationRate) return;
    int vortex1 = dist(gen);
    int vortex2 = dist(gen);
    while (vortex1 == vortex2) vortex2 = dist(gen);
    invert(vortex1, vortex2, path);
}

void Graph::setPopSize(int newPopSize) {
    populationSize = newPopSize;
}

int Graph::getBestWeight() {
    return globalBestW;
}

int* Graph::getBestPath() {
    return globalBestP;
}

void Graph::findGreedy() {
    bool* matrixVisit = new bool [next_ID];
    for (int i = 0; i <next_ID; i ++){
        matrixVisit[i] = true;
    }
    greedyResult = new int[next_ID];
    greedyResult[0] = 0;
    matrixVisit[0] = false;
    greedy = 0;
    for (int i = 1; i < next_ID; ++i) {
        int index_best = -1;
        int weight_best = INT32_MAX;

        for (int j = 0; j < next_ID; ++j) {
            if (!matrixVisit[j]) continue;
            //if (i == 1) std::cout<< matrixAdj[ 0 ] [j]<<"\n";
            if ( WeightMatrix[ greedyResult[i-1] ] [j] < weight_best){
                index_best = j;
                weight_best = WeightMatrix[greedyResult[i-1]][j];
            }
        }
        greedyResult[i] = index_best;

        matrixVisit[index_best] = false;
        greedy += weight_best;
    }
    greedy += WeightMatrix[greedyResult[next_ID-1]][0];
}

void Graph::generateStartPopulationGreedy() {
    int *basePath = new int[next_ID];
    int *shuffledPath = new int[next_ID];
    findGreedy();
    copyPath(basePath,greedyResult);

    std::uniform_int_distribution<> dist(1, next_ID - 1);

    copyPath(population[0],basePath);
    for (int i = 1; i < populationSize; i++) {
        copyPath(shuffledPath, basePath);
        for (int j = 0; j < 10; ++j) {
            int r1 = dist(gen);
            int r2 = dist(gen);
            while (r1==r2) r2 = dist(gen);
            swap(r1,r2,shuffledPath);
        }
        copyPath(population[i], shuffledPath);
    }

    createGenotype(population);
}

void Graph::printTSP() {
    if (!created) return;
    std::cout<<"\nWaga najlepszej sciezki: "<<globalBestW<<"\n\n";
    for (int i = 0; i < next_ID; ++i) {
        std::cout<<globalBestP[i]<<" ";
    }std::cout<<"0\n\n";
}

