//
// Created by micha on 07.10.2023.
//

#ifndef PEAPROJECT_EDGE_H
#define PEAPROJECT_EDGE_H


class Edge {
public:
        int source;
        int destination;
        int capacity;
        Edge(int src, int dest, int cap);
        Edge();
};


#endif //PEAPROJECT_EDGE_H
