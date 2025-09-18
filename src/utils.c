#ifndef UTILS_C
#define UTILS_C
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef ELEM_SWAP
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
    float arr[W] = { [0 ... W-1] = NAN };
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
        printf("%05.5f ",array[i]);
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

//float mean(const float array[W]){
//    float total = 0;
//    float n = 0;
//    for (int i=0;i<W;i++){
//        if (is_nan(array[i])) continue;
//        total = total + array[i];
//        n = n + 1.0;
//    }
//    return total/n;
//}

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

//float std(const float array[W]){
//    float se = 0;
//    for (int i=0;i<W;i++){
//        if (is_nan(array[i])) continue;
//        se = se + sqr(array[i]);
//    }
//    return sqrtf(se/(float)W);
//}
float std2(const float array[W], const bool filter[W]){
    float se = 0;
    for (int i=0;i<W;i++){
        if (filter[i]) continue;
        se = se + sqr(array[i]);
    }
    return sqrtf(se/(float)W);
}

// float variabilitas(const float array[W]){
//     float totalSquares = 0;
//     float total = 0;
//     for (int i=0;i<W;i++){
//         if (is_nan(array[i])) continue;
//         totalSquares = totalSquares + sqr(array[i]);
//         total = total + array[i];
//     }
//     return fabsf(sqrtf(totalSquares/(float)W)*(float)W/total);
// }

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

float _interpLagrange(const long long x[W],const float y[W], const bool nans[W],const long long z){
    long long normalized_x[W];
    long long first = x[0];
    long long last = x[W-1];
    for (int i=0;i<W;i++){
        normalized_x[i] = (x[i]-first);
    }
    
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
        result += term;
    }
    return result;
}

// if y=f(x), predict what f(z) is using interpolation
float interpLagrange(const float x[W],const float y[W], const bool nans[W],const float z){
    if (DEBUGLEVEL>2) printf("interpLagrange(x[W], y[W], nans[W], %f)\n", z);
    if (DEBUGLEVEL>2) printf("x: ");
    float normalized_x[W];
    float first = x[0];
    float last = x[W-1];
    for (int i=0;i<W;i++){
        normalized_x[i] = (x[i]-first);
    }
    if (DEBUGLEVEL>2) aprintf(x);
    if (DEBUGLEVEL>2) printf("y: ");
    if (DEBUGLEVEL>2) aprintf(y);
    if (DEBUGLEVEL>2) printf("nans: ");
    if (DEBUGLEVEL>2) aprintb(nans);
    
    float x_valid[W];  // Valid x values
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
                term *= (z - x_valid[j]) / (x_valid[i] - x_valid[j]);
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
void fill0(float arr[W]){
    for (int i=0;i<W;i++){
        arr[i]= 0.0;
    }
}
float get_leftmost_number(const float arr[W], const bool filter[W]){
    for (int i=0; i<W; i++){
        if (filter) continue;
        return arr[i];
    }
    return 0.0/0.0;
}
float _mean(const float *x, const int n){
    float mean=0;
    const float _n = (float)n;
    for (int i=0;i<n;i++){
        if (!is_nan(x[i])) mean=mean+(x[i]-mean)/_n;
    }
    return mean;
}
// Uniform random in (0,1)
static inline float urand() {
    return (rand() + 1.0f) / (RAND_MAX + 2.0f);
}
#ifndef M_PI
#define M_PI 3.14159
#endif

#ifndef M_E
#define M_E 2.71828
#endif

// N(0,1) using Box-Muller
float rand_normal() {
    float u1 = urand();
    float u2 = urand();
    return sqrtf(-2.0f * logf(u1)) * cosf(2.0f * M_PI * u2);
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
void generate_random_mask(bool *ptr, float p){
    for (int i=0; i<W; i++){
        ptr[i] = bernoulli_sample(p);
    }
}
void clone_bool(const bool *src, bool *tgt){
    for (int i=0; i<W; i++){
        tgt[i]=src[i];
    }
}
void clone_float(const float *src, float *tgt){
    for (int i=0; i<W; i++){
        tgt[i]=src[i];
    }
}

// Comparator helper for qsort, ignores NaN-masked values
static int cmp_floats(const void *a, const void *b) {
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    return (fa > fb) - (fa < fb); // standard float comparison
}

void sort_masked(float vals[], bool nan_mask[]) {
    // First count how many real (non-NaN) values there are
    size_t real_count = 0;
    for (size_t i = 0; i < W; i++) {
        if (!nan_mask[i]) {
            real_count++;
        }
    }

    if (real_count == 0 || real_count == W) {
        // Either all NaNs or no NaNs -> nothing special to do
        qsort(vals, W, sizeof(float), cmp_floats);
        return;
    }

    // Collect real values in a temp buffer
    float *real_vals = (float*) malloc(real_count * sizeof(float));
    if (!real_vals) return; // handle alloc fail
    size_t idx = 0;
    for (size_t i = 0; i < W; i++) {
        if (!nan_mask[i]) {
            real_vals[idx++] = vals[i];
        }
    }

    // Sort real values
    qsort(real_vals, real_count, sizeof(float), cmp_floats);

    // Put them back into vals, and push masked values to the end
    idx = 0;
    for (size_t i = 0; i < W; i++) {
        if (idx < real_count) {
            vals[i] = real_vals[idx++];
            nan_mask[i] = false;
        } else {
            nan_mask[i] = true;
            // vals[i] can be left unchanged or set to NAN if you like
        }
    }

    free(real_vals);
}

#endif

