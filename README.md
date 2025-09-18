# Cara compile

```
gcc -o program src/main.c
```

# Cara running

contoh dengan banten_rr.csv file harus dengan format `timestamp,value` pada setiap line

```
./main -f data/banten_rr.csv -p 0.3 --mad_coeff 3.5 --n_runs 100 --n_searches 1000 --base_search_iters 5 --search_step_i
nterval 15 --ewma_decay_rate 0.5 --persistence 0.7
```

deskripsi parameter:

```
-p: probabilitas nan dari 0 sampai 1
--mad_coeff: koefisien threshold mad yang digunakan untuk mendeteksi anomali. lebih tinggi, berarti lebih dikit data yang diflag menjadi anomali
--n_runs: hitungan run pada evaluasi post-training
--n_searches: hitungan iterasi training
--base_search_iter: hitungan iterasi setiap step training. Apabila nilalinya 10, maka pada setiap iterasi training, contender weights pada time-t (W_t) akan dicoba 10 kali dan dihitung rata-rata MAE dan RMSE nya, kemudian dihitung skor-nya tergantung itu
--search_step_interval: berapa banyak iterasi sebelum weights model diupdate menggunakan weights contender terbaik
--ewma_decay_rate: koefisien EWMA
--persistence: koefisien simulated annealing dari 0 sampai 1. Lebih rendah nilai ini, berarti temperatur pada training menurun lebih cepat
```

