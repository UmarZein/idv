#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DEBUG false
#define PRINT false
#define EVAL true


int bernoulli_sample(double p) {
    return ((double) rand() / RAND_MAX) < p ? 1 : 0;
}


int is_nan(const float num) {
    return num != num;  // NaN is never equal to itself
}
const double SECOND_SCALE=1000000000;
#define SIZE 1000

struct Output{
    float mae;
    float rmse;
};
struct Output inner(int, char*[]);
float _mean(const float *x, const int n){
    float mean=0;
    const float _n = (float)n;
    for (int i=0;i<n;i++){
        if (!is_nan(x[i])) mean=mean+(x[i]-mean)/_n;
    }
    return mean;
}
//TODO: _mean and _std takes an array of error metric. however they contain NaN. find out why and what to do
float _std(const float *x, const int n){
    if (n<=1){
        return 0;
    }
    double mean=0;
    double var=0;
    long long n_nans = 0;
    for (int i=0;i<n;++i){
        if (is_nan(x[i])){
            n_nans++;
            continue;
        }
        const double xi = x[i];
        const double delta = xi-mean;
        mean += delta / ((double)(i-n_nans+1));
        var += delta*(xi-mean);
    }
    var/=(double)n-1.0;
    return sqrtf((float)var);
}
unsigned int hash(const unsigned int x){
    unsigned int h=x;
    h *= 0x5bd1e995;
    h ^= h >> 15;
    return h;
}
int main(int argc, char *argv[]) {
    srand(time(NULL));
    unsigned int rander=0xdeadbeef^time(NULL);
    if (argc != 4) { // Check if a filename was provided
        fprintf(stderr, "Usage: %s <filename> <corruption rate>  <num runs>\n", argv[0]);
        return 1;
    }
    float p = atof(argv[2]);
    int n_runs = atoi(argv[3]);
    if ((p<0)||(p>1)){
        fprintf(stderr, "corruption rate must be between 0 and 1\n");
        return 1;
    }
    float *mae = malloc(sizeof(float)*n_runs);
    float *rmse = malloc(sizeof(float)*n_runs);
    for (int i=0; i<n_runs; i++){
        struct Output out = inner(argc, argv);
        srand(rander);
        rander=hash(rander);
        mae[i]=out.mae;
        rmse[i]=out.rmse;
    }
    if (EVAL) printf("MAE: %02.2f %c %02.2f\n", _mean(mae, n_runs), 241, _std(mae, n_runs));
    if (EVAL) printf("RMSE: %02.2f %c %02.2f\n", _mean(rmse, n_runs), 241, _std(rmse, n_runs));
    free(mae);
    free(rmse);
    return 1;
}


struct Output inner(int argc, char *argv[]) {
    struct Output out;
    out.rmse=0.0/0.0;
    out.mae=0.0/0.0;
    srand(time(NULL));
    float p = atof(argv[2]);
    int n_runs = atoi(argv[3]);
    FILE *file;

    fopen_s(&file, argv[1], "r"); // Open the file specified in the command-line argument
    if (!file) {
        perror("Error opening file");
        return out;
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
    double totalError = 0.0;
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
                    totalError += fabsf(pred-y);
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
    double rmse = sqrtf(totalSquaredError/((double)n));
    double mae = (totalError/((double)n));
    out.rmse=(float)rmse;
    out.mae=(float)mae;
    if (DEBUG) printf("RMSE: %lf", rmse);
    if (DEBUG) printf("MAE: %lf", mae);
    return out;
}

