#ifndef LINEARC
#define LINEARC
#include "./utils.c"
#include "./closest_utils.c"
#include "algo.h"

void linear_predict(void *state, float* out, const float* X, const float* Y, const bool* nans) {
    const int n_nans = countTrue(nans);            
    int L = -1;
    int R = -1;
    // linear interp/extrapolation
    for (int i = 0; i < W; i++) {
        if (nans[i]) {
            L=-1;
            R=-1;
            bool closestMask[W] = {false};
            assignClosestMask(closestMask, 2, i, nans);
            for (int j=0; j<W; j++){
                if (!closestMask[j]) if (L!=-1) R=j;
                if (!closestMask[j]) if (L==-1) L=j;
            }
            
            if (L==R){
                printf("L should not be different from R");
            }
            float dx = X[R] - X[L];
            if (dx == 0.0f) {
                printf("dx should not be 0");
            }
// safety

            float grad = (Y[R] - Y[L]) / dx;
            float prediction = Y[L] + (X[i] - X[L]) * grad;
            out[i]=prediction;
        } else {
            out[i]=0.0/0.0;
        }
    }
}

Algorithm create_linear(){
    Algorithm model = {
        .predict = linear_predict
    };
    return model;
}
#endif
