#!/bin/bash
set -x
./p0 subset_stabil_banten_rr.csv 0.3 5000
./p0 subset_nonstabil_banten_rr.csv 0.3 5000
./p2 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p2 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
./p3 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p3 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
./p4 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p4 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
./p5 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p5 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
./p6 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p6 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
./p7 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p7 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
./p8 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p8 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
./p9 subset_stabil_banten_rr.csv 0.3 0.5 5000
./p9 subset_nonstabil_banten_rr.csv 0.3 0.5 5000
set +x
# N=$2  # Change this to however many times you want to run
# rmse_list=()
# mae_list=()
# if [[ "$2" == "" ]]; then
#    echo "guide: eval.sh <script> <N runs>"
#    return
# fi
# for ((i = 0; i < N; i++)); do
#     # Capture output
#     output=$($1 subset_nonstabil_banten_rr.csv 0.3 0.5)

#     # Extract RMSE and MAE using grep + awk
#     rmse=$(echo "$output" | grep "RMSE:" | awk '{print $2}')
#     mae=$(echo "$output" | grep "MAE:" | awk '{print $2}')

#     rmse_list+=($rmse)
#     mae_list+=($mae)
# done

# # Convert to space-separated strings for processing
# rmse_array="${rmse_list[@]}"
# mae_array="${mae_list[@]}"

# # Use awk to compute mean and std dev
# echo "RMSE stats:"
# echo "${rmse_array[@]}" | awk '{
#     n = NF;
#     for (i = 1; i <= NF; i++) {
#         sum += $i;
#         sumsq += $i * $i;
#     }
#     mean = sum / n;
#     stddev = sqrt(sumsq / n - mean * mean);
#     printf "Mean: %.5f\nStd Dev: %.5f\n", mean, stddev;
# }'

# echo "MAE stats:"
# echo "${mae_array[@]}" | awk '{
#     n = NF;
#     for (i = 1; i <= NF; i++) {
#         sum += $i;
#         sumsq += $i * $i;
#     }
#     mean = sum / n;
#     stddev = sqrt(sumsq / n - mean * mean);
#     printf "Mean: %.5f\nStd Dev: %.5f\n", mean, stddev;
# }'

