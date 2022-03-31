#!/bin/bash

file=results_zero/ABMF2021001.out


awk '{if($1=="G01") print $3, $6, $7, $8, $9 }' $file > G01.txt
awk '{if($1=="G02") print $3, $6, $7, $8, $9 }' $file > G02.txt
awk '{if($1=="G03") print $3, $6, $7, $8, $9 }' $file > G03.txt
awk '{if($1=="G04") print $3, $6, $7, $8, $9 }' $file > G04.txt
awk '{if($1=="G05") print $3, $6, $7, $8, $9 }' $file > G05.txt
awk '{if($1=="G06") print $3, $6, $7, $8, $9 }' $file > G06.txt
awk '{if($1=="G07") print $3, $6, $7, $8, $9 }' $file > G07.txt
awk '{if($1=="G08") print $3, $6, $7, $8, $9 }' $file > G08.txt
awk '{if($1=="G09") print $3, $6, $7, $8, $9 }' $file > G09.txt
awk '{if($1=="G10") print $3, $6, $7, $8, $9 }' $file > G10.txt
awk '{if($1=="G11") print $3, $6, $7, $8, $9 }' $file > G11.txt
awk '{if($1=="G12") print $3, $6, $7, $8, $9 }' $file > G12.txt
awk '{if($1=="G13") print $3, $6, $7, $8, $9 }' $file > G13.txt
awk '{if($1=="G14") print $3, $6, $7, $8, $9 }' $file > G14.txt
awk '{if($1=="G15") print $3, $6, $7, $8, $9 }' $file > G15.txt
awk '{if($1=="G16") print $3, $6, $7, $8, $9 }' $file > G16.txt
awk '{if($1=="G17") print $3, $6, $7, $8, $9 }' $file > G17.txt
awk '{if($1=="G18") print $3, $6, $7, $8, $9 }' $file > G18.txt
awk '{if($1=="G19") print $3, $6, $7, $8, $9 }' $file > G19.txt
awk '{if($1=="G20") print $3, $6, $7, $8, $9 }' $file > G20.txt
awk '{if($1=="G20") print $3, $6, $7, $8, $9 }' $file > G20.txt
awk '{if($1=="G21") print $3, $6, $7, $8, $9 }' $file > G21.txt
awk '{if($1=="G22") print $3, $6, $7, $8, $9 }' $file > G22.txt
awk '{if($1=="G23") print $3, $6, $7, $8, $9 }' $file > G23.txt
awk '{if($1=="G24") print $3, $6, $7, $8, $9 }' $file > G24.txt
awk '{if($1=="G25") print $3, $6, $7, $8, $9 }' $file > G25.txt
awk '{if($1=="G26") print $3, $6, $7, $8, $9 }' $file > G26.txt
awk '{if($1=="G27") print $3, $6, $7, $8, $9 }' $file > G27.txt
awk '{if($1=="G28") print $3, $6, $7, $8, $9 }' $file > G28.txt
awk '{if($1=="G29") print $3, $6, $7, $8, $9 }' $file > G29.txt
awk '{if($1=="G30") print $3, $6, $7, $8, $9 }' $file > G30.txt
awk '{if($1=="G31") print $3, $6, $7, $8, $9 }' $file > G31.txt
awk '{if($1=="G32") print $3, $6, $7, $8, $9 }' $file > G32.txt
awk '{if($1=="SSS") print $3, $6, $7, $8 }' $file > DX.txt


gnuplot << EOF

set terminal png
set output "${file}.GPS.C1.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "C1(m) "

set key top outside horizontal center

plot "G01.txt" using 1:2 with linespoints title "G01", \
     "G02.txt" using 1:2 with linespoints title "G02", \
     "G03.txt" using 1:2 with linespoints title "G03", \
     "G04.txt" using 1:2 with linespoints title "G04", \
     "G05.txt" using 1:2 with linespoints title "G05", \
     "G06.txt" using 1:2 with linespoints title "G06", \
     "G07.txt" using 1:2 with linespoints title "G07", \
     "G08.txt" using 1:2 with linespoints title "G08", \
     "G09.txt" using 1:2 with linespoints title "G09", \
     "G10.txt" using 1:2 with linespoints title "G10", \
     "G11.txt" using 1:2 with linespoints title "G11", \
     "G12.txt" using 1:2 with linespoints title "G12", \
     "G13.txt" using 1:2 with linespoints title "G13", \
     "G14.txt" using 1:2 with linespoints title "G14", \
     "G15.txt" using 1:2 with linespoints title "G15", \
     "G16.txt" using 1:2 with linespoints title "G16", \
     "G17.txt" using 1:2 with linespoints title "G17", \
     "G18.txt" using 1:2 with linespoints title "G18", \
     "G19.txt" using 1:2 with linespoints title "G19", \
     "G20.txt" using 1:2 with linespoints title "G20", \
     "G21.txt" using 1:2 with linespoints title "G21", \
     "G23.txt" using 1:2 with linespoints title "G23", \
     "G22.txt" using 1:2 with linespoints title "G22", \
     "G24.txt" using 1:2 with linespoints title "G24", \
     "G25.txt" using 1:2 with linespoints title "G25", \
     "G26.txt" using 1:2 with linespoints title "G26", \
     "G27.txt" using 1:2 with linespoints title "G27", \
     "G28.txt" using 1:2 with linespoints title "G28", \
     "G29.txt" using 1:2 with linespoints title "G29", \
     "G30.txt" using 1:2 with linespoints title "G30", \
     "G31.txt" using 1:2 with linespoints title "G31", \
     "G32.txt" using 1:2 with linespoints title "G32"

set terminal png
set output "${file}.GPS.C2.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "C2(m) "

set key top outside horizontal center

plot "G01.txt" using 1:3 with linespoints title "G01", \
     "G02.txt" using 1:3 with linespoints title "G02", \
     "G03.txt" using 1:3 with linespoints title "G03", \
     "G04.txt" using 1:3 with linespoints title "G04", \
     "G05.txt" using 1:3 with linespoints title "G05", \
     "G06.txt" using 1:3 with linespoints title "G06", \
     "G07.txt" using 1:3 with linespoints title "G07", \
     "G08.txt" using 1:3 with linespoints title "G08", \
     "G09.txt" using 1:3 with linespoints title "G09", \
     "G10.txt" using 1:3 with linespoints title "G10", \
     "G11.txt" using 1:3 with linespoints title "G11", \
     "G12.txt" using 1:3 with linespoints title "G12", \
     "G13.txt" using 1:3 with linespoints title "G13", \
     "G14.txt" using 1:3 with linespoints title "G14", \
     "G15.txt" using 1:3 with linespoints title "G15", \
     "G16.txt" using 1:3 with linespoints title "G16", \
     "G17.txt" using 1:3 with linespoints title "G17", \
     "G18.txt" using 1:3 with linespoints title "G18", \
     "G19.txt" using 1:3 with linespoints title "G19", \
     "G20.txt" using 1:3 with linespoints title "G20", \
     "G21.txt" using 1:3 with linespoints title "G21", \
     "G23.txt" using 1:3 with linespoints title "G23", \
     "G22.txt" using 1:3 with linespoints title "G22", \
     "G24.txt" using 1:3 with linespoints title "G24", \
     "G25.txt" using 1:3 with linespoints title "G25", \
     "G26.txt" using 1:3 with linespoints title "G26", \
     "G27.txt" using 1:3 with linespoints title "G27", \
     "G28.txt" using 1:3 with linespoints title "G28", \
     "G29.txt" using 1:3 with linespoints title "G29", \
     "G30.txt" using 1:3 with linespoints title "G30", \
     "G31.txt" using 1:3 with linespoints title "G31", \
     "G32.txt" using 1:3 with linespoints title "G32"

set terminal png
set output "${file}.GPS.L1.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "L1(m) "

set key top outside horizontal center

plot "G01.txt" using 1:4 with linespoints title "G01", \
     "G02.txt" using 1:4 with linespoints title "G02", \
     "G03.txt" using 1:4 with linespoints title "G03", \
     "G04.txt" using 1:4 with linespoints title "G04", \
     "G05.txt" using 1:4 with linespoints title "G05", \
     "G06.txt" using 1:4 with linespoints title "G06", \
     "G07.txt" using 1:4 with linespoints title "G07", \
     "G08.txt" using 1:4 with linespoints title "G08", \
     "G09.txt" using 1:4 with linespoints title "G09", \
     "G10.txt" using 1:4 with linespoints title "G10", \
     "G11.txt" using 1:4 with linespoints title "G11", \
     "G12.txt" using 1:4 with linespoints title "G12", \
     "G13.txt" using 1:4 with linespoints title "G13", \
     "G14.txt" using 1:4 with linespoints title "G14", \
     "G15.txt" using 1:4 with linespoints title "G15", \
     "G16.txt" using 1:4 with linespoints title "G16", \
     "G17.txt" using 1:4 with linespoints title "G17", \
     "G18.txt" using 1:4 with linespoints title "G18", \
     "G19.txt" using 1:4 with linespoints title "G19", \
     "G20.txt" using 1:4 with linespoints title "G20", \
     "G21.txt" using 1:4 with linespoints title "G21", \
     "G23.txt" using 1:4 with linespoints title "G23", \
     "G22.txt" using 1:4 with linespoints title "G22", \
     "G24.txt" using 1:4 with linespoints title "G24", \
     "G25.txt" using 1:4 with linespoints title "G25", \
     "G26.txt" using 1:4 with linespoints title "G26", \
     "G27.txt" using 1:4 with linespoints title "G27", \
     "G28.txt" using 1:4 with linespoints title "G28", \
     "G29.txt" using 1:4 with linespoints title "G29", \
     "G30.txt" using 1:4 with linespoints title "G30", \
     "G31.txt" using 1:4 with linespoints title "G31", \
     "G32.txt" using 1:4 with linespoints title "G32"

set terminal png
set output "${file}.GPS.L2.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "L2(m) "

set key top outside horizontal center

plot "G01.txt" using 1:5 with linespoints title "G01", \
     "G02.txt" using 1:5 with linespoints title "G02", \
     "G03.txt" using 1:5 with linespoints title "G03", \
     "G04.txt" using 1:5 with linespoints title "G04", \
     "G05.txt" using 1:5 with linespoints title "G05", \
     "G06.txt" using 1:5 with linespoints title "G06", \
     "G07.txt" using 1:5 with linespoints title "G07", \
     "G08.txt" using 1:5 with linespoints title "G08", \
     "G09.txt" using 1:5 with linespoints title "G09", \
     "G10.txt" using 1:5 with linespoints title "G10", \
     "G11.txt" using 1:5 with linespoints title "G11", \
     "G12.txt" using 1:5 with linespoints title "G12", \
     "G13.txt" using 1:5 with linespoints title "G13", \
     "G14.txt" using 1:5 with linespoints title "G14", \
     "G15.txt" using 1:5 with linespoints title "G15", \
     "G16.txt" using 1:5 with linespoints title "G16", \
     "G17.txt" using 1:5 with linespoints title "G17", \
     "G18.txt" using 1:5 with linespoints title "G18", \
     "G19.txt" using 1:5 with linespoints title "G19", \
     "G20.txt" using 1:5 with linespoints title "G20", \
     "G21.txt" using 1:5 with linespoints title "G21", \
     "G23.txt" using 1:5 with linespoints title "G23", \
     "G22.txt" using 1:5 with linespoints title "G22", \
     "G24.txt" using 1:5 with linespoints title "G24", \
     "G25.txt" using 1:5 with linespoints title "G25", \
     "G26.txt" using 1:5 with linespoints title "G26", \
     "G27.txt" using 1:5 with linespoints title "G27", \
     "G28.txt" using 1:5 with linespoints title "G28", \
     "G29.txt" using 1:5 with linespoints title "G29", \
     "G30.txt" using 1:5 with linespoints title "G30", \
     "G31.txt" using 1:5 with linespoints title "G31", \
     "G32.txt" using 1:5 with linespoints title "G32"


set terminal png
set output "${file}.DX.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "DX(m) "

set key top outside horizontal center

plot "DX.txt" using 1:2 with linespoints title "X", \
     "DX.txt" using 1:3 with linespoints title "Y", \
     "DX.txt" using 1:4 with linespoints title "Z"

EOF

rm G[0-9][0-9].txt
#

