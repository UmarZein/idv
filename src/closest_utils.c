#ifndef CLOSEST_UTILS
#define CLOSEST_UTILS
#include "./utils.c"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int idx;
    int dist;
} Candidate;

int cmpCandidates(const void *a, const void *b) {
    const Candidate *ca = (const Candidate*)a;
    const Candidate *cb = (const Candidate*)b;
    if (ca->dist != cb->dist) {
        return ca->dist - cb->dist; // closer first
    }
    return ca->idx - cb->idx;       // tie-break: smaller index first
}

void assignClosestMask(bool *mask, const int N, const int i, const bool *nans) {
    // 1. clear mask
    for (int j = 0; j < W; j++) {
        mask[j] = true;
    }

    // 2. collect candidates
    Candidate candidates[W];
    int count = 0;
    for (int j = 0; j < W; j++) {
        if (!nans[j]) { // only valid (non-nan)
            candidates[count].idx = j;
            candidates[count].dist = abs(j - i);
            count++;
        }
    }

    // 3. sort candidates by distance
    qsort(candidates, count, sizeof(Candidate), cmpCandidates);

    // 4. assign first N entries to mask
    int take = (N < count) ? N : count;
    for (int k = 0; k < take; k++) {
        mask[candidates[k].idx] = false;
    }
}

#endif
