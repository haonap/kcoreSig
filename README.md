# Code for computing k-core signatures
This repository contains code used for computing k-core signatures in the article "Graph Signatures: Identification and Optimization" which has been submitted European Journal of Operational Research. If you wish to use or cite this code, please cite:
        
    @misc{MWkCORE-GithubRepo,
      Author = {Hao Pan and Balabhaskar Balasundaram and Juan S. Borrero},
      Date-Added = {2020-10-15 17:39:38 -0500},
      Date-Modified = {2020-10-15 17:41:00 -0500},
      Howpublished = {Codes and instances online at: \url{https://github.com/haonap/kcoreSig}},
      Month = {October },
      Title = {Implementation of the moving window method for the maximum $k$-core signature problem.},
      Year = {2020}}
      
# Compilaton and execution in a Linux environment
1. Download or clone the repository. 
2. From terminal, go to repository. 
3. Type "make" and hit enter to compile. 
4. Open instance.txt file, which contains input information. There are four entries in this file: instance name(directory copy one from the "graphSequences" folder), tau, k, method(1 for GSIP-F2, 2 for MW). With these settings, it is going to compute tau-persistent k-core signature of the instance. 
5. In terminal, type "./main" and hit enter to execute. 
