#ifndef POLYFULL
#define POLYFULL
#include "./utils.c"
#include "./closest_utils.c"
#include "algo.h"
#include "./flags.c"

typedef struct {
    Algorithm base;
    float mad_coeff;
    float decay_rate;
    float weights[W];

    float amp;
    float persistence;
    float best_weights[W];
    float best_weights_score;
    float best_contender_weights[W];
    float best_contender_score;
} PolynomialModel;

void predict_pf(void *_state, float* out, const float* X, const float* Y, const bool* nans) {
    PolynomialModel *state = (PolynomialModel*)_state;
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
            for (int deg=1; deg<n_non_missing; deg++){
                bool closestMask[W] = {false};
                //mask, N, i,nans
                assignClosestMask(closestMask, deg+1, i, X, nansOrAnomalies);
                prediction += state->weights[deg]*interpLagrange(X, ewmas, closestMask, (float)i);
            }
            out[i]=prediction;
        }
        else out[i]=0.0/0.0;
    }
}


void init_search_pf(void *_state){
    PolynomialModel *state = (PolynomialModel *)_state;
    fill0(state->weights);
    state->amp=1.3;
    state->persistence=0.8;

    fill0(state->best_weights);
    state->best_weights_score = -1e30f;

    fill0(state->best_contender_weights);
    state->best_contender_score = -1e10f;
}

void search_eval_pf(void *_state){
    PolynomialModel *state = (PolynomialModel *)_state;
    state->amp *= state->persistence;
    if (state->best_contender_score>state->best_weights_score){
        for (int i=0; i<W; i++){
            state->best_weights[i] = state->best_contender_weights[i];
        }
        state->best_weights_score = state->best_contender_score;
    }
}

bool done_searching_pf(void *_state){
    PolynomialModel *state = (PolynomialModel *)_state;
    return state->amp<0.01;
}
void search_step_begin_pf(void *_state){
    PolynomialModel *state = (PolynomialModel *)_state;
    for (int i=0; i<W; i++){
        state->weights[i] = rand_normal()*state->amp+state->best_weights[i];
    }
}
void search_step_end_pf(void *_state, const float total_error){
    PolynomialModel *state = (PolynomialModel *)_state;
    const float score = -total_error;
    if (score>state->best_contender_score){
        state->best_contender_score = score;
        for (int i=0; i<W; i++){
            state->best_contender_weights[i] = state->weights[i];
        }
    }
}

void finish_search_pf(void *_state){
    PolynomialModel *state = (PolynomialModel *)_state;
    for (int i=0; i<W; i++){
        state->weights[i] = state->best_weights[i];
    }
    printf("done searching! weights: ");
    aprintf(state->weights);
    printf("amp: %f\n", state->amp);
}

PolynomialModel create_polymodel(const Flags flags){
    PolynomialModel model;
    fill0(model.weights);
    model.mad_coeff = flags.mad_coeff;
    model.decay_rate = flags.ewma_decay_rate;
    fill0(model.weights);

    model.amp=1.0;
    model.persistence=0.8;
    fill0(model.best_weights);
    model.best_weights_score=-9e10f;
    fill0(model.best_contender_weights);
    model.best_contender_score=-9e10f;
    model.base.can_search = true;
    model.base.predict = predict_pf;
    model.base.init_search = init_search_pf;
    model.base.search_eval = search_eval_pf;
    model.base.search_step_begin = search_step_begin_pf;
    model.base.search_step_end = search_step_end_pf;
    model.base.finish_search = finish_search_pf;
    model.base.done_searching = done_searching_pf;
    return model;

}
#endif
