#ifndef FLAGS
#define FLAGS
#include <stdlib.h>
#include <string.h>
typedef struct{
    const char* filename;
    int rander;
    float p;
    float mad_coeff;
    float ewma_decay_rate;
    int n_runs;
    int n_searches;
    int base_search_iters;
    int search_step_interval;
} Flags;

bool startswith(const char *str, const char *prefix){
    return strncmp(prefix, str, strlen(prefix)) == 0;
}
#define Err -1
#define Ok 1
int assignFlags(Flags *f, int argc, char *argv[]){
    for (int i=1; i<argc; i++){
        if (strcmp(argv[i], "-f")==0){
            i++;
            f->filename=argv[i];
            continue;
        }
        if (strcmp(argv[i], "-p")==0){
            i++;
            float p = atof(argv[i]);
            if ((p<0)||(p>1)){
                fprintf(stderr, "corruption rate must be between 0 and 1\n");
                return Err;
            }
            f->p=p;
            continue;
        }
        if (strcmp(argv[i], "--mad_coeff")==0){
            i++;
            float mad_coeff = atof(argv[i]);
            if ((mad_coeff<=0)){
                fprintf(stderr, "mad coefficient should be higher than 0\n");
                return Err;
            }
            f->mad_coeff=mad_coeff;
            continue;
        }
        if (strcmp(argv[i], "--n_runs")==0){
            i++;
            f->n_runs=atoi(argv[i]);
            continue;
        }
        if (strcmp(argv[i], "--n_searches")==0){
            i++;
            f->n_searches=atoi(argv[i]);
            continue;
        }
        if (strcmp(argv[i], "--base_search_iters")==0){
            i++;
            f->base_search_iters=atoi(argv[i]);
            continue;
        }
        if (strcmp(argv[i], "--search_step_interval")==0){
            i++;
            f->search_step_interval=atoi(argv[i]);
            continue;
        }
        if (strcmp(argv[i], "--ewma_decay_rate")==0){
            i++;
            float ewma_decay_rate = atof(argv[i]);
            if ((ewma_decay_rate<0)||(ewma_decay_rate>1)){
                fprintf(stderr, "ewma decay rate must be between 0 and 1\n");
                return Err;
            }
            f->ewma_decay_rate=ewma_decay_rate;
            continue;
        }
    }
    return Ok;
}
#endif
