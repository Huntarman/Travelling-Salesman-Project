//
// Created by micha on 28.11.2023.
//

#ifndef PEA_PROJECT_2_TABULIST_H
#define PEA_PROJECT_2_TABULIST_H


class TabuList {
private:
    int** tabuArr;
    int cadence;
    int size;
public:
    TabuList();
    ~TabuList();
    TabuList(int size, int cadence);
    void AddTabu(int i, int j);
    void AddTabu(int i, int j, int newCadence);
    void decrementTabu();
    bool isTabu(int i, int j);
};


#endif //PEA_PROJECT_2_TABULIST_H
