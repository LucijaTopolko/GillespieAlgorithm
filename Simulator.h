//
// Created by Lucija Topolko on 06/04/2024.
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <string>
#include <random>
#include <map>
#include "ParseTree.h"

using namespace std;

class Simulator {
public:

    double const deletionRate = 0.01;
    double const insertionRate = 0.01;

    map<string, string> sequences = {};

    string simulate(const string &X, double *rateMatrix, double *frequencies, const double t) {
        return simulateHelper(X, rateMatrix, frequencies, t);
    }

private:
    random_device rng;

    string simulateHelper(const string &X, double *rateMatrix, double *frequencies, double t){
        double S = supstitutionRate(X, rateMatrix, frequencies);

        int length = 0;
        for (char c : X) {
            if (c != '-') {
                length++;
            }
        }

        double I = (length + 1) * insertionRate;
        double D = length * deletionRate;
        double M = S + I + D;
        string Y = X;
        double w = generateWaitingTime(M);

        while (w <= t) {
            int mutation = chooseMutation(S/M, I/M, D/M);
            if (mutation == 1) { //substitution
                vector<double> positionProbability;
                for (char c : Y) {
                    int index = getIndex(c);
                    if (index == -1){
                        positionProbability.push_back(0);
                    } else {
                        positionProbability.push_back(rateMatrix[index*4+index]/S);
                    }
                }
                int position = choose(positionProbability);
                char y = Y[position];

                vector<double> stateProbability;
                for (int i = 0; i<4; i++ ) {
                    if (i == getIndex(Y[position])) {
                        stateProbability.push_back(0);
                    } else {
                        stateProbability.push_back(rateMatrix[getIndex(y)*4 + i] / rateMatrix[getIndex(y)*4 + getIndex(y)]);
                    }
                }
                char z = "ACGT"[choose(stateProbability)];
                Y[position] = z;
                S += rateMatrix[getIndex(y)*4 + getIndex(y)] - rateMatrix[getIndex(z)*4 + getIndex(z)];
            } else if (mutation == 2) { //insertion
                vector<double> positionProbability;
                for (char c : Y) {
                    int index = getIndex(c);
                    if (index == -1){
                        positionProbability.push_back(0);
                    } else {
                        positionProbability.push_back(1);
                    }
                }
                positionProbability.push_back(1);
                int position = choose(positionProbability);

                int j = chooseLength();

                vector<double> stateProbability;
                for (int i = 0; i < 4; i++) {
                    stateProbability.push_back(rateMatrix[i*4+i]);
                }

                string sequence;
                double minus = 0;
                for (int i = 0; i < j; i++)  {
                    int s = choose(stateProbability);
                    char state = "ACGT"[s];
                    sequence.push_back(state);
                    minus += rateMatrix[s*4+s];
                }

                Y.insert(position, sequence);

                for ( auto &oldnode : sequences ) {
                    string gaps(j, '-');
                    oldnode.second.insert(position, gaps);
                }

                S -= minus;
                I += insertionRate * j;
                D += deletionRate * j;

            } else if (mutation == 3) { //deletion

                int j = chooseLength();
                vector<double> positionProbability;
                for (int i = 0; i<=Y.length()-j; i++) {
                    int index = getIndex(Y[i]);
                    if (index == -1){
                        positionProbability.push_back(0);
                    } else {
                        positionProbability.push_back(1);
                    }
                }
                int position = choose(positionProbability);

                int i = 0;
                double sum = 0;
                while (i < j && i < Y.length()) {
                    if (Y[position+i] != '-') {
                        int index = getIndex(Y[position+i]);
                        Y[position+i] = '-';
                        i++;
                        sum += rateMatrix[index*4+index];
                    }
                    i++;
                }
                S += sum;
                I -= insertionRate * j;
                D -= deletionRate * j;

            }
            M = S + I + D;
            t = t - w;
            w = generateWaitingTime(M);
            //cout<<w<<endl;
            //cout << Y << endl;
        }
        //cout << endl;
        return Y;
    }

    int chooseMutation(double substitutionProbability, double insertionProbability, double deletionProbability) {
        vector<double> probabilities = {substitutionProbability, insertionProbability, deletionProbability};
        mt19937 gen(rng());
        discrete_distribution<int> mutationDistribution(probabilities.begin(), probabilities.end());
        // 1 - substitution, 2 - insertion, 3 - deletion
        return mutationDistribution(gen) + 1;
    }

    int choose(vector<double> probability) {
        mt19937 gen(rng());
        discrete_distribution<int> positionDistribution(probability.begin(), probability.end());
        return positionDistribution(gen);
    }

    int chooseLength(double mean = 0.5) {
        mt19937 gen(rng());
        geometric_distribution<int> lengthDistribution(mean);
        return lengthDistribution(gen);
    }

    double generateWaitingTime(double M){
        mt19937 gen(rng());
        exponential_distribution<double> dist(M);
        return dist(gen);
    }

    static int getIndex(char c){
        switch (c) {
            case 'A':
                return 0;
            case 'C':
                return 1;
            case 'G':
                return 2;
            case 'T':
                return 3;
            default:
                return -1;
        }
    }

    static double supstitutionRate(const string &X, const double *rateMatrix, const double *frequencies) {
        double S = 0;
        double qi[4] = {0,0,0,0};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i!=j) {
                    qi[i] += rateMatrix[i*4+j] * frequencies[i];
                }
            }
        }
        for (int i = 0; i < X.length(); i++) {
            int index = getIndex(X[i]);
            if (index != -1) {
                S += qi[index];
            }
        }
        return S;
    }
};

Simulator simulator;

void dfs (Node &node, double *rateMatrix, double *frequencies, int depth = 0) { // NOLINT(*-no-recursion)

    string Y = simulator.simulate(node.parentSequence, rateMatrix, frequencies, node.length);
    simulator.sequences[node.name] = Y;
    node.sequence = Y;

    for (auto &child : node.elements)
    {
        child.parentSequence = simulator.sequences[node.name];
        dfs(child, rateMatrix, frequencies, depth + 1);
    }

}

bool canBeParsedToDouble(const std::string& str) {
    try {
        std::stod(str);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

void printdfsaligned(ofstream &file) {
    for (auto el : simulator.sequences) {
        if (el.first!="" && !canBeParsedToDouble(el.first) || 1) {
            file << ">" << el.first  << endl;
            file << el.second << endl;
        }
    }
}

void printdfs(Node &node, ofstream &file) {
    if (!node.name.empty()) {
        file << ">" << node.name << endl;
        file << node.sequence << endl;
    }
    for (auto &child : node.elements) {
        printdfs(child, file);
    }
}

void simulateSequences(Node &node, double *rateMatrix, double *frequencies) {

    simulator.sequences[node.name] = node.sequence;

    ofstream file;
    file.open("output.fasta");

    for (auto &child : node.elements)
    {
        child.parentSequence = simulator.sequences[node.name];
        dfs(child, rateMatrix, frequencies, 0);
    }

    printdfs(node, file);
    file.close();

    ofstream fileAligned;
    fileAligned.open("output_aligned.fasta");
    printdfsaligned(fileAligned);
    file.close();

}


#endif //SIMULATOR_H