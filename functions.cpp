#include "functions.h"


// containers
vector<graph> graphSeq;
vector<string> graphs;


// functions
void ReadIn(string name){
    double st = get_wall_time();
    cout << "\nRead in..." << endl;

    string fileName = "./graphSequences/" + name + "/" + "info.txt";

    string temp;
    ifstream fin(fileName);

    while(fin >> temp){
        graphs.push_back(temp);
    }

    for(int i = 0; i < graphs.size(); i++) {
        string fileName = "./graphSequences/" + name + "/" + graphs[i];
        graph pGraph(fileName);
        graphSeq.push_back(pGraph);
    }

    cout << "Instance: " << name << endl;
    cout << "Length of sequence: " << graphs.size() << endl;
    cout << "Processing time: " <<  fixed << setprecision(2) << (get_wall_time() - st) << " sec\n";
}


void FindPersistentKCore(int tau, int k){
    double st = get_wall_time();
    int T = (int)graphSeq.size();
    cout << "\nFind " << tau << "-persistent " << k << "-core " << "T = " << graphs.size() << ", tau = " << tau << "..." << endl;
    vector<int> bestKCore;
    int bestWindow = 0;

    for(int i = 0; i < T - tau + 1; i++){
        for(int j = i; j < i + tau; j++){
            for(int v = 0; v < graphSeq[j].n; v++){
                graphSeq[j].nodeList[v].degree = (int)graphSeq[j].nodeList[v].neighbors.size();
            }
        }
        vector<int> pKCore = GetPersistentKCoreWindow(i, tau, k);
        cout << "Window " << i+1 << ": " << (int)pKCore.size() << endl;
        if(pKCore.size() > bestKCore.size()){
            bestKCore = pKCore;
            bestWindow = i;
        }
    }

    cout << "Best persistent core found: ";
    for(int i = 0; i < bestKCore.size(); i++){
        cout << bestKCore[i] + 1 << " ";
    }
    cout << endl;
    cout << "Core size: " << (int)bestKCore.size() << endl;
    cout << "Best window: " << bestWindow + 1 << " (";
    for(int i = bestWindow; i < bestWindow + tau; i++){
        cout << "G" << i + 1;
        if(i < bestWindow + tau - 1){
            cout << " ";
        }
    }
    cout << ")" << endl;
    cout << "Processing time: " <<  fixed << setprecision(2) << (get_wall_time() - st) << " sec\n";
}


vector<int> GetPersistentKCoreWindow(int i, int tau, int k){
    vector<int> pstKCore; // persistent kCore
    queue<int> Q;
    vector<int> F;
    vector<vector<int>> D(tau);

    for(int v = 0; v < graphSeq[i].n; v++){
        F.push_back(0);
        for(int j = i; j < i + tau; j++){
            D[j - i].push_back((int)graphSeq[i].nodeList[v].neighbors.size());
            if(F[v] == 0){
                if(D[j - i][v] < k){
                    Q.push(v);
                    F[v] = 1;
                }
            }
        }
    }

    while(Q.size()){
        int v = Q.front();
        Q.pop();
        for(int j = i; j < i + tau; j++){
            if(D[j - i][v] > 0){
                D[j - i][v] = 0;
                for(int u = 0; u < graphSeq[j].nodeList[v].neighbors.size(); u++){
                    int pNeighbor = graphSeq[j].nodeList[v].neighbors[u];
                    if(D[j - i][pNeighbor] > 0){
                        D[j - i][pNeighbor]--;
                        if(D[j - i][pNeighbor] < k){
                            if(F[pNeighbor] == 0){
                                Q.push(pNeighbor);
                                F[pNeighbor] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    int count = 0;
    for(int v = 0; v < graphSeq[i].n; v++){
        if(D[0][v] > 0){
            pstKCore.push_back(v);
        }
    }
    return pstKCore;
}


string itos_c(int i){
    stringstream s;
    s << i;
    return s.str();
}


int stoi_c(string i){
    stringstream geek(i);
    int s=0;
    geek >> s;
    return s;
}


double stod_c(string i){
    stringstream geek(i);
    double s=0;
    geek >> s;
    return s;
}


string dtos_c(double i){
    stringstream s;
    s << i;
    return s.str();
}


void makeDir(string dirName){
    mkdir(dirName.c_str(), S_IRWXU);
}


void emptyDir(string dirName){
    system(("rm -r " + dirName +"/*").c_str());
}


void goToDir(string dirName){
    chdir(dirName.c_str());
}


string getDir() {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX);
    string current_working_dir(buff);
    return current_working_dir;
}


double get_wall_time(){
    struct timeval time;
    if(gettimeofday(&time, NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec*.000001;
}


double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

