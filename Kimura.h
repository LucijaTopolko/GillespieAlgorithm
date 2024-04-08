//
// Created by Lucija Topolko on 06/04/2024.
//

#ifndef KIMURA_H
#define KIMURA_H

class Kimura {
private:
    double ratematrix[4][4] = {
            { 1, 1, 2, 1},
            { 1, 1, 1, 2},
            { 2, 1, 1, 1},
            { 1, 2, 1, 1}
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

#endif //KIMURA_H