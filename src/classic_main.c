#include "./utils.c"
#include "./eval.c"
#include "./flags.c"
#include "./linear.c"
#include "./polyfull.c"
#include "./polyhybrid.c"

int classic_main(const Flags flags, Algorithm *alg){
    FILE *fp; // Open the file specified in the command-line argument
    ScanIter it;
    unsigned int rander = flags.rander;
    fopen_s(&fp, flags.filename, "r");
    if (!fp) {
        perror("Error opening fp");
        return 1;
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

    float *mae = (float *)malloc(sizeof(float)*flags.n_runs);
    float *rmse = (float *)malloc(sizeof(float)*flags.n_runs);
    float *ratio_solved = (float *)malloc(sizeof(float)*flags.n_runs);
    for (int i=0; i<flags.n_runs; i++){
        // open memory buffer as FILE*
        srand(rander);
        struct Output out = test(flags, &it, alg);
        rander=hash(rander);
        mae[i]=out.mae;
        rmse[i]=out.rmse;
        ratio_solved[i]=out.ratio_solved;
    }
    free(buffer);
    printf("MAE: mean=%02.2f std=%02.2f\n", _mean(mae, flags.n_runs), _std(mae, flags.n_runs));
    printf("RMSE: mean=%02.2f std=%02.2f\n", _mean(rmse, flags.n_runs), _std(rmse, flags.n_runs));
    printf("Ratio Solved: mean=%02.2f std=%02.2f\n", _mean(ratio_solved, flags.n_runs), _std(ratio_solved, flags.n_runs));
    printf("freeing %lld bytes of memory...\n", sizeof(float)*flags.n_runs*3);
    free(mae);
    free(rmse);
    free(ratio_solved);
    return 1;
}

