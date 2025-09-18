#include "./utils.c"
#include "./scaniter.c"
#include "./algo.h"
#include "./flags.c"
#include "./eval.c"

void search(const Flags flags, Algorithm *alg) {
    if (!alg->can_search) return;
    FILE *fp; // Open the file specified in the command-line argument
    ScanIter it;
    unsigned int rander = flags.rander;
    fopen_s(&fp, flags.filename, "r");
    if (!fp) {
        perror("Error opening fp");
    }

    // get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    // read into buffer
    char *buffer = (char *)malloc(size+1);
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);

    scanIterInit(&it, buffer);

    const float p = flags.p;
    const float mad_coeff = flags.mad_coeff;
    const int n_searches = flags.n_searches;
    const int BASE_SEARCH_ITERS = flags.base_search_iters;
    const int SEARCH_STEP_INTERVAL = flags.search_step_interval;

    alg->init_search(alg);
    float *search_mae = (float *)malloc(sizeof(float)*BASE_SEARCH_ITERS);
    float *search_rmse = (float *)malloc(sizeof(float)*BASE_SEARCH_ITERS);
    for (int search=1; search<n_searches; search++){
        alg->search_step_begin(alg);
        if ((search%SEARCH_STEP_INTERVAL)==0){
            alg->search_eval(alg);
            if (alg->done_searching(alg)) break;
        }
        for (int i=0; i<BASE_SEARCH_ITERS; i++){
            struct Output out = test(flags, &it, alg);
            srand(rander);
            rander=hash(rander);
            search_mae[i]=out.mae;
            search_rmse[i]=out.rmse;
        }
        const float mae_score = _mean(search_mae, BASE_SEARCH_ITERS)*_std(search_mae, BASE_SEARCH_ITERS);
        const float rmse_score = _mean(search_rmse, BASE_SEARCH_ITERS)*_std(search_rmse, BASE_SEARCH_ITERS);
        const float total_error = mae_score + rmse_score;
        
        alg->search_step_end(alg, total_error);
    }

    alg->finish_search(alg);
    free(buffer);
    free(search_mae);
    free(search_rmse);
}

