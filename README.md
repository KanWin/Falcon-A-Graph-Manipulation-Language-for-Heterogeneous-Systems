Welcome to the A-Heterogeneous-DSL-for-CPU-and-GPU wiki!

This work is part of the ACM TACO publication Falcon: A Graph Manipulation Language for Heterogeneous Systems ( http://dl.acm.org/citation.cfm?id=2842618).

Above code can be used freely for any academic research works. If you are using the code please cite the above paper.

ROOT FOLDER = ./
Set the GALOIS_ROOT to appropriate directory in your machine. (modifiy the CPU-Galois/comm.mk file)
If Galois is not installed, comment the Make of Galois-codes in MakeFile in ROOT DIRECTORY.
Do a make from ROOT DIRECTORY.
dsl codes in apps folder will be compiled to C++/CUDA code and written in CPU/GPU/CPU-Galois folders under ROOT FOLDER.
then these codes will be compiled to binaries and written to bin folder under ROOT FOLDER.

input graph can  be in binary .gr or edglist format. convertion of edgelist to .gr can be done with code unde folder utility.
edgelist format has following syntax with edges sorted in ascending order of src vertex.
npoints nedges (line 0)
src dst weight (line 1 to nedges)

Morph algorithm
For dynamic sssp addition of edges should be given as a text file. input graph should be in .gr format.

For DMR algorithm the inputs and node files and preprocessed nbrs files.
if command line argument to dmr is r25k , inputs expected by algorithm are r25k.node,r25k.ele,r25.poly,r25k.nbr



Who do I talk to?

Repo owner or admin - unni_c@csa.iisc.ernet.in

**A Distributed memory Version of Falcon using MPI will be released soon.**
