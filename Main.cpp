//
// Created by Lucija Topolko on 06/04/2024.
//

#include <iostream>
#include <string>
#include "ParseTree.h"
#include "Simulator.h"

using namespace std;

string filename = "tree.nwk";
string rootDNA = "TACCCAACCTCACAGCCATCAGGGCGGCGGATGAGCATGTCCTCCAGATGCCCATACGTCAGCTATCGTACACCGGTCTCCGAGCCCCAGCAGCTCTTCG";

int main() {
    Node root = parseTree(filename);
    //dfs(root);

    root.sequence = rootDNA;

    simulateSequences(root);
    return 0;
}