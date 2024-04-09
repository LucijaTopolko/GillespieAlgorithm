//
// Created by Lucija Topolko on 06/04/2024.
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <string>
#include <random>
#include "ParseTree.h"

using namespace std;

class Simulator {
public:

    string simulate(const string &X, double *rateMatrix, double *frequencies, const double t) {
        return simulateHelper(X, rateMatrix, frequencies, t);
    }

private:
    double deletionRate = 0.1;
    double insertionRate = 0.1;
    random_device rng;

    string simulateHelper(const string &X, double *rateMatrix, double *frequencies, double t){
        double S = supstitutionRate(X, rateMatrix, frequencies);
        double D = (double)X.length() * deletionRate;
        double I = (double)(X.length() + 1) * insertionRate;
        double M = S + I + D;

        string Y = X;
        cout << Y<<endl;
        double w = generateWaitingTime(M);

        while (w <= t) {
            int mutation = chooseMutation(S/M, I/M, D/M);
            cout << "Mutation: "<<mutation<<endl;
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
                    cout << Y[position] << " " << z << endl;
                    Y[position] = z;
                    S += rateMatrix[getIndex(y)*4 + getIndex(y)] - rateMatrix[getIndex(z)*4 + getIndex(z)];
            } else if (mutation == 2) { //insertion

            } else if (mutation == 3) { //deletion

            }
            M = S + I + D;
            t = t - w;
            w = generateWaitingTime(M);
            cout<<Y<<endl;
        }

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

    static double supstitutionRate(const string &X, const double *rateMatrix, double *frequencies) {
        double S = 0;
        double qi[4];
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

void simulateSequences(const Node &root, double *rateMatrix, double *frequencies) {
    Simulator simulator;
    simulator.simulate(root.sequence, rateMatrix, frequencies, root.length);
}

#endif //SIMULATOR_H