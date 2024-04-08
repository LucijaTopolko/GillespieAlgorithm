//
// Created by Lucija Topolko on 06/04/2024.
//

#ifndef JUKES_CANTOR_H
#define JUKES_CANTOR_H


#include <iostream>

class JukesCantor {
private:
    double ratematrix[4][4] = {
        { 1, 1, 1, 1},
        { 1, 1, 1, 1},
        { 1, 1, 1, 1},
        { 1, 1, 1, 1}
    };

    double frequencies[4] = {0.25, 0.25, 0.25, 0.25};

public:
    double* getRatematrix() {
        return &ratematrix[0][0];
    }

    double* getFrequencies() {
        return frequencies;
    }
};

#endif //JUKES_CANTOR_H