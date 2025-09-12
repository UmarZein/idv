//u3code.c except we use u5
#include "./utils.c"
#define NOTSEARCHING
#include "./classic_main.c"
#include "./u5code.c"
float weights[W] = {-7.96030, 1.05543, -0.04011, -0.04387, 0.04606, 2.38006};
int main(int argc, char *argv[]) {
    classic_main(argc, argv);
}
struct Output inner(int argc, char *argv[], FILE* file) {
    return _inner(argc, argv, file, weights);
}
