#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int bernoulli_sample(double p) {
    return ((double) rand() / RAND_MAX) < p ? 1 : 0;
}

const double SECOND_SCALE=1000000000;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 4) { // Check if a filename was provided
        fprintf(stderr, "Usage: %s <filename> <corruption rate> <momentum>\n", argv[0]);
        return 1;
    }
    double p = atof(argv[2]);
    double momentum = atof(argv[3]);
    FILE *file;

    fopen_s(&file, argv[1], "r"); // Open the file specified in the command-line argument
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    long long timestamp;
    double value;

    long long xa=0.0;
    long long xb=0.0;
    double ya=0.0;
    double yb=0.0;

    double total_error = 0.0;
    unsigned int n = 0;

    // mainloop is here.
    while (fscanf_s(file, "%lld %lf", &timestamp, &value) == 2) {
        if (bernoulli_sample(p)){
            // corrupted
            double dx = xb-xa;
            double dy = yb-ya;
            double grad=dy/dx;
            double pred = ya+grad*((double)(timestamp-xb));
            double se = powf(value-pred, 2.0);
            total_error += se;
            n++;
            xa=xb;
            xb=timestamp;
            ya=yb;
            yb=pred;
        } else {
            // regular
            xa=xb;
            ya=yb;
            xb=timestamp;
            yb=value;
        }
    }
    fclose(file);
    double final_error = sqrtf(total_error/((double)n));
    printf("RMSE: %lf", final_error);
    return 0;
}

