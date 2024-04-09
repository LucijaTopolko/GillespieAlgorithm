//
// Created by Lucija Topolko on 06/04/2024.
//

#include <iostream>
#include <string>
#include "ParseTree.h"
#include "Simulator.h"
#include "JukesCantor.h"
#include "Kimura.h"

using namespace std;

string filename = "tree.nwk";
string rootDNA = "TACCCAACCTCACAGCCATCAGGGCGGCGGATGAGCATGTCCTCCAGATGCCCATACGTCAGCTATCGTACACCGGTCTCCGAGCCCCAGCAGCTCTTCG";
string MODE = "K"; // ili K

int main() {
    Node root = parseTree(filename);
    //dfs(root);

    root.sequence = rootDNA;

    double* rateMatrix;
    double* frequencies;

    if (MODE == "JC") {
        JukesCantor model;
        rateMatrix = model.getRatematrix();
        frequencies = model.getFrequencies();
    } else if (MODE == "K") {
        Kimura model;
        rateMatrix = model.getRatematrix();
        frequencies = model.getFrequencies();
    } else {
        exit(0);
    }

    simulateSequences(root, rateMatrix, frequencies);
    return 0;
}