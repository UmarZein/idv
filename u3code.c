//u2code except searching is done
#include "./utils.c"
#define NOTSEARCHING
#include "./classic_main.c"
#include "./u2code.c"
float weights[W] = {9e100, 0.62659, -0.00779, -0.11531, 0.91490, 1.43686};
int main(int argc, char *argv[]) {
    classic_main(argc, argv);
}
struct Output inner(int argc, char *argv[], FILE* file) {
    return _inner(argc, argv, file, weights);
}
