//
// Created by micha on 07.10.2023.
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
    this->TSP_path = nullptr;
    this->TSP_path_best = nullptr;
    this->TSP_path_weight = 0;
    this->TSP_path_index = 0;
    this->TSP_path_weight_best = INT32_MAX;
    this->WeightMatrix = nullptr;
    this->memoArr = nullptr;
    all_visited = 0;
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

void Graph::TSP(int index) {
    // zapamietywanie aktualnej sciezki
    TSP_path[TSP_path_index] = index;
    TSP_path_index++;

    // sprawdzenie czy nie zostaly odwiedzone juz wszystkie krawedzie
    if(TSP_path_index < next_ID){
        vortices[index].visited = true;//ustawienie stanu wierzcholka aktualnie odwiedzanego na odwiedzony
        //szukanie nieodwiedzonego wierzcholka
        for (int i = 0; i < next_ID; ++i) {

            if(!vortices[i].visited){
                TSP_path_weight += WeightMatrix[index][i];//sledzenie wagi aktualnej sciezki
                //if (TSP_path_weight < TSP_path_weight_best)//przyspiesza bardzo funkcje, ale nie robi przegladu wszystkich permutacji
                    TSP(i);//rekurencyjne wywolanie funkcji, aby sprawdzik kolejny ineodwiedzony wierzcholek
                TSP_path_weight -= WeightMatrix[index][i];
            }

        }
        //ustawienie stanu na nieodwiedzony
        vortices[index].visited = false;
    }
    else{
        // jesli wszystkie wierzcholki odwiedzono program sprawdza czy sciezka nie jest lepsza od aktualnej najlepszej
        TSP_path_weight += WeightMatrix[index][origin_index];
        if (TSP_path_weight < TSP_path_weight_best){
            //jesli jest, zostaje zmieniona
            for (int i = 0; i < next_ID; ++i) {
                TSP_path_best[i] = TSP_path[i];
            }
            TSP_path_weight_best = TSP_path_weight;
        }
        TSP_path_weight -= WeightMatrix[index][origin_index];
    }
    TSP_path_index--;
}
void Graph::TSP() {
    TSPcreated = true;// program moze wyswietlac TSP, gdy ta funkcja zakonczy dzialanie
    origin_index = 0;// nadpisanie pomocniczej, program zapamietuje startowy indeks
    TSP(0);//wywolanie funkcji
}

void Graph::TSP_prep(){
    TSP_path_weight_best = INT32_MAX;
    TSP_path_best = new int[next_ID];
    TSP_path = new int[next_ID];
    WeightMatrix = new int*[next_ID];
    for (int i = 0; i < next_ID; ++i) {
        vortices[i].sortEdgesByDest();
        WeightMatrix[i] = new int[next_ID];
    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i==j) WeightMatrix[i][j] = INT32_MAX;
            else WeightMatrix[i][j] = vortices[i].getEdge((j<i)? j:j-1).capacity;
        }
    }
}

void Graph::printTSP() {
    if (!TSPcreated) return;
    std::cout<<"Best path weight: "<<TSP_path_weight_best<<std::endl;
    std::cout<<"Path: ";
    for (int i = 0; i < next_ID; ++i) {
        std::cout<<TSP_path_best[i]<<", ";
    }
    std::cout<<TSP_path_best[0]<<"\n";
}


int Graph::TSP_DP(int position, int mask) {
    if (mask == all_visited) {
        return WeightMatrix[position][origin_index];
    }//jesli odwiedzone zostały wszystkie wierzcholki, funkcja zwraca wartosc wagi krawedzi z terazniejszego do poczatkowego wierzcholka
    if (memoArr[position][mask] != -1){
        return memoArr[position][mask];
    }//jesli waga obecnej kombinacji wierzcholkow zostala juz obliczona, funkcja zwraca te wage
    int cost = INT32_MAX;//oryginalny maksymalny koszt ustawiony zostaje na duza wartosc
    for (int i = 0; i < next_ID; ++i) {// petla szukajaca kolejnego nieodwiedzonego wierzcholka
        if ((mask&(1 << i)) == 0) {//jesli operacja bitowa and zwroci zero, wierzcholek i nie zostal jeszcze odwiedzony
            int a = TSP_DP(i, mask | (1 << i)); // rekurencja, z nowym argumentem pozycji i maski
            int weight = WeightMatrix[position][i] + a; //dodanie wagi krawedzi wyniku kombinacji wierzcholkow i wagi krawedzi z obecnej pozycji
                                                        //do znalezionego nieodwiedzonego wierzcholka
            cost = std::min(cost, weight); //wybor najmniejszej z: aktualnej najlepszej wagi oraz znalezionej wagi
        }
    }
    memoArr[position][mask] = cost; // zapis wyniku wagi aktualnej kombinacji
    return cost;
}

void Graph::TSP_DP(int starting_index) {
    TSPcreated = true;
    origin_index = starting_index;//zapamietanie początkowego indeksu
    int weight = TSP_DP(starting_index,1 << starting_index);//waga - wynik funkcji wlasciwej
    if (weight < TSP_path_weight_best) {
        TSP_path_weight_best = weight;
    }
    findPath(starting_index,1 << starting_index);//funkcja znajdujaca droge
}

void Graph::findPath(int position, long mask) {
    TSP_path_best[TSP_path_index++] = position;//dodanie do sciezki poczatkowego wierzcholka
    while(true){
        int index = -1;//zmienna, ktora nie moze byc indeksem wierzcholka, na poczatku kazdej iteracji
        int cost = INT32_MAX;
            for (int i = 0; i < next_ID; ++i) {//szukanie nastepnego nieodwiedzonego wierzcholka
                if ((mask&(1 << i)) == 0) {
                    int maskNew = mask | (1 << i);
                    int a = WeightMatrix[position][i] + memoArr[i][maskNew];
                    if(a < cost){//petla szuka dla kazdego wierzcholka kombinacji, o najmniejszej wadze sciezki, przechodzac zawsze o jeden do przodu
                        cost = a;
                        index = i;
                    }
                }
            }
        TSP_path_best[TSP_path_index++] = index;
        mask = mask | (1<<index);//zmiana maski
        if (mask == all_visited) return;
    }
}

void Graph::TSP_DP_prep() {
    all_visited = (1 << next_ID) - 1;
    TSP_path = new int[next_ID];
    TSP_path_best = new int[next_ID];
    TSP_path_index = 0;
    TSP_path_weight_best = INT32_MAX;
    WeightMatrix = new int*[next_ID];
    memoArr = new int*[next_ID];
    for (int i = 0; i < next_ID; ++i) {
        vortices[i].sortEdgesByDest();
        WeightMatrix[i] = new int[next_ID];
        memoArr[i] = new int[1 << next_ID];
    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < 1 << next_ID; ++j) {
            memoArr[i][j] = -1;
        }
    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i==j) WeightMatrix[i][j] = INT32_MAX;
            else WeightMatrix[i][j] = vortices[i].getEdge((j<i)? j:j-1).capacity;
        }
    }

}

Graph::~Graph() {
    for (int i = 0; i < next_ID; ++i) {
        vortices[i].deleteEdges();
    }
    delete[] WeightMatrix;
    delete[] memoArr;
    delete[] vortices;
    delete[] TSP_path;
    delete[] TSP_path_best;
    next_ID = 0;
}

int Graph::getVortexAmount() {
    return next_ID;
}


/*void Graph::TSP_BNB() {
    int* minRows =(int*) malloc(next_ID * sizeof(int));
    for (int i = 0; i < next_ID; ++i) {
        minRows[i] = findMinInRow(i);
    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i != j){
                WeightMatrix[i][j] -= minRows[i];
            }
        }
    }
    int* minColumns =(int*) malloc(next_ID * sizeof(int));
    for (int i = 0; i < next_ID; ++i) {
        minColumns[i] = findMinInColumn(i);
    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if (i != j){
                WeightMatrix[j][i] -= minColumns[i];
            }
        }
    }
    lowerBound += sumOfArray(minColumns,next_ID);
    lowerBound += sumOfArray(minRows,next_ID);
    free(minRows);free(minColumns);
    ZerosOnWeightMatrix =

}

void Graph::TSP_BNB_starter() {
    WeightMatrix = (int**) malloc(next_ID*sizeof(int*));
    for (int i = 0; i < next_ID; ++i) {
        WeightMatrix[i] = (int*)malloc(next_ID * sizeof(int));
    }
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if(i == j) WeightMatrix[i][j] = INT32_MAX;
            else WeightMatrix[i][j] = vortices[i].getEdge((j<i>)? j:j-1).capacity;
        }
    }
    TSP_path_best = (int*) malloc(next_ID * sizeof(int));;
}

int Graph::findMinInRow(int index) {
    int min = WeightMatrix[index][0];
    for (int j = 1; j < next_ID; ++j) {
        if (WeightMatrix[index][j] < min) {
            min = WeightMatrix[index][j];
        }
    }
    return min;
}

int Graph::findMinInColumn(int index) {
    int min = WeightMatrix[0][index];
    for (int j = 1; j < next_ID; ++j) {
        if (WeightMatrix[j][index] < min) {
            min = WeightMatrix[j][index];
        }
    }
    return min;
}

void Graph::printWeightMatrix() {
    for (int i = 0; i < next_ID; ++i) {
        for (int j = 0; j < next_ID; ++j) {
            if(i == j) std::cout<<"-\t";
            else std::cout<<WeightMatrix[i][j]<<"\t";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";
}

int Graph::sumOfArray(int *array, int size) {
    int sum = 0;
    for (int i = 0; i < size;++i) {
        sum += array[i];
    }
    return sum;
}*/
