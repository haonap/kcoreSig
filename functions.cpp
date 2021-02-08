#include "functions.h"


// containers
vector<graph> graphSeq;
vector<string> graphs;


// read in a graph sequence
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


// solve for a k-core signature using GSIP-F2
void GSIP_F2(int tau, int k){
    double st = get_wall_time();
    int T = (int)graphSeq.size();
    cout << "\nGSIP-F2..." << endl;
    cout << "Find " << tau << "-persistent " << k << "-core signature " << "T = " << graphs.size() << ", tau = " << tau << "..." << endl;

    GRBEnv* env = 0;
    GRBVar* xvar = 0;
    GRBVar* yvar = 0;
    GRBVar* zvar = 0;

    try{
        env = new GRBEnv();
        GRBModel model = GRBModel(*env);

        //create variables
        xvar = model.addVars(graphSeq[0].n, GRB_BINARY);
        yvar=model.addVars((int)(graphSeq.size() - tau + 1), GRB_BINARY); // this variable is introduced to linearize GSIP-F2
        zvar=model.addVars((int)graphSeq.size(), GRB_BINARY);
        model.update();

        //create objective function, set bounds, name variables
        for(int i = 0; i < graphSeq[0].n; i++){
            xvar[i].set(GRB_DoubleAttr_Obj,1);
        }
        model.update();

        //create constraints
        for(int t = 0; t < (int)graphSeq.size(); t++){
            for(int i = 0; i < graphSeq[t].n; i++){
                if(graphSeq[t].nodeList[i].neighbors.size() >= k){
                    GRBLinExpr expr = k*xvar[i];
                    for(int j = 0; j < graphSeq[t].nodeList[i].neighbors.size(); j++){
                        int pNeighbor = graphSeq[t].nodeList[i].neighbors[j];
                        expr -= xvar[pNeighbor];
                    }
                    model.addConstr(expr <= k*(1 - zvar[t]));
                }else{
                    model.addConstr(xvar[i] <= 1 - zvar[t]);
                }
            }
        }


        GRBLinExpr sumY = 0;
        for(int t = 0; t < (int)(graphSeq.size() - tau + 1); t++){
            sumY += yvar[t];
            for(int j = t; j <= t + tau - 1 ; j++){
                model.addConstr(yvar[t] <= zvar[j]);
            }
        }
        model.addConstr(sumY >= 1);
        model.update();

        // set Gurobi Parameters

        //set feasibility vs optimality balance
        model.set(GRB_IntParam_MIPFocus,0);
        //1-feasible sols quickly;2-prove optimality;3-focus on MIP bound; default is 0

        //set threads; review guidance on Gurobi.com; 0-default;
        model.set(GRB_IntParam_Threads,1);

        //set root node LPR solver
        model.set(GRB_IntParam_Method,-1);
        //-1=automatic, 0=primal simplex, 1=dual simplex, 2=barrier, 3=concurrent, 4=deterministic concurrent

        //set BC node LPR solver
        model.set(GRB_IntParam_NodeMethod,1);
        //0=primal simplex, 1=dual simplex, 2=barrier

        //set global cut aggressiveness; over-ridden by individual cut settings
        model.set(GRB_IntParam_Cuts,-1);
        //0=no cuts;1=moderate;2=aggressive;3=very aggressive;-1=default
        //model.set(GRB_IntParam_RLTCuts, 0);

        //set maximum time limit
        model.set(GRB_DoubleParam_TimeLimit,7200);

        //set termination gap limit; as needed; default is 1e-4
        model.set(GRB_DoubleParam_MIPGap,0);

        //set Gurobi log file name, if necessary; "" to switch off
        model.set(GRB_StringParam_LogFile, "");

        //set Gurobi screen display flag
        model.set(GRB_IntParam_OutputFlag,1);
        //0=switch off; 1=default

        // set Model Attributes

        //set objective to maximize
        model.set(GRB_IntAttr_ModelSense,-1);

        //set model name
        model.set(GRB_StringAttr_ModelName, "GSIP-F2");

        //in case of exhausting memory
        //model.getEnv().set(GRB_DoubleParam_NodefileStart,0.1);

        //begin optimization
        model.optimize();

        //output results
        if((int)model.get(GRB_IntAttr_SolCount) == 0){
            cout << "\nNo solution found, Gurobi optimization status: " << model.get(GRB_IntAttr_Status) << endl;

//            fstream fout("./graphSequences/" + seqName + "/results.txt", fstream::out);
//            fout << "No solution found, Gurobi optimization status: " << model.get(GRB_IntAttr_Status) << endl;
//            fout << "Processing time: " <<  fixed << setprecision(2) << (get_wall_time() - st) << " sec" << endl;
//            fout.close();
        }else{
            cout << "\nBest " << k << "-core signature found: ";
            int count = 0;
            for(int i = 0; i < graphSeq[0].n; i++){
                if(xvar[i].get(GRB_DoubleAttr_X) > 0.5){
                    cout << i + 1 << " ";
                    count++;
                }
            }
            cout << endl;
            cout << k << "-core signature size: " << count << endl;
            cout << "Best window: ";
            if(count != 0){
                for(int t = 0; t < graphSeq.size(); t++){
                    if(zvar[t].get(GRB_DoubleAttr_X) > 0.5){
                        cout << t + 1 << " (";
                        for(int i = t; i < t + tau; i++){
                            cout << "G" << i + 1;
                            if(i != t + tau -1){
                                cout << " ";
                            }
                        }
                        cout << ")" << endl;
                        break;
                    }
                }
            }else{
                cout << endl;
            }
            cout << "MIP gap: " << model.get(GRB_DoubleAttr_MIPGap) << endl;
            cout << "Best bound: " << model.get(GRB_DoubleAttr_ObjBound) << endl;
        }

    }catch (GRBException e){
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e .getMessage() << endl;
    }catch(...){
        cout << "Exception during optimization" << endl;
    }
    cout << "Processing time: " <<  fixed << setprecision(2) << (get_wall_time() - st) << " sec" << endl;
}


/* solve for a k-core signature using a MW method based on input parameter ``method"
 * if method = 2, use MW-CORE
 * if method = 3, use MW-F2 */
void MW(int tau, int k, int method){
    double st = get_wall_time();
    int T = (int)graphSeq.size();
    if(method == 2){
        cout << "\nMW..." << endl;
    }else if(method == 3){
        cout << "\nMW Generic..." << endl;
    }

    cout << "Find " << tau << "-persistent " << k << "-core " << "T = " << graphs.size() << ", tau = " << tau << "..." << endl;
    vector<int> bestKCore;
    int bestWindow = 0;

    for(int i = 0; i < T - tau + 1; i++){
        for(int j = i; j < i + tau; j++){
            for(int v = 0; v < graphSeq[j].n; v++){
                graphSeq[j].nodeList[v].degree = (int)graphSeq[j].nodeList[v].neighbors.size();
            }
        }
        vector<int> pKCore;
        if(method == 2){
            pKCore = GetPersistentKCoreWindow(i, tau, k);
        }else if(method == 3){
            pKCore = GetPersistentKCoreWindowGeneric(i, tau, k);
        }
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
    if((int)bestKCore.size() == 0){
        cout << "Best window: " << endl;
    }else{
        cout << "Best window: " << bestWindow + 1 << " (";
        for(int i = bestWindow; i < bestWindow + tau; i++){
            cout << "G" << i + 1;
            if(i < bestWindow + tau - 1){
                cout << " ";
            }
        }
        cout << ")" << endl;
    }
    cout << "Processing time: " <<  fixed << setprecision(2) << (get_wall_time() - st) << " sec\n";
}


// for solving each window problem when using MW-CORE
vector<int> GetPersistentKCoreWindow(int i, int tau, int k){
    vector<int> pstKCore; // persistent kCore
    queue<int> Q;
    vector<int> F;
    vector<vector<int>> D(tau);

    for(int v = 0; v < graphSeq[i].n; v++){
        F.push_back(0);
        for(int j = i; j < i + tau; j++){
            D[j - i].push_back((int)graphSeq[j].nodeList[v].neighbors.size());
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


// for solving each window problem when using MW-F2
vector<int> GetPersistentKCoreWindowGeneric(int windowHead, int tau, int k) {
    vector<int> pstKCore;
    //int upperBound = graphSeq[windowHead].n + 1;

    GRBEnv *env = 0;
    GRBVar *xvar = 0;

    try {
        env = new GRBEnv();
        GRBModel model =GRBModel(*env);

        // add variables
        xvar = model.addVars(graphSeq[0].n, GRB_BINARY);
        model.update();

        // add objective functions
        for(int i = 0; i < graphSeq[0].n; i++){
            xvar[i].set(GRB_DoubleAttr_Obj, 1);
        }
        model.update();

        // add constraints
        for(int t = windowHead; t < windowHead + tau; t++){
            for(int i = 0; i < graphSeq[t].n; i++){
                GRBLinExpr expr = k*xvar[i];
                for(int j = 0; j < graphSeq[t].nodeList[i].neighbors.size(); j++){
                    expr -= xvar[graphSeq[t].nodeList[i].neighbors[j]];
                }
                model.addConstr(expr <= 0);
            }
        }

        model.update();

        // set Gurobi Parameters

        //set feasibility vs optimality balance
        model.set(GRB_IntParam_MIPFocus, 0);
        //1-feasible sols quickly;2-prove optimality;3-focus on MIP bound; default is 0

        //set threads; review guidance on Gurobi.com; 0-default;
        model.set(GRB_IntParam_Threads, 0);

        //set root node LPR solver
        model.set(GRB_IntParam_Method, -1);
        //-1=automatic, 0=primal simplex, 1=dual simplex, 2=barrier, 3=concurrent, 4=deterministic concurrent

        //set BC node LPR solver
        model.set(GRB_IntParam_NodeMethod, 1);
        //0=primal simplex, 1=dual simplex, 2=barrier

        //set global cut aggressiveness; over-ridden by individual cut settings
        model.set(GRB_IntParam_Cuts, 0);
        //0=no cuts;1=moderate;2=aggressive;3=very aggressive;-1=default

        //set maximum time limit
        model.set(GRB_DoubleParam_TimeLimit, 3600);

        //set termination gap limit; as needed; default is 1e-4
        model.set(GRB_DoubleParam_MIPGap, 0);

        //set Gurobi log file name, if necessary; "" to switch off
        model.set(GRB_StringParam_LogFile, "");

        //set Gurobi screen display flag
        model.set(GRB_IntParam_OutputFlag, 1);
        //0=switch off; 1=default

        // set Model Attributes

        //set objective to maximize
        model.set(GRB_IntAttr_ModelSense, -1);

        //set model name
        model.set(GRB_StringAttr_ModelName, "FindPersistentKCore");

        //in case of exhausting memory
        //model.getEnv().set(GRB_DoubleParam_NodefileStart,0.1);

        //begin optimization
        model.optimize();

        // get results
        if ((int) model.get(GRB_IntAttr_SolCount)) {
            for (int i = 0; i < graphSeq[0].n; i++) {
                if (xvar[i].get(GRB_DoubleAttr_X) > 0.5) {
                    pstKCore.push_back(i);
                }
            }
        }

    } catch (GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Exception during optimization" << endl;
    }

    return pstKCore;
}


// integer to string
string itos_c(int i){
    stringstream s;
    s << i;
    return s.str();
}


// string to integer
int stoi_c(string i){
    stringstream geek(i);
    int s=0;
    geek >> s;
    return s;
}


// string to double
double stod_c(string i){
    stringstream geek(i);
    double s=0;
    geek >> s;
    return s;
}


// double to string
string dtos_c(double i){
    stringstream s;
    s << i;
    return s.str();
}


// create new directory
void makeDir(string dirName){
    mkdir(dirName.c_str(), S_IRWXU);
}


// delete all files in a directory
void emptyDir(string dirName){
    system(("rm -r " + dirName +"/*").c_str());
}


// go to a directory
void goToDir(string dirName){
    chdir(dirName.c_str());
}


// get current directory
string getDir() {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX);
    string current_working_dir(buff);
    return current_working_dir;
}


// get wall time
double get_wall_time(){
    struct timeval time;
    if(gettimeofday(&time, NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec*.000001;
}


// get CPU time
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

