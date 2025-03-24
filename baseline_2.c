#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DEBUG false
#define PRINT true


int bernoulli_sample(double p) {
    return ((double) rand() / RAND_MAX) < p ? 1 : 0;
}

const double SECOND_SCALE=1000000000;
#define SIZE 1000
int main(int argc, char *argv[]) {
    
    srand(time(NULL));
    if (argc != 3) { // Check if a filename was provided
        fprintf(stderr, "Usage: %s <filename> <corruption rate>\n", argv[0]);
        return 1;
    }
    double p = atof(argv[2]);
    FILE *file;

    fopen_s(&file, argv[1], "r"); // Open the file specified in the command-line argument
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    long long timestamp;
    double value;

    int isPredicting=0;
    long long xL=0.0;
    long long xR=0.0;
    double yL=0.0;
    double yR=0.0;

    long long *unknowns = malloc(SIZE * sizeof(long long));
    unsigned int nUnknowns=0;
    
    double *targets = malloc(SIZE * sizeof(double));

    double totalSquaredError = 0.0;
    unsigned int n = 0;

    // mainloop is here.
    while (fscanf_s(file, "%lld,%lf", &timestamp, &value) == 2) {
        if (bernoulli_sample(p)){
            // corrupted
            isPredicting=1;
            unknowns[nUnknowns]=timestamp;
            targets[nUnknowns]=value;
            nUnknowns++;
            n++;

        } else {
            // regular
            if (isPredicting){
                xR=timestamp;
                yR=value;
                long long dx=xR-xL;
                double dy=yR-yL;
                double grad=dy/dx;
                for (int i=0;i<nUnknowns;i++){
                    long long x = unknowns[i];
                    double y = targets[i];
                    double pred = grad*(x-xL)+yL;
                    totalSquaredError += powf(pred-y, 2.0);
                    if (PRINT) printf("%lld,%f\n",unknowns[i],y);
                }
                nUnknowns=0;
            }
            xL=timestamp;
            yL=value;
            isPredicting=0;

        }
    }
    fclose(file);
    double final_error = sqrtf(totalSquaredError/((double)n));
    if (DEBUG) printf("RMSE: %lf", final_error);
    return 0;
}

