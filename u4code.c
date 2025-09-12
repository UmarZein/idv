//u0code.c (aka lerp) except we enable extrapolation
#include "./utils.c"
#include "./closest_utils.c"
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
        iters++;
        truths[iters%W]=target;
        // tumbling mechanism (only process every W step)
        if (iters%W==0){
            generate_random_mask(nans, p);
            int n_nans = countTrue(nans);            
            while ((W-n_nans)<2){
                generate_random_mask(nans, p);
                n_nans = countTrue(nans);            
            }
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
                    float dx = timestamps[R] - timestamps[L];
                    if (dx == 0.0f) {
                        printf("dx should not be 0");
                    }
 // safety

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
    out.mae =mae;
    out.ratio_solved=solved/total;
    return out;
}
