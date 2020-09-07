//
// Created by Hao Pan on 9/1/20.
//

#ifndef KCORESIG_FUNCTIONS_H
#define KCORESIG_FUNCTIONS_H


#include <iostream>
#include "classes.h"
#include <fstream>
#include <vector>
#include <sys/time.h>
#include <iomanip>
//#include <map>
#include <queue>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

/* for getDir() */
#include <stdio.h>  // defines FILENAME_MAX
// #define WINDOWS  // uncomment this line to use it for windows
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

void ReadIn(string);
void FindPersistentKCore(int, int);
vector<int> GetPersistentKCoreWindow(int, int, int);

string itos_c(int);
int stoi_c(string);
double stod_c(string);
string dtos_c(double);

void makeDir(string);
void emptyDir(string);
void goToDir(string);
string getDir();

double get_wall_time();
double get_cpu_time();


#endif //KCORESIG_FUNCTIONS_H

