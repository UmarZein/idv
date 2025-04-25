#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#ifndef ELEM_SWAP(a,b)
#define ELEM_SWAP(a,b) { register float t=(a);(a)=(b);(b)=t; }
#endif
#define MADK 3.0
#define W 6
#define DEBUGLEVEL 0 
#define PRINT false
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
//Algorithm from Numerical recipes in C of 1992
float median(const float arr_[W], const bool nans[W])
{
    float arr[W] = { [0 ... W-1] = 0.0/0.0 };
    int n=0;
    for (int i=0;i<W;i++){
        if (!nans[i]){
            arr[n]=arr_[i];
            n++;
        }
    }
    int low, high ;
    int median;
    int middle, ll, hh;
    low = 0 ; high = n-1 ; median = (low + high) / 2;
    for (;;) {
        if (high <= low) return arr[median];
        if (high == low + 1) { /* Two elements only */
            if (arr[low] > arr[high])
                ELEM_SWAP(arr[low], arr[high]) ;
            return arr[median] ;
        }
        /* Find median of low, middle and high items; swap into position low */
        middle = (low + high) / 2;
        if (arr[middle] > arr[high])
            ELEM_SWAP(arr[middle], arr[high]) ;
        if (arr[low] > arr[high])
            ELEM_SWAP(arr[low], arr[high]) ;
        if (arr[middle] > arr[low])
            ELEM_SWAP(arr[middle], arr[low]) ;
        /* Swap low item (now in position middle) into position (low+1) */
        ELEM_SWAP(arr[middle], arr[low+1]) ;
        /* Nibble from each end towards middle, swapping items when stuck */
        ll = low + 1;
        hh = high;
        for (;;) {
            do ll++; while (arr[low] > arr[ll]) ;
            do hh--; while (arr[hh] > arr[low]) ;
            if (hh < ll) break;
            ELEM_SWAP(arr[ll], arr[hh]) ;
        }
        /* Swap middle item (in position low) back into correct position */
        ELEM_SWAP(arr[low], arr[hh]) ;
        /* Re-set active partition */
        if (hh <= median) low = ll;
        if (hh >= median) high = hh - 1;
    }
    return arr[median] ;
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
    return fabsf(sqrtf(totalSquares/(float)W)*(float)W/total);
}

float variabilitas2(const float array[W], const bool filter[W]){
    float totalSquares = 0;
    float total = 0;
    for (int i=0;i<W;i++){
        if (filter[i]) continue;
        totalSquares = totalSquares + sqr(array[i]);
        total = total + array[i];
    }
    return fabsf(sqrtf(totalSquares/(float)W)*(float)W/total);
}
void updateAnd(const bool a[W], const bool b[W], bool result[W]){
    for (int i=0;i<W;i++){
        result[i]=a[i]&&b[i];
    }
}
void updateOr(const bool a[W], const bool b[W], bool result[W]){
    for (int i=0;i<W;i++){
        result[i]=a[i]||b[i];
    }
}
void updateLFillable(const bool nans[W], bool result[W]){
    bool has_atleast_one_elem=false;
    for (int i=0;i<W;i++){
        if (!nans[i]){
            has_atleast_one_elem=true;
            break;
        }
    }
    bool active=true;
    for (int i=0;i<W;i++){
        active=active&&nans[i]&&has_atleast_one_elem;
        result[i]=active;
    }
}
void updateRFillable(const bool nans[W], bool result[W]){
    bool has_atleast_one_elem=false;
    for (int i=0;i<W;i++){
        if (!nans[i]){
            has_atleast_one_elem=true;
            break;
        }
    }
    bool active=true;
    for (int i=W-1;i>=0;i--){
        active=active&&nans[i]&&has_atleast_one_elem;
        result[i]=active;
    }
}
void updateInterpolable(const bool nans[W], bool result[W]){
    bool isLBounded[W] = { [0 ... W-1] = false };
    bool isRBounded[W] = { [0 ... W-1] = false };
    bool LBounded=false;
    bool RBounded=false;
    for (int i=0;i<W;i++){
        isLBounded[i] = LBounded;
        LBounded = !nans[i] | LBounded;

        int j = W-1-i;
        isRBounded[j] = RBounded;
        RBounded = !nans[j] | RBounded;
    }
    for (int i=0;i<W;i++){
        result[i] = nans[i]&isLBounded[i]&isRBounded[i];
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
void arrcpyf(float a[W], const float b[W]){
    for (int i=0; i<W; i++){
        a[i]=b[i];
    }
}
void arrcpyll(long long a[W], const long long b[W]){
    for (int i=0; i<W; i++){
        a[i]=b[i];
    }
}
float ewma2(float cur, float prev, float decay_rate, float delta_t){
    if (DEBUGLEVEL>1) printf("ewma2 delta_t = %0.2f\n",delta_t);
    float lambda = powf(1.0-decay_rate, delta_t);
    return cur*(1.0-lambda)+prev*lambda;
}
float ewma(float cur, float prev, float decay_rate){
    return cur*decay_rate+prev*(1.0-decay_rate);
}
float mad(const float arr[W], const bool nans[W]){
    const float med = median(arr, nans);
    float absdiffs[W];
    for (int i=0; i<W; i++){
        absdiffs[i]=fabsf(arr[i]-med);
    }
    return median(absdiffs, nans);
}
void updateAnomalies(const float arr[W], const bool nans[W], bool result[W]){
    float median_ = median(arr, nans);
    float mad_ = mad(arr, nans);
    for (int i=0; i<W; i++){
        result[i]=fabsf(arr[i]-median_)>MADK*mad_;
    }
}
float get_rightmost_number(const float arr[W], const bool filter[W]){
    for (int i=0; i<W; i++){
        if (filter) continue;
        return arr[i];
    }
    return 0.0/0.0;
}
float get_leftmost_number(const float arr[W], const bool filter[W]){
    for (int i=0; i<W; i++){
        if (filter) continue;
        return arr[i];
    }
    return 0.0/0.0;
}
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
float _std(const float *x, const int _n){
    if (_n<=1){
        return 0;
    }
    double mean=0;
    double var=0;
    const double n = (double)_n;
    double n_nans = 0;
    for (int i=0;i<n;++i){
        if (is_nan(x[i])){
            n_nans+=1.0;
            continue;
        }
        const double xi = x[i];
        const double delta = xi-mean;
        mean += delta / ((double)i-n_nans+1.0);
        var += delta*(xi-mean);
    }
    return (float)(var/(n-1.0));
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
    if (argc != 5) { // Check if a filename was provided
        fprintf(stderr, "Usage: %s <filename> <corruption rate> <ewm coeff> <num runs>\n", argv[0]);
        return 1;
    }
    float p = atof(argv[2]);
    float decay_rate = atof(argv[3]);
    int n_runs = atoi(argv[4]);
    if ((p<0)||(p>1)){
        fprintf(stderr, "corruption rate must be between 0 and 1\n");
        return 1;
    }
    if ((decay_rate<0)||(decay_rate>1)){
        fprintf(stderr, "ewm coeff must be between 0 and 1\n");
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
    printf("MAE: %02.2f %c %02.2f\n", _mean(mae, n_runs), 241, _std(mae, n_runs));
    printf("RMSE: %02.2f %c %02.2f\n", _mean(rmse, n_runs), 241, _std(rmse, n_runs));
    free(mae);
    free(rmse);
    return 1;
}
struct Output inner(int argc, char *argv[]) {
    float p = atof(argv[2]);
    float decay_rate = atof(argv[3]);
    struct Output out;
    out.mae = 0.0/0.0;
    out.rmse = 0.0/0.0;
    if ((p<0)||(p>1)){
        fprintf(stderr, "corruption rate must be between 0 and 1\n");
        return out;
    }
    if ((decay_rate<0)||(decay_rate>1)){
        fprintf(stderr, "ewm coeff must be between 0 and 1\n");
        return out;
    }
    FILE *file;

    fopen_s(&file, argv[1], "r"); // Open the file specified in the command-line argument
    if (!file) {
        perror("Error opening file");
        return out;
    }
    long long timestamp;
    float target;
    
    float truths[W] = { [0 ... W-1] = 0.0 };
    float ewms[W] = { [0 ... W-1] = 0.0 };
    float values[W] = { [0 ... W-1] = 0.0 };
    long long timestamps[W] = { [0 ... W-1] = 0.0 };
    long long last_ewma_timestamp = -99999999999;

    bool nanOrAnomaly[W] = { [0 ... W-1] = false };
    bool nans[W] = { [0 ... W-1] = false };
    bool anomalies[W] = { [0 ... W-1] = false }; 
    bool interpolable[W] = { [0 ... W-1] = false };
    bool rfillable[W] = { [0 ... W-1] = false };
    bool lfillable[W] = { [0 ... W-1] = false };
    float stds[W] = { [0 ... W-1] = 0.0 };
    float varias[W] = { [0 ... W-1] = 0.0 };//variabilitas
    float interps[W][W] = { [0 ... W-1] = { [0 ... W-1] = (0.0/0.0) }};
    float cur_mad = 0.0/0.0;
    float cur_median = 0.0/0.0;
    

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
        ffillf(ewms);
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
        updateLFillable(nans, lfillable);
        updateRFillable(nans, rfillable);
        updateAnomalies(values, nans, anomalies);
        updateOr(nans, anomalies, nanOrAnomaly);
        cur_median = median(values, nanOrAnomaly);
        cur_mad = mad(values, nanOrAnomaly);
        // we divide by 600 because our default sample rate is 10 minutes
        // scratch that, i think 30 minutes is closer after looking at the data
        if (!anomalies[W-1])// btw removing this makes our avg loss go from 1.3 -> 0.6
        {//(!anomalies[W-1]&&!nans[W-1]){
            // do we update ewms only when the current value is not an anomaly? or also when it is also not nan? 
            // well, we wouldn't know if it's an anomaly if it's a nan...
            //ewms[W-1]=ewma2(values[W-1],ewms[W-2],decay_rate, (float)(timestamps[W-1]-last_ewma_timestamp)/SECOND_SCALE/(60.0*30.0));
            ewms[W-1]=ewma(values[W-1],ewms[W-2],decay_rate);
            last_ewma_timestamp=timestamp;
        }
        

        if (DEBUGLEVEL) printf("values: ");
        if (DEBUGLEVEL) aprintf(values);

        if (DEBUGLEVEL) printf("ewms:   ");
        if (DEBUGLEVEL) aprintf(ewms);

        if (DEBUGLEVEL) printf("nans:         ");
        if (DEBUGLEVEL) aprintb(nans);

        if (DEBUGLEVEL) printf("anomalies:    ");
        if (DEBUGLEVEL) aprintb(anomalies);

        if (DEBUGLEVEL) printf("interpolable: ");
        if (DEBUGLEVEL) aprintb(interpolable);
        
        float cur_variabilitas = variabilitas2(values, nanOrAnomaly);

        int num_nans = countTrue(nans);
        if ((0<num_nans)&&(num_nans<=W/2)){
            for (int i=1; i<W-1; i++){
                if (nans[i]==false) {
                    if (DEBUGLEVEL>1) printf("nans[%d] is false so skipping\n",i);
                    continue;
                };
                int interpIndex = (iters+i)%W;
                if (interpolable[i]==false) {
                    if (lfillable[i]^rfillable[i]){
                        if (lfillable[i]) interps[interpIndex][W-1] = get_leftmost_number(values, nanOrAnomaly);// if this doesnt work, use nans rather than nanOrAnomaly
                        if (rfillable[i]) interps[interpIndex][W-1] = get_rightmost_number(values, nanOrAnomaly);
                        if (DEBUGLEVEL) printf("assigned to interps[%d][-1]\n",interpIndex);
                    } else {
                        // this case happens 
                        if (DEBUGLEVEL>1) printf("interpolable is false... lfillable^rfillable is false... this is a wierd case\n");
                    }
                    continue;
                };
                if (cur_variabilitas>0.5) interps[interpIndex][W-1] = interpLagrange(timestamps, ewms, nans, timestamps[i]);
                else interps[interpIndex][W-1] = interpLagrange(timestamps, values, nans, timestamps[i]);
                if (DEBUGLEVEL) printf("assigned to interps[%d][-1]\n",interpIndex);
            }
        }
        if (DEBUGLEVEL) printf(" === interp table ===\n");
        if (DEBUGLEVEL) wwprintf(interps);
        if (DEBUGLEVEL) printf(" === ****** ***** ===\n");
        if ((nans[0]!=false) && (iters>1+W)){
            float pred = special_mean(interps[iters%W]);
            if (DEBUGLEVEL) printf("truth:pred = %02.2f:%02.2f\n", truths[0], pred);
            if (is_nan(pred)){//TODO: check if delta t > delta t limit: (also idk where to put this)
                if (PRINT) printf("%lld,nan\n",timestamps[0]);
                totalUninterpolated++;
                continue;
            }
            if (PRINT) printf("%lld,%f\n",timestamps[0],pred);
            totalSquaredError+=sqr(pred-truths[0]);
            totalAbsoluteError+=fabsf(pred-truths[0]);
        } else {
            if (DEBUGLEVEL) printf("truth:- = %02.2f -\n", truths[0]);
            if (PRINT) printf("%lld, %0.2f\n",timestamps[0],ewms[0]);
        }
        if (!isCorrupted){
        }
    }
    fclose(file);
    double sigMean = totalSignal/iters;
    double sigStd = sqrt((totalSquaredSignal-sqr(totalSignal)/iters)/(iters-1));
    float rmse = sqrtf(totalSquaredError/((float)totalNans));
    float mae = totalAbsoluteError/((float)totalNans);
    if (EVAL) {
        // printf("iters: %d\n", iters);
        // printf("data mean: %lf\n", sigMean);
        // printf("data std: %lf\n", sigStd);
        // printf("NaNs/samples: %02.2f%%\n", 100*(float)totalNans/(float)iters);
        // printf("uninterpolated/samples: %02.2f%%\n", 100*(float)totalUninterpolated/(float)iters);
        // printf("uninterpolated/NaNs: %02.2f%%\n", 100*(float)totalUninterpolated/(float)totalNans);
        // printf("RMSE: %02.5f\n", rmse);
        // printf("MAE: %02.5f\n", mae);
        out.rmse=rmse;
        out.mae=mae;
    }
    return out;
}

