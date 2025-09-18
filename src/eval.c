#ifndef EVALC
#define EVALC
#include "./flags.c"
#include "./scaniter.c"
#include "./algo.h"
#include "./utils.c"
struct Output{
    float mae;
    float rmse;
    float ratio_solved;
};

struct Output test(const Flags flags, ScanIter *iterator, Algorithm *alg) {
    resetScanIter(iterator);
    struct Output out;
    float timestamp;
    float target;
    
    float truths[W] = {0.0};
    float values[W] = {0.0};
    float timestamps[W] = {0.0};
    for (int i=0;i<W;i++){
        timestamps[i]=(float)(i);
    }

    bool nans[W] = {false};
    
    float totalSquaredError = 0.0;
    float totalSquaredErrorDivisor = 0.0;
    float totalAbsoluteError = 0.0;
    float totalAbsoluteErrorDivisor = 0.0;
    int iters=0;
    float solved=0;
    float total=0;

    // mainloop is here.
    while (scanIter(iterator, "%f,%f", &timestamp, &target) == 2) {
        // we assume time will always have uniform dt=1, so actually timestamp variable is unused
        // because interpLagrange is invariant to X-translation
        iters++;
        // # pre-processing phase
        truths[iters%W]=target;
        // tumbling mechanism (only process every W step)
        if (iters%W==0){
            generate_random_mask(nans, flags.p);
            int n_nans = countTrue(nans);            
            while ((W-n_nans)<2){
                generate_random_mask(nans, flags.p);
                n_nans = countTrue(nans);            
            }
            for (int i=0;i<W;i++){
                if (nans[i]){
                    values[i]=0.0/0.0;
                } else values[i]=truths[i];
            }

            // void, float, const float, const float, const bool
            float out[W] = {};
            alg->predict(alg, out, timestamps, values, nans);
            
            // skipping to MILI-UD
            for (int i=0; i<W; i++){
                if (nans[i]&&!is_nan(out[i])){
                    const float error = out[i]-truths[i];
                    const float squaredError = error*error;

                    totalSquaredError += squaredError;
                    totalSquaredErrorDivisor += 1.0;
                    totalAbsoluteError += fabsf(error);
                    totalAbsoluteErrorDivisor += 1.0;
                    solved += 1;
                }
                total += 1;
            }
        }
    }
    float rmse = sqrtf(totalSquaredError/((float)totalSquaredErrorDivisor));
    float mae = totalAbsoluteError/((float)totalAbsoluteErrorDivisor);
    out.rmse=rmse;
    out.mae=mae;
    out.ratio_solved=solved/total;
    return out;
}

#endif
