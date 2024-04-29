//
// Created by micha on 02.01.2024.
//

#include "Edge.h"

Edge::Edge(int src, int dest, int cap) {
    this->source = src;
    this->destination = dest;
    this->capacity = cap;
}

Edge::Edge() {
}
