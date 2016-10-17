#!/bin/bash

DATA_FILE=$(mktemp)

function populateFile () 
{
    ./build/baker > $DATA_FILE 
}

#echo "0 0" >> $DATA_FILE

populateFile &

sleep 2

gnuplot -e \
"set xrange [];
set yrange [15:75];
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0;
while (1) { plot \"$DATA_FILE\" with linespoints ls 1; pause 1; }"


