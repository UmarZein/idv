#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define W 6
#define DEBUGLEVEL 0 
#define PRINT true
#define EVAL true
#define sqr(x) ((x) * (x))

bool bernoulli_sample(const float p) {
    return ((float) rand() / RAND_MAX) < p;
}

bool alternating(const int x) {
    int n = 1; // Number of zeros before ones
    int count = 0; // Position tracker
    
    while (count <= x) {
        // Zeros first
        for (int i = 0; i < n; i++) {
            if (count == x) return false;
            count++;
        }
        // Ones next
        for (int i = 0; i < n; i++) {
            if (count == x) return true;
            count++;
        }
        n++; // Increase the zero-one block size
    }
    return false; // Default case (should never reach here)
}
bool corrupted(const int iters, const float p){
    //return (iters>W)&&alternating(iters-W);
    return bernoulli_sample(p);
}

int is_nan(const float num) {
    return num != num;  // NaN is never equal to itself
}

const float SECOND_SCALE=1000000000;

bool hasTrue(const bool array[W]){
    for (int i = 0; i<W; i++){
        if (array[i]!=false){return true;}
    }
    return 0;
}

void ffillll(long long array[W]){
    for (int i = 0; i<W-1; i++){
        array[i]=array[i+1];
    }
}
void aprintlld(const long long array[W]){
    for (int i = 0; i<W; i++){
        printf("%lld ",array[i]);
    }
    printf("\n");
}
void aprintf(const float array[W]){
    for (int i = 0; i<W; i++){
        printf("%02.2f ",array[i]);
    }
    printf("\n");
}
void wwprintf(const float array[W][W]){
    for (int i = 0; i<W; i++){
        for (int j=0;j<W;j++){
            const float x = array[i][j];
            if (x>=10) printf("%03.0f ",x);
            else printf("%.01f ",x);
        }
        printf("\n");
    }
}
void aprintb(const bool array[W]){
    for (int i = 0; i<W; i++){
        printf("%d ",(int)array[i]);
    }
    printf("\n");
}
void ffillb(bool array[W]){
    for (int i = 0; i<W-1; i++){
        array[i]=array[i+1];
    }
}
void ffillf(float array[W]){
    for (int i = 0; i<W-1; i++){
        array[i]=array[i+1];
    }
}
int countTrue(const bool array[W]){
    int n = 0;
    for (int i = 0; i<W; i++){
        if (array[i]!=false){n++;}
    }
    return n;
}
void __ffillfww(float array[W][W]){
    for (int i=0;i<W;i++){
        ffillf(array[i]);
    }
}

const float NaN=0.0/0.0;
void fillLastsAsNans(float array[W][W]){
    for (int i=0;i<W;i++){
        array[i][W-1]=NaN;
    }
}

void ffillfww(float array[W][W]){
    for (int i=0;i<W;i++){
        ffillf(array[i]);
        array[i][W-1]=NaN;
    }
}

float mean(const float array[W]){
    float total = 0;
    float n = 0;
    for (int i=0;i<W;i++){
        if (is_nan(array[i])) continue;
        total = total + array[i];
        n = n + 1.0;
    }
    return total/n;
}

float mean2(const float array[W], const bool filter[W]){
    float total = 0;
    float n = 0;
    for (int i=0;i<W;i++){
        if (filter[i]) continue;
        total = total + array[i];
        n = n + 1.0;
    }
    return total/n;
}

float std(const float array[W]){
    float se = 0;
    for (int i=0;i<W;i++){
        if (is_nan(array[i])) continue;
        se = se + sqr(array[i]);
    }
    return sqrtf(se/(float)W);
}
float std2(const float array[W], const bool filter[W]){
    float se = 0;
    for (int i=0;i<W;i++){
        if (filter[i]) continue;
        se = se + sqr(array[i]);
    }
    return sqrtf(se/(float)W);
}

float variabilitas(const float array[W]){
    float totalSquares = 0;
    float total = 0;
    for (int i=0;i<W;i++){
        if (is_nan(array[i])) continue;
        totalSquares = totalSquares + sqr(array[i]);
        total = total + array[i];
    }
    return sqrtf(totalSquares/(float)W)*(float)W/total;
}
float variabilitas2(const float array[W], const bool filter[W]){
    float totalSquares = 0;
    float total = 0;
    for (int i=0;i<W;i++){
        if (filter[i]) continue;
        totalSquares = totalSquares + sqr(array[i]);
        total = total + array[i];
    }
    return sqrtf(totalSquares/(float)W)*(float)W/total;
}

void updateInterpolable(const bool array[W], bool result[W]){
    bool isLBounded[W] = { [0 ... W-1] = false };
    bool isRBounded[W] = { [0 ... W-1] = false };
    bool LBounded=false;
    bool RBounded=false;
    for (int i=0;i<W;i++){
        isLBounded[i] = LBounded;
        LBounded = !array[i] | LBounded;

        int j = W-1-i;
        isRBounded[j] = RBounded;
        RBounded = !array[j] | RBounded;
    }
    for (int i=0;i<W;i++){
        result[i] = array[i]&isLBounded[i]&isRBounded[i];
    }
}


// if y=f(x), predict what f(z) is using interpolation
float interpLagrange(const long long x[W],const float y[W], const bool nans[W],const long long z){
    if (DEBUGLEVEL>2) printf("interpLagrange(x[W], y[W], nans[W], %lld)\n", z);
    if (DEBUGLEVEL>2) printf("x: ");
    long long normalized_x[W];
    long long first = x[0];
    long long last = x[W-1];
    for (int i=0;i<W;i++){
        normalized_x[i] = (x[i]-first);
    }
    if (DEBUGLEVEL>2) aprintlld(x);
    if (DEBUGLEVEL>2) printf("y: ");
    if (DEBUGLEVEL>2) aprintf(y);
    if (DEBUGLEVEL>2) printf("nans: ");
    if (DEBUGLEVEL>2) aprintb(nans);
    
    long long x_valid[W];  // Valid x values
    float y_valid[W];       // Valid y values
    int valid_count = 0;    // Number of valid points

    // Filter out NaN values
    for (int i = 0; i < W; i++) {
        if (!nans[i]) {
            x_valid[valid_count] = x[i];
            y_valid[valid_count] = y[i];
            valid_count++;
        }
    }

    if (DEBUGLEVEL>2) printf("valid_count = %d\n",valid_count);

    // If no valid points or only one valid point, return NaN
    if (valid_count == 0) return NaN;
    if (valid_count == 1) return y_valid[0];  // Only one point, return its y value

    // Perform Lagrange interpolation
    float result = 0.0;
    for (int i = 0; i < valid_count; i++) {
        float term = y_valid[i];
        for (int j = 0; j < valid_count; j++) {
            if (i != j) {
                term *= (float)(z - x_valid[j]) / (float)(x_valid[i] - x_valid[j]);
            }
        }
        if (DEBUGLEVEL>2) printf("term = %02.2f\n", term);
        result += term;
    }
    if (DEBUGLEVEL>2){
        if (result<0.0){
            printf("NEGATIVE RESULT\n");
        }
    }
    if (DEBUGLEVEL>2) printf("result = %02.2f\n", result);
    return result;
}


bool findAnomaly(const float array[W], const bool filter[W]){
    float se = 0;
    float total = 0;
    float n = 0;
    float largest = -1.0/0.0;
    float smallest = 1.0/0.0;
    for (int i=0;i<W;i++){
        if (filter[i]) continue;
        const float cur = array[i];
        n = n + 1;
        total = total + cur;
        se = se + sqr(cur);
        if (cur>largest){
            largest = cur;
        } else if (cur<smallest){
            smallest = cur;
        }
    }
    if (n==0) return false;
    const float var = se/W;
    const float mean = total/n;
    const float variabilitasSqr = var/sqr(mean);
    if (variabilitasSqr>sqr(0.5)){ // variabilitas
        return true;
    }
    return ((sqr(largest-mean)>4*var)||
       (sqr(mean-smallest)>4*var));
}

float special_mean(const float array[W]){
    float total=0;
    float n = 0;
    for (int i=0;i<W;i++){
        if (!is_nan(array[i])){
            total = total + array[i];
            n = n + 1;
        } else {
        }
    }
    return total/n;
}
int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 4) { // Check if a filename was provided
        fprintf(stderr, "Usage: %s <filename> <corruption rate> <ewm coeff>\n", argv[0]);
        return 1;
    }
    float p = atof(argv[2]);
    float decay_rate = atof(argv[3]);
    if ((p<0)||(p>1)){
        fprintf(stderr, "corruption rate must be between 0 and 1\n");
        return 1;
    }
    if ((decay_rate<0)||(decay_rate>1)){
        fprintf(stderr, "ewm coeff must be between 0 and 1\n");
        return 1;
    }
    FILE *file;

    fopen_s(&file, argv[1], "r"); // Open the file specified in the command-line argument
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    long long timestamp;
    float target;
    bool hasAnomaly = false;
    
    float truths[W] = { [0 ... W-1] = 0.0 };
    float ewms[W] = { [0 ... W-1] = 0.0 };
    float values[W] = { [0 ... W-1] = 0.0 };
    long long timestamps[W] = { [0 ... W-1] = 0.0 };

    bool nans[W] = { [0 ... W-1] = false };
    bool interpolable[W] = { [0 ... W-1] = false };
    float stds[W] = { [0 ... W-1] = 0.0 };
    float varias[W] = { [0 ... W-1] = 0.0 };//variabilitas
    float interps[W][W] = { [0 ... W-1] = { [0 ... W-1] = (0.0/0.0) }};

    float totalSquaredError = 0.0;
    float totalAbsoluteError = 0.0;
    double totalSignal = 0.0;
    double totalSquaredSignal = 0.0;
    unsigned int totalNans = 0;
    unsigned int totalUninterpolated = 0;
    int iters=0;

    // mainloop is here.
    while (fscanf_s(file, "%lld,%f", &timestamp, &target) == 2) {
        if (DEBUGLEVEL) printf("\n------------------\n\n");
        iters++;
        totalSignal = totalSignal + (double)target;
        totalSquaredSignal = totalSquaredSignal + sqr((double)target);
        if (DEBUGLEVEL) printf("iteration: %d\n", iters);

        ffillf(truths);
        truths[W-1]=target;

        if (DEBUGLEVEL) printf("truths: ");
        if (DEBUGLEVEL) aprintf(truths);

        ffillll(timestamps);
        timestamps[W-1]=timestamp;

        ffillf(values);
        ffillb(nans);
        ffillfww(interps);

        bool isCorrupted = corrupted(iters,p) || is_nan(target);
        nans[W-1]=isCorrupted;
        if (isCorrupted){
            totalNans++;
        } else {
            values[W-1]=target;
        }
        updateInterpolable(nans, interpolable);

        if (DEBUGLEVEL) printf("values: ");
        if (DEBUGLEVEL) aprintf(values);

        if (DEBUGLEVEL) printf("ewms:   ");
        if (DEBUGLEVEL) aprintf(ewms);

        if (DEBUGLEVEL) printf("nans:         ");
        if (DEBUGLEVEL) aprintb(nans);

        if (DEBUGLEVEL) printf("interpolable: ");
        if (DEBUGLEVEL) aprintb(interpolable);

        ffillf(ewms);
        ewms[W-1]=values[W-1]*decay_rate+ewms[W-2]*(1.0-decay_rate);

        hasAnomaly = findAnomaly(values, nans);
        if (DEBUGLEVEL) printf("hasAnomaly: %d\n",hasAnomaly);

        int num_nans = countTrue(nans);
        if ((0<num_nans)&&(num_nans<=W/2)){
            for (int i=1; i<W-1; i++){
                if (nans[i]==false) {
                    if (DEBUGLEVEL>1) printf("nans[%d] is false so skipping\n",i);
                    continue;
                };
                if (interpolable[i]==false) {
                    if (DEBUGLEVEL>1) printf("interpolable[%d] is false so skipping\n",i);
                    continue;
                };
                int interpIndex = (iters+i)%W;
                if (hasAnomaly) interps[interpIndex][W-1] = interpLagrange(timestamps, ewms, nans, timestamps[i]);
                else interps[interpIndex][W-1] = interpLagrange(timestamps, values, nans, timestamps[i]);
                if (DEBUGLEVEL) printf("assigned to interps[%d][-1]\n",interpIndex);
            }
        }
        if (DEBUGLEVEL) printf(" === interp table ===\n");
        if (DEBUGLEVEL) wwprintf(interps);
        if (DEBUGLEVEL) printf(" === ****** ***** ===\n");
        if ((nans[0]!=false) && (iters>1+W)){
            float pred = special_mean(interps[iters%W]);
            if (DEBUGLEVEL) printf("%02.2f %02.2f\n", truths[0], pred);
            if (is_nan(pred)){
                if (PRINT) printf("%lld,nan\n",timestamps[0]);
                totalUninterpolated++;
                continue;
            }
            if (PRINT) printf("%lld,%f\n",timestamps[0],pred);
            totalSquaredError+=sqr(pred-truths[0]);
            totalAbsoluteError+=fabsf(pred-truths[0]);
        } else {
            if (DEBUGLEVEL) printf("%02.2f -\n", truths[0]);
	    if (PRINT) printf("%lld,%0.2f\n",timestamps[0],ewms[0]);
        }

    }
    fclose(file);
    double sigMean = totalSignal/iters;
    double sigStd = sqrt((totalSquaredSignal-sqr(totalSignal)/iters)/(iters-1));
    float rmse = sqrtf(totalSquaredError/((float)totalNans));
    float mae = totalAbsoluteError/((float)totalNans);
    if (EVAL) {
        printf("iters: %d\n", iters);
        printf("data mean: %lf\n", sigMean);
        printf("data std: %lf\n", sigStd);
        printf("NaNs/samples: %02.2f%%\n", 100*(float)totalNans/(float)iters);
        printf("uninterpolated/samples: %02.2f%%\n", 100*(float)totalUninterpolated/(float)iters);
        printf("uninterpolated/NaNs: %02.2f%%\n", 100*(float)totalUninterpolated/(float)totalNans);
        printf("RMSE: %02.5f\n", rmse);
        printf("MAE: %02.5f\n", mae);
    }
    return 0;
}

