#include "./utils.c"
#include "./classic_main.c"
int main(int argc, char *argv[]) {
    classic_main(argc, argv);
}
struct Output inner(int argc, char *argv[], FILE* file) {
    const float p = atof(argv[2]);//probability of missing data
    const float mad_coeff = atof(argv[3]);// c*MAD <- the c is `mad_coeff`
    struct Output out;
    out.mae = 0.0/0.0;
    out.rmse = 0.0/0.0;
    out.ratio_solved = 0.0/0.0;
    if ((p<0)||(p>1)){
        fprintf(stderr, "corruption rate must be between 0 and 1\n");
        return out;
    }
    float timestamp;
    float target;
    
    float truths[W] = {0.0};
    float timestamps[W] = {0.0};
    for (int i=0;i<W;i++){
        timestamps[i]=(float)(i);
    }

    bool nans[W] = {false};
    int L=-1;
    int R=-1;
    
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
            // # anomaly detection phase
            // skipping to MILI-UD
            for (int i = 0; i < W; i++) {
                if (nans[i]) {
                    L = -1;
                    R = -1;

                    for (int j = 0; j < W; j++) {
                        if (j < i && !nans[j]) L = j;                     // last non-nan left
                        if (j > i && R == -1 && !nans[j] && j != L) R = j; // first non-nan right, not equal to L
                    }

                    if (L == -1 || R == -1 || L == R) continue;  // need two distinct neighbors

                    float dx = timestamps[R] - timestamps[L];
                    if (dx == 0.0f) continue; // safety

                    float grad = (truths[R] - truths[L]) / dx;
                    float prediction = truths[L] + (timestamps[i] - timestamps[L]) * grad;
                    float error = prediction - truths[i];

                    totalSquaredError += error * error;
                    totalSquaredErrorDivisor += 1.0f;
                    totalAbsoluteError += fabsf(error);
                    totalAbsoluteErrorDivisor += 1.0f;
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
