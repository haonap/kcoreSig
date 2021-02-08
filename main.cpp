#include <iostream>
#include "functions.h"

using namespace std;

int main(){
    string seqName;
    int tau, k, method;
    fstream fin("instance.txt", fstream::in); // read in input parameters
    fin >> seqName;
    fin >> tau;
    fin >> k;
    fin >> method;
    fin.close();

    ReadIn(seqName); // read in graph sequence

    if(method == 1){ // pick a method, 1 for GSIP-F2, 2 for MW-2CORE, 3 for MW-F2
        GSIP_F2(tau, k);
    }else{
        MW(tau, k, method);
    }

    return 0;
}