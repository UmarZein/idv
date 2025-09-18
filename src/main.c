#include "./classic_main.c"
#include "./search_main.c"
#include "./polyhybrid.c"
#include "./polyfull.c"
#include "./linear.c"
#include "algo.h"
#include <time.h>
int main(int argc, char *argv[]) {
    // srand(42);
    // const unsigned int rander = 32;
    const unsigned int rander=0xdeadbeef^time(NULL);
    srand(rander);
    Flags flags;
    if (assignFlags(&flags, argc, argv)==Err){
        perror("Error assigning flags\n");
        return 1;
    }

    flags.rander = rander;
    srand(flags.rander);
    PolynomialHybrid hybrid = create_polyhybrid(flags);
    printf("initialized hybrid\n");
    search(flags, (Algorithm *)&hybrid);
    printf("searched parameters\n");
    flags.rander = rander;
    classic_main(flags, (Algorithm *)&hybrid);
    printf("done\n");

    flags.rander = rander;
    srand(flags.rander);
    PolynomialModel polyfull = create_polymodel(flags);
    printf("initialized polyfull\n");
    search(flags, (Algorithm *)&polyfull);
    printf("searched parameters\n");
    flags.rander = rander;
    classic_main(flags, (Algorithm *)&polyfull);
    printf("done\n");
    
    flags.rander = rander;
    srand(flags.rander);
    Algorithm linear = create_linear();
    printf("initialized linear\n");
    classic_main(flags, (Algorithm *)&linear);
    printf("done\n");
    // printf(" === linear model === \n");
    // Algorithm model; 
    // model.predict = linear_predict;
    // flags.rander = rander;
    // classic_main(flags, &model);

    // printf(" === polynomial model === \n");
    // PolynomialModel pfullstate;
    // float weights1[W] = {9e10, 0.62659, -0.00779, -0.11531, 0.91490, 1.43686};
    // pfullstate.weights = weights1;
    // pfullstate.mad_coeff = flags.mad_coeff;
    // pfullstate.decay_rate = flags.ewma_decay_rate;
    // model.predict = polyfull_predict;
    // model.state = &pfullstate;
    // flags.rander = rander;
    // classic_main(flags, &model);

    // printf(" === hybrid polynomial model === \n");
    // PolynomialModel phybrstate;
    // float weights2[W] = {-7.96030, 1.05543, -0.04011, -0.04387, 0.04606, 2.38006};
    // phybrstate.weights = weights1;
    // phybrstate.mad_coeff = flags.mad_coeff;
    // phybrstate.decay_rate = flags.ewma_decay_rate;
    // model.predict = poly2_predict;
    // model.state = &phybrstate;
    // flags.rander = rander;
    // classic_main(flags, &model);

    // printf(" === linear model === \n");
    // model.predict = linear_predict;
    // flags.rander = rander;
    // classic_main(flags, &model);

}

