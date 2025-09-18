#include "./utils.c"
#include "./closest_utils.c"
#include "algo.h"

typedef struct {
    float mad_coeff;
    float decay_rate;
    float *weights;
} PolynomialHybrid;

void poly2_predict(void *_state, float* out, const float* X, const float* Y, const bool* nans) {
    PolynomialHybrid *state = (PolynomialHybrid*)_state;
    const int n_nans = countTrue(nans);            
    bool nansOrAnomalies[W] = {false};
    float cur_mad = 0.0/0.0;
    float cur_median = 0.0/0.0;
    float cur_tao = 0.0/0.0;
    float cur_variabilty = 0.0/0.0;
    const int n_non_missing = W-n_nans;
    // # anomaly detection phase
    cur_median = median(Y, nans);
    cur_mad = mad(Y, nans);//median absolute deviation
    cur_tao = state->mad_coeff*cur_mad;
    for (int i=0;i<W;i++){
        const float ad = fabsf(Y[i]-cur_median);
        const bool anomaly=ad>cur_tao;
        nansOrAnomalies[i]=anomaly||nans[i];
    }
    
    int L=-1;
    int R=-1;
    float leftmost=0.0/0.0;
    float rightmost;
    for (int i=0;i<W;i++){
        const float ad = fabsf(Y[i]-cur_median);
        const bool anomaly=ad>cur_tao;
        const bool nan_or_anomaly=anomaly||nans[i];
        nansOrAnomalies[i]=nan_or_anomaly;
        if (!nan_or_anomaly){
            if (L==-1){
                L=i;
                leftmost=Y[i];
            }
            R=i;
            rightmost=Y[i];
        }
    }
    
    // # variability checking phase
    cur_variabilty=variabilitas2(Y, nansOrAnomalies);

    float ewmas[W]={0.0};
    for (int i=0; i<W; i++)
        ewmas[i]=Y[i];
    
    bool hit_leftmost=false;
    float carried_number=leftmost;
    if (cur_variabilty>0.5){
        for (int i=0; i<W; i++){
            if (!hit_leftmost){
                if (nansOrAnomalies[i]) ewmas[i]=leftmost;
                else {
                    hit_leftmost=true;
                    ewmas[i]=leftmost;
                }
            }
            else{
                if (!nansOrAnomalies[i]) carried_number=Y[i];
                ewmas[i]=state->decay_rate*ewmas[i-1]+(1.0-state->decay_rate)*carried_number;
            }
        }
    }
    
    // skipping to MILI-UD
    for (int i=0; i<W; i++){
        if (nansOrAnomalies[i]){
            float prediction = 0;
            if (i<L) prediction=leftmost;
            else if (i>R) prediction=rightmost;
            else for (int deg=1; deg<n_non_missing; deg++){
                bool closestMask[W] = {false};
                //mask, N, i,nans
                assignClosestMask(closestMask, deg+1, i, nansOrAnomalies);
                prediction += state->weights[deg]*interpLagrange(X, ewmas, closestMask, (float)i);
            }
            out[i]=prediction;
        }
        else out[i]=0.0/0.0;
    }
}
