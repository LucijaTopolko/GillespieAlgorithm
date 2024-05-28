//
// Created by Lucija Topolko on 06/04/2024.
//

#include <iostream>
#include <string>
#include <windows.h>
#include <psapi.h>
#include <chrono>
#include "ParseTree.h"
#include "Simulator.h"
#include "JukesCantor.h"
#include "Kimura.h"

using namespace std;

SIZE_T GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
}

string filename = "10.nwk";
string rootDNA = "";
string MODE = "JC"; // JC ili K


int main() {
    auto start = chrono::high_resolution_clock::now();

    Node root = parseTree(filename);

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

    string bases = "ACGT";

    random_device rd;
    mt19937 gen(rd());

    discrete_distribution<> d(frequencies, frequencies + 4);

    if (rootDNA.empty()) {
        for (int i = 0; i < 1000; ++i) {
            int index = d(gen);
            rootDNA += bases[index];
        }
    }


    root.sequence = rootDNA;

    simulateSequences(root, rateMatrix, frequencies);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Vrijeme izvrsavanja: " << duration.count() << " sekundi" << std::endl;

    SIZE_T memoryUsage = GetMemoryUsage() - sizeof(start) - sizeof(end) - sizeof(duration);

    cout << "Memorijska potrosnja: " << memoryUsage*1.0/1024/1024 << " MB" << endl;

    return 0;
}