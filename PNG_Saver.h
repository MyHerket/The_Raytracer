#pragma once
#include "Imagen.cpp"
#include "Matrices.c"
#include <cmath>
#include <algorithm>

class PNG {
public:
    unsigned int nx;
    unsigned int ny;
    unsigned char** mat;
    double** img;

    PNG();
    PNG(unsigned int x, unsigned int y, unsigned char** A) {
        nx = x;
        ny = y;
        mat = A;
        img = createMatrix(h2, w2);
        for (int i = 0; i < h2; i++) {
            for (int j = 0; j < w2; j++) {
                img[i][j] = (double)(mat[i][j]);
            }
        }
    }

    ~PNG() {
        freeMatrixChar(mat);
        freeMatrix(img);
    };
};

unsigned char** ConvertirTexto(double** X, unsigned int h, unsigned int w) {
    unsigned char** A = createMatrixChar(h, w);
    double a;
    int b;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            a = round(X[i][j]);
            b = (int)a;
            A[i][j] = (unsigned char)b;
        }
    }
    return A;
}
