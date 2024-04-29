#include <iostream>
#include <vector>
#include "TabuSearch/TabuSearch.h"
#include "Graph/Graph.h"
#include "tinyxml2.h"
#include "SimulatedAnnealing/SimulatedAnnealing.h"
#include "fstream"

using std::cout;
using std::cin;

void graphMenu();

void TabuMenu(int **arr, int size, std::string fileName);

int **daneXML(char *Filename);

void SAMenu(int **arr, int size, std::string fileName);

void testy(int i);

void saveResult(int *path, int size, std::string graph_filename, std::string method, std::string algorithm);
void saveResult(int *path, int size, std::string graph_filename);

void saveResultCSV(int path_weight, int iteration, double time, std::string method, std::string algorithm,
                   std::string fileName);

int sizePom;

int main() {
    while (true) {
        system("cls");
        cout << "MENU\n";
        cout << "1 - Graph - Tabu and SM\n";
        cout << "2 - Wczytaj wynik\n";
        cout << "3 - EXIT\n";
        int a;
        cin >> a;
        switch (a) {
            case 1:
                system("cls");
                graphMenu();
                break;
            case 2: {
                system("cls");
                int **graph;
                std::string fileName;
                cout << "Podaj nazwe pliku: \n";
                cin >> fileName;
                if (fileName.substr(0,4) != "last") {
                    std::string b = fileName.substr(6);
                    b = b.substr(0, b.length() - 7);
                    b = b + ".xml";
                    char *filenameChar = new char[b.length() + 1];
                    std::strcpy(filenameChar, b.c_str());
                    graph = daneXML(filenameChar);
                }
                else{
                    std::string b = fileName.substr(11);
                    b = b.substr(0, b.length() - 4);
                    b = b + ".xml";
                    char *filenameChar = new char[b.length() + 1];
                    std::strcpy(filenameChar, b.c_str());
                    graph = daneXML(filenameChar);
                }
                std::ifstream file(fileName);
                int size;
                file>>size;
                int* path = new int[size];
                for (int i = 0; i < size; ++i) {
                    file>>path[i];
                }
                file.close();
                cout<<"Path: ";
                for (int i = 0; i < size; ++i) {
                    cout<<path[i]<<" ";
                }cout<<path[0]<<"\n";
                TabuSearch pom = TabuSearch(graph,size);
                cout<<"Weight calculated: " << pom.calculateWeight(path);
                delete[] graph;
                delete[] path;
                getchar();getchar();
            }
                break;

            case 3:
                exit(0);

            default:
                break;
        }
    }
}

void graphMenu() {
    Graph graph = Graph();
    std::string name;
    while (true) {
        system("cls");
        cout << "GRAPH MENU\n";
        cout << "1 - Tabu Search na aktualnym grafie\n";
        cout << "2 - Algorytm symulowanego wyrzazania na aktualnym grafie\n";
        cout << "3 - Wczytaj graf (plik XML)\n";
        cout << "4 - Wyswietl graf (lista sasiedztwa)\n";
        cout << "5 - Wyswietl graf (macierz sasiedztwa)\n";
        cout << "Opcje grafu w wypadku niewczytania z pliku\n";
        cout << "6 - Dodaj wierzcholki\n";
        cout << "7 - Wypelnij graf losowymi krawedziami\n";
        cout << "8 - Dodaj krawedz\n";
        cout << "9 - EXIT\n";
        cout << "0 - Testy\n";
        int b;
        cin >> b;
        switch (b) {
            case 1: {
                //if (graph.getGraph() == nullptr)
                graph.makeMatrix();
                TabuMenu(graph.getGraph(), graph.getVortexAmount(), name.substr(0, name.size() - 4));
            }
                break;

            case 2:
                graph.makeMatrix();
                SAMenu(graph.getGraph(), graph.getVortexAmount(), name.substr(0, name.size() - 4));
                break;

            case 3: {
                system("cls");
                cout << "Podaj nazwe pliku: \n";
                std::string a;
                cin >> a;
                name = a;
                char *filenameChar = new char[a.length() + 1];
                std::strcpy(filenameChar, a.c_str());
                int **graf = daneXML(filenameChar);
                graph.addMultipleVortices(sizePom);
                for (int i = 0; i < sizePom; ++i) {
                    for (int j = 0; j < sizePom; ++j) {
                        if (i != j)
                            graph.addEdge(Edge(i, j, graf[i][j]));
                    }
                }
            }
                break;

            case 4:
                graph.printGraphList();
                break;

            case 5:
                graph.printGraphMatrix();
                break;

            case 6: {
                cout << "Ile wierzcholkow: \n";
                int c;
                cin >> c;
                graph.addMultipleVortices(c);
            }
                break;

            case 7:
                graph.fillGraphWithEdgesRandom();
                break;

            case 8: {
                cout << "Zrodlo -> cel -> waga (krawedzi): \n";
                int e, r, t;
                cin >> e;
                cin >> r;
                cin >> t;
                graph.addEdge(Edge(e, r, t));
            }
                break;

            case 9:
                return;

            case 0:
                int i;
                cin >> i;
                testy(i);
                break;

            default:
                break;
        }
    }
}

void TabuMenu(int **arr, int size, std::string fileName) {
    TabuSearch tabu = TabuSearch(arr, size);
    double time = 30.0;
    int neigh = 1;
    bool flaga = false;
    int *res;
    int result;
    tabu.findGreedy();
    int *resultGreedy = tabu.greedyResult;
    int resGreedy = tabu.greedy;
    std::string method = "";
    while (true) {
        system("cls");
        cout << "TABU MENU\n";
        cout << "1 - Uruchom algorytm\n";
        cout << "2 - Zmien tryb sasiedztwa\n";
        cout << "3 - Zmien czas wykonywania\n";
        cout << "4 - Zapisz wynik\n";
        cout << "5 - EXIT\n\n\n";
        cout << "Wynik zachlanny : " << resGreedy << "\n";
        cout << "Sciezka zachlanny: ";
        for (int i = 0; i < size; ++i) {
            cout << resultGreedy[i] << " ";
        }
        cout << resultGreedy[0] << "\n";
        if (flaga) {
            cout << "Czas znalezienia: " << tabu.time_found << "\n";
            cout << "Znaleziony najlepszy wynik : " << result << "\n";
            cout << "Znaleziona najlepsza sciezka: ";
            for (int i = 0; i < size; ++i) {
                cout << res[i] << " ";
            }
            cout << res[0] << "\n\n\n";
        } else cout << "\n\n";
        cout << "Aktualny tryb sasiedztwa: ";
        switch (neigh) {
            case 1:
                cout << "Tryb Swap\n";
                break;
            case 2:
                cout << "Tryb Insert\n";
                break;
            case 3:
                cout << "Tryb Invert\n";
                break;
            default:
                cout << "Niepoprawny tryb sasiedztwa\n";
        }
        cout << "Aktualny czas wykonywania :" << time << "\n\n";

        int c;
        cin >> c;
        switch (c) {
            case 1:
                tabu.tabuPrep();
                tabu.searchTSP(time);
                res = tabu.getBestPath();
                result = tabu.getBestWeight();
                flaga = true;
                break;

            case 2: {
                system("cls");
                cout << "1 - Swap\n";
                cout << "2 - Insert\n";
                cout << "3 - Invert\n";
                int tryp;
                cin >> tryp;
                if (tryp == 1 || tryp == 2 || tryp == 3) {
                    neigh = tryp;
                    tabu.setNeighborhoodMode(tryp);
                } else cout << "Zly tryb!\n";

                if (tryp == 1) method = "Swap";
                else if (tryp == 2) method = "Insert";
                else if (tryp == 3) method = "Invert";
                else method = "ERROR";
            }
                break;

            case 3:
                system("cls");
                cout << "Podaj czas: \n";
                cin >> time;
                break;

            case 4:
                saveResult(res, size,fileName);
                break;

            case 5:
                delete[] res;
                delete[] resultGreedy;
                return;

            default:
                break;
        }
    }
}

void SAMenu(int **arr, int size, std::string fileName) {
    SimulatedAnnealing annealing = SimulatedAnnealing(arr, size);
    double time = 30.0;
    int neigh = 1;
    bool flaga = false;
    int *res;
    int result;
    annealing.findGreedy();
    int *resultGreedy = annealing.greedyResult;
    int resGreedy = annealing.greedy;
    std::string method = "";
    while (true) {
        system("cls");
        cout << "TABU MENU\n";
        cout << "1 - Uruchom algorytm\n";
        cout << "2 - Zmien tryb schladzania\n";
        cout << "3 - Zmien czas wykonywania\n";
        cout << "4 - Zapisz wynik\n";
        cout << "5 - EXIT\n\n\n";
        cout << "Wynik zachlanny : " << resGreedy << "\n";
        cout << "Sciezka zachlanny: ";
        for (int i = 0; i < size; ++i) {
            cout << resultGreedy[i] << " ";
        }
        cout << resultGreedy[0] << "\n";
        if (flaga) {
            cout << "Czas znalezienia: " << annealing.time_found << "\n";
            cout << "Znaleziony najlepszy wynik : " << result << "\n";
            cout << "Znaleziona najlepsza sciezka: ";
            for (int i = 0; i < size; ++i) {
                cout << res[i] << " ";
            }
            cout << res[0] << "\n\n\n";
        } else cout << "\n\n";
        cout << "c = 0.001\n";
        cout << "T(i+1) = T(i) * a\n";
        cout << "Aktualny tryb schladzania: ";
        switch (neigh) {
            case 1:
                cout << "Schladzanie eksponencjalne : a = 1 - c\n";
                break;
            case 2:
                cout << "Schladzanie odwrotne logarytmicznie : a = T(i)/( 1 + T(i) * c)\n";
                break;
            case 3:
                cout << "3 - Schladzanie logarytmiczne na podstawie temperatury startowej:\n"
                        "T(i) nie jest mnozone przez a, wynosi: T(i) = T(0)*c/log(1 + k)\n";
                break;
            default:
                cout << "Niepoprawny tryb schladzania\n";
        }
        cout << "Aktualny czas wykonywania :" << time << "\n\n";

        int c;
        cin >> c;
        switch (c) {
            case 1:
                annealing.annealPrep();
                annealing.searchTSP(time);
                res = annealing.getBestPath();
                result = annealing.getBestWeight();
                flaga = true;
                break;

            case 2: {
                system("cls");
                cout << "c = 0.001\n";
                cout << "T(i+1) = T(i) * a\n";
                cout << "k - ilosc iteracji";
                cout << "1 - Schladzanie eksponencjalne : a = 1 - c\n";
                cout << "2 - Schladzanie odwrotne logarytmicznie : a = T(i)/( 1 + T(i) * c)\n";
                cout << "3 - Schladzanie logarytmiczne na podstawie temperatury startowej:\n"
                        "T(i) nie jest mnozone przez a, wynosi: T(i) = T(0)*c/log(1 + k)\n";
                int tryp;
                cin >> tryp;
                if (tryp == 1 || tryp == 2 || tryp == 3) {
                    neigh = tryp;
                    annealing.setCoolingSchedule(tryp);
                } else cout << "Zly tryb!\n";

                if (tryp == 1) method = "Exponential";
                else if (tryp == 2) method = "InverseLog";
                else if (tryp == 3) method = "LogBOST";//Log-Based-On-Starting-Temp
                else method = "ERROR";
            }
                break;

            case 3:
                system("cls");
                cout << "Podaj czas: \n";
                cin >> time;
                break;

            case 4:
                saveResult(res, size,fileName);
                break;

            case 5:
                delete[] res;
                delete[] resultGreedy;
                return;

            default:
                break;
        }
    }
}

void testy(int i) {
    std::string a = "";
    std::string b = "";
    double time;
    switch (i) {
        case 0:
            a = "ftv55.xml";
            b = "ftv55";
            break;
        case 1:
            a = "ftv170.xml";
            b = "ftv170";
            break;
        case 2:
            a = "rbg358.xml";
            b = "rbg358";
            break;
    }
    switch (6) {
        case 0:
            time = 0;
            break;
        case 1:
            time = 5;
            break;
        case 2:
            time = 10;
            break;
        case 3:
            time = 15;
            break;
        case 4:
            time = 20;
            break;
        case 5:
            time = 25;
            break;
        case 6:
            time = 30;
            break;
    }
    char *filenameChar = new char[a.length() + 1];
    std::strcpy(filenameChar, a.c_str());
    int **graf = daneXML(filenameChar);
    TabuSearch tabu = TabuSearch(graf, sizePom);
    int *best_path_tabu = new int[sizePom];
    int best_weight_tabu = INT32_MAX;
    for (int j = 1; j < 4; ++j) {
        int *best_path_tabu_method = new int[sizePom];
        int best_weight_tabu_method = INT32_MAX;
        double best_weight_time;
        tabu.setNeighborhoodMode(j);

        tabu.tabuPrep();
        tabu.searchTSP(time);
        if (best_weight_tabu_method > tabu.getBestWeight()) {
            best_weight_tabu_method = tabu.getBestWeight();
            tabu.copyPath(best_path_tabu_method, tabu.getBestPath());
            if (best_weight_tabu_method < best_weight_tabu) {
                best_weight_tabu = best_weight_tabu_method;
                tabu.copyPath(best_path_tabu, best_path_tabu_method);
                best_weight_time = tabu.time_found;
            }
            saveResultCSV(tabu.getBestWeight(), 0, time, std::to_string(j), "_TS", b);
        }
    }
    //saveResult(best_path_tabu, sizePom, b, "_TS", "");

    SimulatedAnnealing annealing = SimulatedAnnealing(graf, sizePom);
    int *best_path_anneal = new int[sizePom];
    int best_weight_anneal = INT32_MAX;
    for (int j = 1; j < 4; ++j) {
        int *best_path_anneal_method = new int[sizePom];
        int best_weight_anneal_method = INT32_MAX;
        double best_weight_time;
        annealing.setCoolingSchedule(j);

        annealing.annealPrep();
        annealing.searchTSP(time);
        if (best_weight_anneal_method > annealing.getBestWeight()) {
            best_weight_anneal_method = annealing.getBestWeight();
            annealing.copyPath(best_path_anneal_method, annealing.getBestPath());
            if (best_weight_anneal_method < best_weight_anneal) {
                best_weight_anneal = best_weight_anneal_method;
                annealing.copyPath(best_path_anneal, best_path_anneal_method);
                best_weight_time = annealing.time_found;
            }

            saveResultCSV(annealing.getBestWeight(), 0, time, std::to_string(j), "_SA", b);
        }
    }
    //saveResult(best_path_anneal, sizePom, b, "_SA", "");
}

int **daneXML(char *Filename) {
    std::vector<std::vector<double>> adjacencyMatrix;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(Filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << Filename << std::endl;
        exit(0);
    }
    tinyxml2::XMLElement *root = doc.FirstChildElement("travellingSalesmanProblemInstance");
    if (!root) {
        std::cerr << "Invalid XML format: Root element 'travellingSalesmanProblemInstance' not found." << std::endl;
        exit(0);
    }
    tinyxml2::XMLElement *graphElement = root->FirstChildElement("graph");
    if (!graphElement) {
        std::cerr << "Invalid XML format: 'graph' element not found." << std::endl;
        exit(0);
    }
    for (tinyxml2::XMLElement *vertexElement = graphElement->FirstChildElement(
            "vertex"); vertexElement; vertexElement = vertexElement->NextSiblingElement("vertex")) {
        std::vector<double> row;
        for (tinyxml2::XMLElement *edgeElement = vertexElement->FirstChildElement(
                "edge"); edgeElement; edgeElement = edgeElement->NextSiblingElement("edge")) {
            const char *costStr = edgeElement->Attribute("cost");
            if (!costStr) {
                std::cerr << "Invalid XML format: 'cost' attribute not found in 'edge' element." << std::endl;
                exit(0);
            }
            double cost = std::stod(costStr);
            row.push_back(cost);
        }
        adjacencyMatrix.push_back(row);
    }
    int **adjacencyMatrix2 = new int *[adjacencyMatrix[0].size()];
    sizePom = adjacencyMatrix[0].size();
    for (int i = 0; i < adjacencyMatrix[0].size(); ++i) {
        adjacencyMatrix2[i] = new int[adjacencyMatrix[0].size()];
    }
    for (int i = 0; i < adjacencyMatrix[0].size(); ++i) {
        for (int j = 0; j < adjacencyMatrix[0].size(); ++j) {
            adjacencyMatrix2[i][j] = (int) adjacencyMatrix[i][j];
            //if (adjacencyMatrix2[i][j] == 0)
            //adjacencyMatrix2[i][j] = INT32_MAX;
        }
    }
    return adjacencyMatrix2;
}

void saveResult(int *path, int size, std::string graph_filename, std::string method, std::string algorithm) {
    std::string name = "result" + graph_filename + method + algorithm + ".txt";
    std::ofstream outputFile(name);

    if (outputFile.is_open()) {
        outputFile << size << "\n";
        for (int i = 0; i < size; ++i) {
            outputFile << path[i] << "\n";
        }
        outputFile << path[0];
        outputFile.close();
    } else {
    }
}

void saveResultCSV(int path_weight, int iteration, double time, std::string method, std::string algorithm,
                   std::string fileName) {
    std::string name =
            "result_" + fileName + "_" + algorithm + "_" + method + "_" + std::to_string((int) time) + ".csv";
    std::ofstream outputFile(name);
    if (outputFile.is_open()) {
        outputFile << "weight;" << "time;\n";
        outputFile << path_weight << ";" << time << ";\n";
        outputFile.close();
    } else {
    }
}

void saveResult(int *path, int size, std::string graph_filename){
    std::string name = "last_result"+ graph_filename +".txt";
    std::ofstream outputFile(name);

    if (outputFile.is_open()) {
        outputFile << size << "\n";
        for (int i = 0; i < size; ++i) {
            outputFile << path[i] << "\n";
        }
        outputFile << path[0];
        outputFile.close();
    } else {
    }
}