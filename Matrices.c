#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reserva memoria para una matriz de double tama~no nr x nc
double **createMatrix(int nr, int nc) {
    int      i;
    double **mat;

    // Reservamos memoria
    mat    = (double **) malloc( (nr)*sizeof(double *));
    if(mat==NULL) return(NULL);
    mat[0] = (double *) malloc( nr*nc*sizeof(double));
    if(mat[0]==NULL) return(NULL);
    for(i=1; i<nr; ++i)
        mat[i] = mat[i-1] + nc;
    return(mat);
}

unsigned char **createMatrixChar(int nr, int nc) {
    int      i;
    unsigned char **mat;

    // Reservamos memoria
    mat    = (unsigned char **) malloc( (nr)*sizeof(unsigned char *));
    if(mat==NULL) return(NULL);
    mat[0] = (unsigned char *) malloc( nr*nc*sizeof(unsigned char));
    if(mat[0]==NULL) return(NULL);
    for(i=1; i<nr; ++i)
        mat[i] = mat[i-1] + nc;
    return(mat);
}


// Libera la memoria del arreglo bidimensional de double
void freeMatrix(double **mat) {
    free(mat[0]);
    free(mat);
}

// Libera la memoria del arreglo bidimensional de char
void freeMatrixChar(unsigned char **mat) {
    free(mat[0]);
    free(mat);
}

// Imprime en la consola las entradas de la matriz
void printMatrix(double **mat, int nr, int nc) {
    int i, j;
    for(i=0; i<nr; ++i) {
        for(j=0; j<nc; ++j)
            printf("% 6.2f   ", mat[i][j]);
        printf("\n");
    }
}
