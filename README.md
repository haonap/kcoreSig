# Code for computing k-core signatures
This repository contains C++ code used for computing k-core signatures in the article "Graph Signatures: Identification and Optimization" which has been published in European Journal of Operational Research. If you wish to use or cite this code, please cite:
        
        @article{Hao2022GraphSign,
                author = {Balabhaskar Balasundaram and Juan S. Borrero and Hao Pan},
                journal = {European Journal of Operational Research},
                month = {February},
                number = {3},
                pages = {764--775},
                title = {Graph Signatures: {I}dentification and Optimization},
                volume = {296},
                year = {2022},
                doi = {https://doi.org/10.1016/j.ejor.2021.03.051},
        }

# Understanding and using the code
The code should be straightforward if you start to read from file main.cpp. Necessary comments have been added in the code for easiness of understanding. Descriptions are added at the top of each function in functions.cpp and classes.cpp. As stated previously, the code is used for computing k-core signatures. We present three methods for computing k-core signatures, GSIP-F2, MW-CORE, and MW-F2. 

In file main.cpp, the main function starts by reading in input parameters from file instance.txt. File instance.txt contains four entries, instance name, tau, k, and method (method is indexed by a number, 1 for GSIP-F2, 2 for MW-2CORE, and 3 for MW-F2). For instance name, please go into folder graphSequences and directly copy the name of a instance you would like to test on. Each instance is actually a graph sequence. For tau, you can pick any positive integer no bigger than the length of the graph sequence. For $k$, you can pick any nonnegative integer. For example, file instance.txt contains "karate_10_0.8 3 3 1" by default, which is instructing the code to compute a 3-persistent 3-core signature of the instance karate_10_0.8 using GSIP-F2. 

# Compilation and execution in Linux environment
1. Download or clone the repository to your machine. 
2. From terminal, go to the repository. 
3. Type "make" and hit enter to compile. 
4. Open instance.txt file, enter instance name, tau, k, and method sequentially as input parameters. 
5. In terminal, type "./main" and hit enter to execute. 
