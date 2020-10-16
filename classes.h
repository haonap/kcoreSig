//
// Created by Hao Pan on 9/1/20.
//

#ifndef KCORESIG_CLASSES_H
#define KCORESIG_CLASSES_H


#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

class node{
public:
    int name;
    int degree;
    vector<int> neighbors;
    //constructor
    node(int);
};

class graph {
public:

    string name;
    int n, m;
    vector<node> nodeList;
    int maxDeg;
    vector<int> kCore;

    // constructor
    graph(string);

    // functions
    void GetKCore(int); // this function gets kCore of a single static graph without reordering vertices
    bool IsAdj(int, int); // this function checks if two nodes are adjacent
};


#endif //KCORESIG_CLASSES_H

