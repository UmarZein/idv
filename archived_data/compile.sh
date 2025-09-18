#!/usr/bin/bash
# gcc -o p0 0code.c;
# gcc -o p2 2code.c;
# gcc -o p3 3code.c;
# gcc -o p4 4code.c;
# gcc -o p5 5code.c;
# gcc -o p6 6code.c;
# gcc -o p7 7code.c;
# gcc -o p8 8code.c;
# gcc -o p9 9code.c;
set -x
CR=0.3
MADK=1.4826
NRUNS=100000
CC=gcc
# gcc -o u1 u1code.c -lm;
# ./u1 sample.txt ${CR} ${MADK} ${NRUNS};
# gcc -o u1c cleaned_code.c -lm;
# ./u1c sample.txt ${CR} ${MADK} ${NRUNS};
# gcc -o u0 u0code.c -lm;
# ./u0 sample.txt ${CR} ${MADK} ${NRUNS};
${CC} -o u3 u3code.c -lm;
./u3.exe sample.txt ${CR} ${MADK} ${NRUNS};
${CC} -o u6 u6code.c -lm;
./u6.exe sample.txt ${CR} ${MADK} ${NRUNS};
${CC} -o u4 u4code.c -lm;
./u4.exe sample.txt ${CR} ${MADK} ${NRUNS};
set +x
