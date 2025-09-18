// algo.h
typedef struct {
    void *state;
    void (*predict)(void *state, float *out, const float *X, const float *Y, const bool *nans);
} Algorithm;

