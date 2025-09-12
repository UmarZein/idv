// #include "./utils_rand_search.c"
// struct Output{
//     float mae;
//     float rmse;
//     float ratio_solved;
// };
// struct Output _inner(int, char*[], FILE*, float[W]);
// int main(int argc, char *argv[]) {
//     srand(time(NULL));
//     unsigned int rander=0xdeadbeef^time(NULL);
//     if (argc != 5) { // Check if a filename was provided
//         fprintf(stderr, "Usage: %s <filename> <corruption rate> <mad coefficient> <num searches>\n", argv[0]);
//         return 1;
//     }
//     const float p = atof(argv[2]);
//     const float mad_coeff = atof(argv[3]);
//     const int n_searches = atoi(argv[4]);
//     const int BASE_SEARCH_ITERS = 1000;
//     const int SEARCH_STEP_INTERVAL = 100;
//     float amp=0.1;
//     const float DECAY_RATE=0.95;
//     if ((p<0)||(p>1)){
//         fprintf(stderr, "corruption rate must be between 0 and 1\n");
//         return 1;
//     }
//     if ((mad_coeff<=0)){
//         fprintf(stderr, "mad coefficient should be higher than 0\n");
//         return 1;
//     }
// 
//     FILE *fp = fopen(argv[1], "r"); // Open the file specified in the command-line argument
//     if (!fp) {
//         perror("Error opening fp");
//         return 1;
//     }
// 
//     // get file size
//     fseek(fp, 0, SEEK_END);
//     long size = ftell(fp);
//     rewind(fp);
// 
//     // read into buffer
//     char *buffer = malloc(size+1);
//     fread(buffer, 1, size, fp);
//     buffer[size] = '\0';
//     fclose(fp);
// 
//     float weights[W] = {9e100, 0.62659, -0.00779, -0.11531, 0.91490, 1.43686};
//     float best_weights[W] = {0.0};
//     float best_weights_error = 1e300;
//     float contendor_weights[W] = {0.0};
//     float contendor_weights_error = 1e300;
//     float *search_mae = malloc(sizeof(float)*BASE_SEARCH_ITERS);
//     float *search_rmse = malloc(sizeof(float)*BASE_SEARCH_ITERS);
//     for (int search=0; search<n_searches; search++){
//         if ((search%SEARCH_STEP_INTERVAL)==0){
//             amp *= DECAY_RATE;
//             if (contendor_weights_error<best_weights_error){
//                 for (int i=0; i<W; i++){
//                     best_weights[i] = contendor_weights[i];
//                 }
//                 best_weights_error = contendor_weights_error;
//                 printf("amp: %f\n", amp);
//                 printf("best error: %f\n", best_weights_error);
//                 aprintf(best_weights);
//                 float ws=0.0;
//                 for (int k=1; k<W; k++) ws+=best_weights[k];
//                 printf("weight sum: %f\n", ws);
//                 printf("----------\n");
//             }
//         }
//         if (search>0){
//             for (int i=0; i<W; i++){
//                 weights[i] = rand_normal()*amp+best_weights[i];
//             }
//         }
//         for (int i=0; i<BASE_SEARCH_ITERS; i++){
//             FILE *memfp = fmemopen(buffer, size, "r");
//             struct Output out = _inner(argc, argv, memfp, weights);
//             fclose(memfp);
//             srand(rander);
//             rander=hash(rander);
//             search_mae[i]=out.mae;
//             search_rmse[i]=out.rmse;
//         }
//         const float mae_score = _mean(search_mae, BASE_SEARCH_ITERS)*_std(search_mae, BASE_SEARCH_ITERS);
//         const float rmse_score = _mean(search_rmse, BASE_SEARCH_ITERS)*_std(search_rmse, BASE_SEARCH_ITERS);
//         const float total_error = mae_score + rmse_score;
//         
//         if (total_error<contendor_weights_error){
//             for (int i=0; i<W; i++){
//                 contendor_weights[i] = weights[i];
//             }
//             contendor_weights_error = total_error;
//         }
//     }
// 
// 
//     free(buffer);
//     free(search_mae);
//     free(search_rmse);
//     return 1;
// }

