#include <iostream>
#include <vector>
#include <fstream>
#include "tinyxml2.h"
#include "Graph/Graph.h"

int sizePom = 0;

using std::cout;
using std::cin;

int **daneXML(char *Filename);
void graphMenu();
void testy();
int main() {
    while (true) {
        system("cls");
        cout << "MENU\n";
        cout << "1 - Graph - Genetic Algorithm\n";
        cout << "2 - EXIT\n";
        int a;
        cin >> a;
        switch (a) {
            case 1:
                system("cls");
                graphMenu();
                break;
            case 2:
                exit(0);

            default:
                break;
        }

    }

    return 0;
}
void graphMenu() {
    Graph graph = Graph();
    std::string name;
    double timeins = 60;
    while (true) {
        system("cls");
        cout << "GRAPH MENU\n";
        cout << "Wspolczynniki:\n"<<"Mutacji: "<<graph.mutationRate<<"\nKrzyzowania: "<<graph.crossoverRate;
        cout <<"\nWielkosc populacji: "<<graph.populationSize<<"\n";
        cout <<"Metoda krzyzowania: Order (Kolejonsci)\n";
        cout <<"Medota mutacji: ";
        switch (graph.mutationType) {
            case 1:
            default:
                cout<<"Swap\n";
                break;
            case 2:
                cout<<"Insert\n";
                break;
            case 3:
                cout<<"Invert\n";
                break;
        }
        cout<<"Aktualny czas wykonywania: "<<timeins<<"\n";
        graph.printTSP();
        cout << "1 - Algorytm\n";
        cout << "2 - Ustaw czas wykonywania\n";
        cout << "3 - Ustaw wielkosc populacji\n";
        cout << "4 - Ustaw wspolczynnik mutacji\n";
        cout << "5 - Ustaw wspolczynnik krzyzowania\n";
        cout << "6 - Wybierz metode mutacji\n";
        cout << "7 - Wczytaj graf (plik XML)\n";
        cout << "8 - Wyswietl graf (lista sasiedztwa)\n";
        cout << "9 - Wyswietl graf (macierz sasiedztwa)\n";
        cout << "Opcje grafu w wypadku niewczytania z pliku\n";
        cout << "10 - Dodaj wierzcholki\n";
        cout << "11 - Wypelnij graf losowymi krawedziami\n";
        cout << "12 - Dodaj krawedz\n";
        cout << "13 - Usun graf\n";
        cout << "14 - EXIT\n";
        cout << "0 - Testy\n";
        int b;
        cin >> b;
        switch (b) {
            case 1:
                graph.GeneticAlgorithm(timeins);
                break;

            case 2:{
                cout<<"Podaj czas: \n";
                cin>>timeins;
            }
                break;
            case 3:{
                double a;
                cout<<"Podaj nowa wielkosc populacji: \n";
                cin>>a;
                graph.setPopSize(a);
            }
                break;
            case 4:{
                double a;
                cout<<"Podaj nowa wielkosc populacji: \n";
                cin>>a;
                graph.setMutRate(a);
            }
                break;
            case 5: {
                int a;
                cout << "Podaj nowa wielkosc populacji: \n";
                cin >> a;
                graph.setCrossRate(a);
            }
                break;
            case 6:{
                int a;
                cout << "Podaj nowa metode mutacji: \n";
                cout << "1 - Swap\n2 - Insert\n3 - Invert\n";
                cin >> a;
                graph.setMutRate(a);
            }
                break;
            case 7:{
                cout << "Podaj nazwe pliku: \n";
                std::string a = "rbg403.xml";
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
                graph.makeMatrix();
            }
                break;

            case 8:
                system("cls");
                graph.printGraphList();
                getchar();getchar();
                break;

            case 9:
                system("cls");
                graph.printGraphMatrix();
                getchar();getchar();
                break;

            case 10:{
                system("cls");
                cout << "Ile wierzcholkow: \n";
                int c;
                cin >> c;
                graph.addMultipleVortices(c);
            }
                break;

            case 11:
                graph.fillGraphWithEdgesRandom();
                break;

            case 12:{
                cout << "Zrodlo -> cel -> waga (krawedzi): \n";
                int e, r, t;
                cin >> e;
                cin >> r;
                cin >> t;
                graph.addEdge(Edge(e, r, t));
            }
                break;

            case 13:
                graph.deleteGraph();
                break;

            case 14:
                return;

            case 15:
                testy();
        }
    }
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

void testy(){
    cout << "Podaj nazwe pliku: \n";
    std::string a = "rbg403.xml";

    for(int q = 0; q < 3; q++) {
        switch (q) {
            case 0:
                a = "ftv47.xml";
                break;
            case 1:
                a = "ftv170.xml";
                break;
            case 2:
                a = "rbg403.xml";
                break;
            default:
                return;
        }
        double czas = 0.0;
        if (a == "ftv47.xml" || a == "ftv55.xml") czas = 120.0;
        else if (a == "ftv170.xml") czas = 180.0;
        else czas = 240.0;
        int populacje[]{20, 100, 1000};
        double czasy[]{5.0, 10.0, 15.0,20.0,25.0,30.0};
        for (int i = 0; i < 2; ++i) {
            std::string name = "result_" + std::to_string((int) i) + "_" + a.substr(0, a.length() - 4) + "fTime" ".csv";
            std::ofstream outputFile(name);
            outputFile << "weight;time;mutation;popsize;index\n";
            for (int j = 0; j < 3; ++j) {
                for (int l = 0; l < 1; ++l) {
                    Graph graph = Graph();
                    char *filenameChar = new char[a.length() + 1];
                    std::strcpy(filenameChar, a.c_str());
                    int **graf = daneXML(filenameChar);
                    graph.addMultipleVortices(sizePom);
                    for (int o = 0; o < sizePom; ++o) {
                        for (int p = 0; p < sizePom; ++p) {
                            if (o != p)
                                graph.addEdge(Edge(o, p, graf[o][p]));
                        }
                    }
                    graph.makeMatrix();
                    graph.setPopSize(populacje[j]);
                    graph.setMutType(i + 1);
                    graph.GeneticAlgorithm(0);
                    outputFile << graph.getBestWeight() << ";" << czas << ";" << i + 1 << ";" << populacje[j] << ";"
                               << l << "\n";
                }
            }
            outputFile.close();
        }
    }
}
