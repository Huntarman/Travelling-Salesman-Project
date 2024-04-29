//
// Created by micha on 18.11.2023.
//

#include "Edge.h"

Edge::Edge(int src, int dest, int cap) {
    this->source = src;
    this->destination = dest;
    this->capacity = cap;
}

Edge::Edge() {
}