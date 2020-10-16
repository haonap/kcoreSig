#include <iostream>
#include "functions.h"

using namespace std;

int main(){
    string seqName;
    int tau, k, method;
    fstream fin("instance.txt", fstream::in);
    fin >> seqName;
    fin >> tau;
    fin >> k;
    fin >> method; // 1 for GSIP-F2, 2 for MW
    fin.close();

    ReadIn(seqName);

    if(method == 1){
        GSIP_F2(tau, k);
    }else if(method == 2){
        MW(tau, k);
    }

    return 0;
}