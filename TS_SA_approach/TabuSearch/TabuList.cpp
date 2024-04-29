//
// Created by micha on 28.11.2023.
//

#include <iostream>
#include "TabuList.h"

TabuList::TabuList(int size, int cadence){
    this->tabuArr = new int*[size];
    this->cadence = cadence;
    for (int i = 0; i < size; ++i) {
        tabuArr[i] = new int[size];
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            tabuArr[i][j] = 0;
        }
    }
    this->size = size;
}

void TabuList::AddTabu(int i, int j) {
    tabuArr[i][j] += cadence;
}

void TabuList::AddTabu(int i, int j, int newCadence) {
    tabuArr[i][j] += newCadence;
}

void TabuList::decrementTabu() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (tabuArr[i][j] > 0){
                tabuArr[i][j]--;
            }
        }
    }
}

bool TabuList::isTabu(int i, int j) {

    if (tabuArr[i][j] == 0) return false;
    return true;
}

TabuList::TabuList() {

}

TabuList::~TabuList() {
    delete[] tabuArr;
}
