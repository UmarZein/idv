// algo.h
#ifndef ALGOH 
#define ALGOH
typedef struct {
    bool can_search;
    bool (*done_searching)(void *self);
    void (*init_search)(void *self);
    void (*search_eval)(void *self);
    void (*search_step_begin)(void *self);
    void (*search_step_end)(void *self, const float total_error);
    void (*finish_search)(void *self);
    void (*predict)(void *self, float *out, const float *X, const float *Y, const bool *nans);
} Algorithm;

#endif
