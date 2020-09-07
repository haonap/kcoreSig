#include <iostream>
#include "classes.h"
using namespace std;

graph::graph(string nameG){

    name = nameG;
    ifstream fin(name);
    string buf;
    int u, v;
    fin >> buf;
    fin >> n;
    fin >> buf;
    fin >> m;

    for(int i = 0; i < n; i++){
        node pNode(i);
        nodeList.push_back(pNode);
    }

    while(fin >> buf){
        if(buf == "e"){
            fin >> u;
            fin >> v;
            nodeList[u - 1].neighbors.push_back(v - 1);
            nodeList[v - 1].neighbors.push_back(u - 1);
        }
    }

    maxDeg = 0;
    for(int i = 0; i < n; i++){
        nodeList[i].degree = (int)nodeList[i].neighbors.size();
        if(nodeList[i].neighbors.size() > maxDeg){
            maxDeg = (int)nodeList[i].neighbors.size();
        }
    }
}


void graph::GetKCore(int k){
    queue<int> Q;
    vector<int> F, D;
    kCore.clear();

    for(int i = 0; i < n; i++){
        D.push_back((int)nodeList[i].neighbors.size());
        if(D[i] < k){
            F.push_back(1);
            Q.push(i);
        }else{
            F.push_back(0);
        }
    }

    while (Q.size()) {
        int v = Q.front();
        Q.pop();
        if(D[v] > 0){
            D[v] = 0;
            for(int u = 0; u < nodeList[v].neighbors.size(); u++){
                int pNeighbor = nodeList[v].neighbors[u];
                if(D[pNeighbor] > 0){
                    D[pNeighbor]--;
                    if(D[pNeighbor] < k){
                        if(F[pNeighbor] == 0){
                            Q.push(pNeighbor);
                            F[pNeighbor] = 1;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < n; i++){
        if(D[i] > 0){
            kCore.push_back(i);
        }
    }
}


bool graph::IsAdj(int u, int v){
    if(binary_search(nodeList[u].neighbors.begin(), nodeList[u].neighbors.end(), v)){
        return 1;
    }else{
        return 0;
    }
}


node::node(int nameN){
    name = nameN;
}

