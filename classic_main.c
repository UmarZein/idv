#include "./utils.c"
#include <time.h>
struct Output{
    float mae;
    float rmse;
    float ratio_solved;
};
struct Output;
struct Output _inner(int, char*[], FILE*, float[W]);
struct Output inner(int, char*[], FILE*);
int classic_main(int argc, char *argv[]) {
    srand(time(NULL));
    unsigned int rander=0xdeadbeef^time(NULL);
    if (argc != 5) { // Check if a filename was provided
        fprintf(stderr, "Usage: %s <filename> <corruption rate> <mad coefficient> <num runs>\n", argv[0]);
        return 1;
    }
    float p = atof(argv[2]);
    float mad_coeff = atof(argv[3]);
    int n_runs = atoi(argv[4]);
    if ((p<0)||(p>1)){
        fprintf(stderr, "corruption rate must be between 0 and 1\n");
        return 1;
    }
    if ((mad_coeff<=0)){
        fprintf(stderr, "mad coefficient should be higher than 0\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r"); // Open the file specified in the command-line argument
    if (!fp) {
        perror("Error opening fp");
        return 1;
    }

    // get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    // read into buffer
    char *buffer = malloc(size+1);
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);



    float *mae = malloc(sizeof(float)*n_runs);
    float *rmse = malloc(sizeof(float)*n_runs);
    float *ratio_solved = malloc(sizeof(float)*n_runs);
    for (int i=0; i<n_runs; i++){
        // open memory buffer as FILE*
        FILE *memfp = fmemopen(buffer, size, "r");
        struct Output out = inner(argc, argv, memfp);
        fclose(memfp);
        srand(rander);
        rander=hash(rander);
        mae[i]=out.mae;
        rmse[i]=out.rmse;
        ratio_solved[i]=out.ratio_solved;
    }
    free(buffer);
    printf("MAE: mean=%02.2f std=%02.2f\n", _mean(mae, n_runs), _std(mae, n_runs));
    printf("RMSE: mean=%02.2f std=%02.2f\n", _mean(rmse, n_runs), _std(rmse, n_runs));
    printf("Ratio Solved: mean=%02.2f std=%02.2f\n", _mean(ratio_solved, n_runs), _std(ratio_solved, n_runs));
    printf("freeing %ld bytes of memory...\n", sizeof(float)*n_runs*3);
    free(mae);
    free(rmse);
    return 1;
}

