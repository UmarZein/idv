#include "./utils.c"
#include "./closest_utils.c"

#ifndef NOTSEARCHING
#include "./search_main.c"
int main(int argc, char *argv[]) {
    search_main(argc, argv);
}
#endif

struct Output _inner(int argc, char *argv[], FILE* file, float* weights) {
    const float p = atof(argv[2]);//probability of missing data
    const float mad_coeff = atof(argv[3]);// c*MAD <- the c is `mad_coeff`
    const int n_searches = atoi(argv[4]);
    struct Output out;
    float timestamp;
    float target;
    
    float truths[W] = {0.0};
    float timestamps[W] = {0.0};
    for (int i=0;i<W;i++){
        timestamps[i]=(float)(i);
    }

    bool nans[W] = {false};
    bool nansOrAnomalies[W] = {false};
    float cur_mad = 0.0/0.0;
    float cur_median = 0.0/0.0;
    float cur_tao = 0.0/0.0;
    float cur_variabilty = 0.0/0.0;
    
    float totalSquaredError = 0.0;
    float totalSquaredErrorDivisor = 0.0;
    float totalAbsoluteError = 0.0;
    float totalAbsoluteErrorDivisor = 0.0;
    int iters=0;
    float solved=0;
    float total=0;

    // mainloop is here.
    while (fscanf(file, "%f,%f", &timestamp, &target) == 2) {
        // we assume time will always have uniform dt=1, so actually timestamp variable is unused
        // because interpLagrange is invariant to X-translation
        iters++;
        // # pre-processing phase
        truths[iters%W]=target;
        // tumbling mechanism (only process every W step)
        if (iters%W==0){
            generate_random_mask(nans, p);
            int n_nans = countTrue(nans);            
            while ((W-n_nans)<2){
                generate_random_mask(nans, p);
                n_nans = countTrue(nans);            
            }
            const int n_non_missing = W-n_nans;
            // # anomaly detection phase
            cur_median = median(truths, nans);
            cur_mad = mad(truths, nans);//median absolute deviation
            cur_tao = mad_coeff*cur_mad;
            for (int i=0;i<W;i++){
                const float ad = fabsf(truths[i]-cur_median);
                const bool anomaly=ad>cur_tao;
                nansOrAnomalies[i]=anomaly||nans[i];
            }
            
            // # variability checking phase
            cur_variabilty=variabilitas2(truths, nansOrAnomalies);

            // TODO: heteroscedasticity check (for now assume always false)
            
            // skipping to MILI-UD
            for (int i=0; i<W; i++){
                if (nansOrAnomalies[i]){
                    float prediction = 0;
                    for (int deg=1; deg<n_non_missing; deg++){
                        bool closestMask[W] = {false};
                        //mask, N, i,nans
                        assignClosestMask(closestMask, deg+1, i, nansOrAnomalies);
                        prediction += weights[deg]*interpLagrange(timestamps, truths, closestMask, (float)i);
                    }
                    const float error = prediction-truths[i];
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
