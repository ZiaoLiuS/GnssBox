#!/bin/bash

file=result.txt


awk '{if($5=="G01") print $3, $6, $7, $8, $9 }' $file > G01.txt
awk '{if($5=="G02") print $3, $6, $7, $8, $9 }' $file > G02.txt
awk '{if($5=="G03") print $3, $6, $7, $8, $9 }' $file > G03.txt
awk '{if($5=="G04") print $3, $6, $7, $8, $9 }' $file > G04.txt
awk '{if($5=="G05") print $3, $6, $7, $8, $9 }' $file > G05.txt
awk '{if($5=="G06") print $3, $6, $7, $8, $9 }' $file > G06.txt
awk '{if($5=="G07") print $3, $6, $7, $8, $9 }' $file > G07.txt
awk '{if($5=="G08") print $3, $6, $7, $8, $9 }' $file > G08.txt
awk '{if($5=="G09") print $3, $6, $7, $8, $9 }' $file > G09.txt
awk '{if($5=="G10") print $3, $6, $7, $8, $9 }' $file > G10.txt
awk '{if($5=="G11") print $3, $6, $7, $8, $9 }' $file > G11.txt
awk '{if($5=="G12") print $3, $6, $7, $8, $9 }' $file > G12.txt
awk '{if($5=="G13") print $3, $6, $7, $8, $9 }' $file > G13.txt
awk '{if($5=="G14") print $3, $6, $7, $8, $9 }' $file > G14.txt
awk '{if($5=="G15") print $3, $6, $7, $8, $9 }' $file > G15.txt
awk '{if($5=="G16") print $3, $6, $7, $8, $9 }' $file > G16.txt
awk '{if($5=="G17") print $3, $6, $7, $8, $9 }' $file > G17.txt
awk '{if($5=="G18") print $3, $6, $7, $8, $9 }' $file > G18.txt
awk '{if($5=="G19") print $3, $6, $7, $8, $9 }' $file > G19.txt
awk '{if($5=="G20") print $3, $6, $7, $8, $9 }' $file > G20.txt
awk '{if($5=="G20") print $3, $6, $7, $8, $9 }' $file > G20.txt
awk '{if($5=="G21") print $3, $6, $7, $8, $9 }' $file > G21.txt
awk '{if($5=="G22") print $3, $6, $7, $8, $9 }' $file > G22.txt
awk '{if($5=="G23") print $3, $6, $7, $8, $9 }' $file > G23.txt
awk '{if($5=="G24") print $3, $6, $7, $8, $9 }' $file > G24.txt
awk '{if($5=="G25") print $3, $6, $7, $8, $9 }' $file > G25.txt
awk '{if($5=="G26") print $3, $6, $7, $8, $9 }' $file > G26.txt
awk '{if($5=="G27") print $3, $6, $7, $8, $9 }' $file > G27.txt
awk '{if($5=="G28") print $3, $6, $7, $8, $9 }' $file > G28.txt
awk '{if($5=="G29") print $3, $6, $7, $8, $9 }' $file > G29.txt
awk '{if($5=="G30") print $3, $6, $7, $8, $9 }' $file > G30.txt
awk '{if($5=="G31") print $3, $6, $7, $8, $9 }' $file > G31.txt
awk '{if($5=="G32") print $3, $6, $7, $8, $9 }' $file > G32.txt


gnuplot << EOF

set terminal png
set output "${file}.GPS.dx.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dx(m) "

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
set output "${file}.GPS.dy.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dy(m) "

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
set output "${file}.GPS.dz.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dz(m) "

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
set output "${file}.GPS.dt.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dz(m) "

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
EOF

rm G[0-9][0-9].txt
#

awk '{if($5=="E01") print $3, $6, $7, $8, $9 }' $file > E01.txt
awk '{if($5=="E02") print $3, $6, $7, $8, $9 }' $file > E02.txt
awk '{if($5=="E03") print $3, $6, $7, $8, $9 }' $file > E03.txt
awk '{if($5=="E04") print $3, $6, $7, $8, $9 }' $file > E04.txt
awk '{if($5=="E05") print $3, $6, $7, $8, $9 }' $file > E05.txt
awk '{if($5=="E06") print $3, $6, $7, $8, $9 }' $file > E06.txt
awk '{if($5=="E07") print $3, $6, $7, $8, $9 }' $file > E07.txt
awk '{if($5=="E08") print $3, $6, $7, $8, $9 }' $file > E08.txt
awk '{if($5=="E09") print $3, $6, $7, $8, $9 }' $file > E09.txt
awk '{if($5=="E10") print $3, $6, $7, $8, $9 }' $file > E10.txt
awk '{if($5=="E11") print $3, $6, $7, $8, $9 }' $file > E11.txt
awk '{if($5=="E12") print $3, $6, $7, $8, $9 }' $file > E12.txt
awk '{if($5=="E13") print $3, $6, $7, $8, $9 }' $file > E13.txt
awk '{if($5=="E14") print $3, $6, $7, $8, $9 }' $file > E14.txt
awk '{if($5=="E15") print $3, $6, $7, $8, $9 }' $file > E15.txt
awk '{if($5=="E16") print $3, $6, $7, $8, $9 }' $file > E16.txt
awk '{if($5=="E17") print $3, $6, $7, $8, $9 }' $file > E17.txt
awk '{if($5=="E18") print $3, $6, $7, $8, $9 }' $file > E18.txt
awk '{if($5=="E19") print $3, $6, $7, $8, $9 }' $file > E19.txt
awk '{if($5=="E20") print $3, $6, $7, $8, $9 }' $file > E20.txt
awk '{if($5=="E20") print $3, $6, $7, $8, $9 }' $file > E20.txt
awk '{if($5=="E21") print $3, $6, $7, $8, $9 }' $file > E21.txt
awk '{if($5=="E22") print $3, $6, $7, $8, $9 }' $file > E22.txt
awk '{if($5=="E23") print $3, $6, $7, $8, $9 }' $file > E23.txt
awk '{if($5=="E24") print $3, $6, $7, $8, $9 }' $file > E24.txt
awk '{if($5=="E25") print $3, $6, $7, $8, $9 }' $file > E25.txt
awk '{if($5=="E26") print $3, $6, $7, $8, $9 }' $file > E26.txt
awk '{if($5=="E27") print $3, $6, $7, $8, $9 }' $file > E27.txt
awk '{if($5=="E28") print $3, $6, $7, $8, $9 }' $file > E28.txt
awk '{if($5=="E29") print $3, $6, $7, $8, $9 }' $file > E29.txt
awk '{if($5=="E30") print $3, $6, $7, $8, $9 }' $file > E30.txt
awk '{if($5=="E31") print $3, $6, $7, $8, $9 }' $file > E31.txt
awk '{if($5=="E32") print $3, $6, $7, $8, $9 }' $file > E32.txt


gnuplot << EOF

set terminal png
set output "${file}.GAL.dx.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dx(m) "

set key top outside horizontal center

plot "E01.txt" using 1:2 with linespoints title "E01", \
     "E02.txt" using 1:2 with linespoints title "E02", \
     "E03.txt" using 1:2 with linespoints title "E03", \
     "E04.txt" using 1:2 with linespoints title "E04", \
     "E05.txt" using 1:2 with linespoints title "E05", \
     "E06.txt" using 1:2 with linespoints title "E06", \
     "E07.txt" using 1:2 with linespoints title "E07", \
     "E08.txt" using 1:2 with linespoints title "E08", \
     "E09.txt" using 1:2 with linespoints title "E09", \
     "E10.txt" using 1:2 with linespoints title "E10", \
     "E11.txt" using 1:2 with linespoints title "E11", \
     "E12.txt" using 1:2 with linespoints title "E12", \
     "E13.txt" using 1:2 with linespoints title "E13", \
     "E14.txt" using 1:2 with linespoints title "E14", \
     "E15.txt" using 1:2 with linespoints title "E15", \
     "E16.txt" using 1:2 with linespoints title "E16", \
     "E17.txt" using 1:2 with linespoints title "E17", \
     "E18.txt" using 1:2 with linespoints title "E18", \
     "E19.txt" using 1:2 with linespoints title "E19", \
     "E20.txt" using 1:2 with linespoints title "E20", \
     "E21.txt" using 1:2 with linespoints title "E21", \
     "E23.txt" using 1:2 with linespoints title "E23", \
     "E22.txt" using 1:2 with linespoints title "E22", \
     "E24.txt" using 1:2 with linespoints title "E24", \
     "E25.txt" using 1:2 with linespoints title "E25", \
     "E26.txt" using 1:2 with linespoints title "E26", \
     "E27.txt" using 1:2 with linespoints title "E27", \
     "E28.txt" using 1:2 with linespoints title "E28", \
     "E29.txt" using 1:2 with linespoints title "E29", \
     "E30.txt" using 1:2 with linespoints title "E30", \
     "E31.txt" using 1:2 with linespoints title "E31", \
     "E32.txt" using 1:2 with linespoints title "E32"

set terminal png
set output "${file}.GAL.dy.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dy(m) "

set key top outside horizontal center

plot "E01.txt" using 1:3 with linespoints title "E01", \
     "E02.txt" using 1:3 with linespoints title "E02", \
     "E03.txt" using 1:3 with linespoints title "E03", \
     "E04.txt" using 1:3 with linespoints title "E04", \
     "E05.txt" using 1:3 with linespoints title "E05", \
     "E06.txt" using 1:3 with linespoints title "E06", \
     "E07.txt" using 1:3 with linespoints title "E07", \
     "E08.txt" using 1:3 with linespoints title "E08", \
     "E09.txt" using 1:3 with linespoints title "E09", \
     "E10.txt" using 1:3 with linespoints title "E10", \
     "E11.txt" using 1:3 with linespoints title "E11", \
     "E12.txt" using 1:3 with linespoints title "E12", \
     "E13.txt" using 1:3 with linespoints title "E13", \
     "E14.txt" using 1:3 with linespoints title "E14", \
     "E15.txt" using 1:3 with linespoints title "E15", \
     "E16.txt" using 1:3 with linespoints title "E16", \
     "E17.txt" using 1:3 with linespoints title "E17", \
     "E18.txt" using 1:3 with linespoints title "E18", \
     "E19.txt" using 1:3 with linespoints title "E19", \
     "E20.txt" using 1:3 with linespoints title "E20", \
     "E21.txt" using 1:3 with linespoints title "E21", \
     "E23.txt" using 1:3 with linespoints title "E23", \
     "E22.txt" using 1:3 with linespoints title "E22", \
     "E24.txt" using 1:3 with linespoints title "E24", \
     "E25.txt" using 1:3 with linespoints title "E25", \
     "E26.txt" using 1:3 with linespoints title "E26", \
     "E27.txt" using 1:3 with linespoints title "E27", \
     "E28.txt" using 1:3 with linespoints title "E28", \
     "E29.txt" using 1:3 with linespoints title "E29", \
     "E30.txt" using 1:3 with linespoints title "E30", \
     "E31.txt" using 1:3 with linespoints title "E31", \
     "E32.txt" using 1:3 with linespoints title "E32"

set terminal png
set output "${file}.GAL.dz.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dz(m) "

set key top outside horizontal center

plot "E01.txt" using 1:4 with linespoints title "E01", \
     "E02.txt" using 1:4 with linespoints title "E02", \
     "E03.txt" using 1:4 with linespoints title "E03", \
     "E04.txt" using 1:4 with linespoints title "E04", \
     "E05.txt" using 1:4 with linespoints title "E05", \
     "E06.txt" using 1:4 with linespoints title "E06", \
     "E07.txt" using 1:4 with linespoints title "E07", \
     "E08.txt" using 1:4 with linespoints title "E08", \
     "E09.txt" using 1:4 with linespoints title "E09", \
     "E10.txt" using 1:4 with linespoints title "E10", \
     "E11.txt" using 1:4 with linespoints title "E11", \
     "E12.txt" using 1:4 with linespoints title "E12", \
     "E13.txt" using 1:4 with linespoints title "E13", \
     "E14.txt" using 1:4 with linespoints title "E14", \
     "E15.txt" using 1:4 with linespoints title "E15", \
     "E16.txt" using 1:4 with linespoints title "E16", \
     "E17.txt" using 1:4 with linespoints title "E17", \
     "E18.txt" using 1:4 with linespoints title "E18", \
     "E19.txt" using 1:4 with linespoints title "E19", \
     "E20.txt" using 1:4 with linespoints title "E20", \
     "E21.txt" using 1:4 with linespoints title "E21", \
     "E23.txt" using 1:4 with linespoints title "E23", \
     "E22.txt" using 1:4 with linespoints title "E22", \
     "E24.txt" using 1:4 with linespoints title "E24", \
     "E25.txt" using 1:4 with linespoints title "E25", \
     "E26.txt" using 1:4 with linespoints title "E26", \
     "E27.txt" using 1:4 with linespoints title "E27", \
     "E28.txt" using 1:4 with linespoints title "E28", \
     "E29.txt" using 1:4 with linespoints title "E29", \
     "E30.txt" using 1:4 with linespoints title "E30", \
     "E31.txt" using 1:4 with linespoints title "E31", \
     "E32.txt" using 1:4 with linespoints title "E32"

set terminal png
set output "${file}.GAL.dt.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dz(m) "

set key top outside horizontal center

plot "E01.txt" using 1:5 with linespoints title "E01", \
     "E02.txt" using 1:5 with linespoints title "E02", \
     "E03.txt" using 1:5 with linespoints title "E03", \
     "E04.txt" using 1:5 with linespoints title "E04", \
     "E05.txt" using 1:5 with linespoints title "E05", \
     "E06.txt" using 1:5 with linespoints title "E06", \
     "E07.txt" using 1:5 with linespoints title "E07", \
     "E08.txt" using 1:5 with linespoints title "E08", \
     "E09.txt" using 1:5 with linespoints title "E09", \
     "E10.txt" using 1:5 with linespoints title "E10", \
     "E11.txt" using 1:5 with linespoints title "E11", \
     "E12.txt" using 1:5 with linespoints title "E12", \
     "E13.txt" using 1:5 with linespoints title "E13", \
     "E14.txt" using 1:5 with linespoints title "E14", \
     "E15.txt" using 1:5 with linespoints title "E15", \
     "E16.txt" using 1:5 with linespoints title "E16", \
     "E17.txt" using 1:5 with linespoints title "E17", \
     "E18.txt" using 1:5 with linespoints title "E18", \
     "E19.txt" using 1:5 with linespoints title "E19", \
     "E20.txt" using 1:5 with linespoints title "E20", \
     "E21.txt" using 1:5 with linespoints title "E21", \
     "E23.txt" using 1:5 with linespoints title "E23", \
     "E22.txt" using 1:5 with linespoints title "E22", \
     "E24.txt" using 1:5 with linespoints title "E24", \
     "E25.txt" using 1:5 with linespoints title "E25", \
     "E26.txt" using 1:5 with linespoints title "E26", \
     "E27.txt" using 1:5 with linespoints title "E27", \
     "E28.txt" using 1:5 with linespoints title "E28", \
     "E29.txt" using 1:5 with linespoints title "E29", \
     "E30.txt" using 1:5 with linespoints title "E30", \
     "E31.txt" using 1:5 with linespoints title "E31", \
     "E32.txt" using 1:5 with linespoints title "E32"
EOF

rm E[0-9][0-9].txt
#

awk '{if($5=="R01") print $3, $6, $7, $8, $9 }' $file > R01.txt
awk '{if($5=="R02") print $3, $6, $7, $8, $9 }' $file > R02.txt
awk '{if($5=="R03") print $3, $6, $7, $8, $9 }' $file > R03.txt
awk '{if($5=="R04") print $3, $6, $7, $8, $9 }' $file > R04.txt
awk '{if($5=="R05") print $3, $6, $7, $8, $9 }' $file > R05.txt
awk '{if($5=="R06") print $3, $6, $7, $8, $9 }' $file > R06.txt
awk '{if($5=="R07") print $3, $6, $7, $8, $9 }' $file > R07.txt
awk '{if($5=="R08") print $3, $6, $7, $8, $9 }' $file > R08.txt
awk '{if($5=="R09") print $3, $6, $7, $8, $9 }' $file > R09.txt
awk '{if($5=="R10") print $3, $6, $7, $8, $9 }' $file > R10.txt
awk '{if($5=="R11") print $3, $6, $7, $8, $9 }' $file > R11.txt
awk '{if($5=="R12") print $3, $6, $7, $8, $9 }' $file > R12.txt
awk '{if($5=="R13") print $3, $6, $7, $8, $9 }' $file > R13.txt
awk '{if($5=="R14") print $3, $6, $7, $8, $9 }' $file > R14.txt
awk '{if($5=="R15") print $3, $6, $7, $8, $9 }' $file > R15.txt
awk '{if($5=="R16") print $3, $6, $7, $8, $9 }' $file > R16.txt
awk '{if($5=="R17") print $3, $6, $7, $8, $9 }' $file > R17.txt
awk '{if($5=="R18") print $3, $6, $7, $8, $9 }' $file > R18.txt
awk '{if($5=="R19") print $3, $6, $7, $8, $9 }' $file > R19.txt
awk '{if($5=="R20") print $3, $6, $7, $8, $9 }' $file > R20.txt
awk '{if($5=="R20") print $3, $6, $7, $8, $9 }' $file > R20.txt
awk '{if($5=="R21") print $3, $6, $7, $8, $9 }' $file > R21.txt
awk '{if($5=="R22") print $3, $6, $7, $8, $9 }' $file > R22.txt
awk '{if($5=="R23") print $3, $6, $7, $8, $9 }' $file > R23.txt
awk '{if($5=="R24") print $3, $6, $7, $8, $9 }' $file > R24.txt
awk '{if($5=="R25") print $3, $6, $7, $8, $9 }' $file > R25.txt
awk '{if($5=="R26") print $3, $6, $7, $8, $9 }' $file > R26.txt
awk '{if($5=="R27") print $3, $6, $7, $8, $9 }' $file > R27.txt
awk '{if($5=="R28") print $3, $6, $7, $8, $9 }' $file > R28.txt
awk '{if($5=="R29") print $3, $6, $7, $8, $9 }' $file > R29.txt
awk '{if($5=="R30") print $3, $6, $7, $8, $9 }' $file > R30.txt
awk '{if($5=="R31") print $3, $6, $7, $8, $9 }' $file > R31.txt
awk '{if($5=="R32") print $3, $6, $7, $8, $9 }' $file > R32.txt


gnuplot << ROF

set terminal png
set output "${file}.GLO.dx.png"

set terminal png size 2100,1300
set xlabel "Rpoch (seconds)"
set ylabel "dx(m) "

set key top outside horizontal center

plot "R01.txt" using 1:2 with linespoints title "R01", \
     "R02.txt" using 1:2 with linespoints title "R02", \
     "R03.txt" using 1:2 with linespoints title "R03", \
     "R04.txt" using 1:2 with linespoints title "R04", \
     "R05.txt" using 1:2 with linespoints title "R05", \
     "R06.txt" using 1:2 with linespoints title "R06", \
     "R07.txt" using 1:2 with linespoints title "R07", \
     "R08.txt" using 1:2 with linespoints title "R08", \
     "R09.txt" using 1:2 with linespoints title "R09", \
     "R10.txt" using 1:2 with linespoints title "R10", \
     "R11.txt" using 1:2 with linespoints title "R11", \
     "R12.txt" using 1:2 with linespoints title "R12", \
     "R13.txt" using 1:2 with linespoints title "R13", \
     "R14.txt" using 1:2 with linespoints title "R14", \
     "R15.txt" using 1:2 with linespoints title "R15", \
     "R16.txt" using 1:2 with linespoints title "R16", \
     "R17.txt" using 1:2 with linespoints title "R17", \
     "R18.txt" using 1:2 with linespoints title "R18", \
     "R19.txt" using 1:2 with linespoints title "R19", \
     "R20.txt" using 1:2 with linespoints title "R20", \
     "R21.txt" using 1:2 with linespoints title "R21", \
     "R23.txt" using 1:2 with linespoints title "R23", \
     "R22.txt" using 1:2 with linespoints title "R22", \
     "R24.txt" using 1:2 with linespoints title "R24", \
     "R25.txt" using 1:2 with linespoints title "R25", \
     "R26.txt" using 1:2 with linespoints title "R26", \
     "R27.txt" using 1:2 with linespoints title "R27", \
     "R28.txt" using 1:2 with linespoints title "R28", \
     "R29.txt" using 1:2 with linespoints title "R29", \
     "R30.txt" using 1:2 with linespoints title "R30", \
     "R31.txt" using 1:2 with linespoints title "R31", \
     "R32.txt" using 1:2 with linespoints title "R32"

set terminal png
set output "${file}.GLO.dy.png"

set terminal png size 2100,1300
set xlabel "Rpoch (seconds)"
set ylabel "dy(m) "

set key top outside horizontal center

plot "R01.txt" using 1:3 with linespoints title "R01", \
     "R02.txt" using 1:3 with linespoints title "R02", \
     "R03.txt" using 1:3 with linespoints title "R03", \
     "R04.txt" using 1:3 with linespoints title "R04", \
     "R05.txt" using 1:3 with linespoints title "R05", \
     "R06.txt" using 1:3 with linespoints title "R06", \
     "R07.txt" using 1:3 with linespoints title "R07", \
     "R08.txt" using 1:3 with linespoints title "R08", \
     "R09.txt" using 1:3 with linespoints title "R09", \
     "R10.txt" using 1:3 with linespoints title "R10", \
     "R11.txt" using 1:3 with linespoints title "R11", \
     "R12.txt" using 1:3 with linespoints title "R12", \
     "R13.txt" using 1:3 with linespoints title "R13", \
     "R14.txt" using 1:3 with linespoints title "R14", \
     "R15.txt" using 1:3 with linespoints title "R15", \
     "R16.txt" using 1:3 with linespoints title "R16", \
     "R17.txt" using 1:3 with linespoints title "R17", \
     "R18.txt" using 1:3 with linespoints title "R18", \
     "R19.txt" using 1:3 with linespoints title "R19", \
     "R20.txt" using 1:3 with linespoints title "R20", \
     "R21.txt" using 1:3 with linespoints title "R21", \
     "R23.txt" using 1:3 with linespoints title "R23", \
     "R22.txt" using 1:3 with linespoints title "R22", \
     "R24.txt" using 1:3 with linespoints title "R24", \
     "R25.txt" using 1:3 with linespoints title "R25", \
     "R26.txt" using 1:3 with linespoints title "R26", \
     "R27.txt" using 1:3 with linespoints title "R27", \
     "R28.txt" using 1:3 with linespoints title "R28", \
     "R29.txt" using 1:3 with linespoints title "R29", \
     "R30.txt" using 1:3 with linespoints title "R30", \
     "R31.txt" using 1:3 with linespoints title "R31", \
     "R32.txt" using 1:3 with linespoints title "R32"

set terminal png
set output "${file}.GLO.dz.png"

set terminal png size 2100,1300
set xlabel "Rpoch (seconds)"
set ylabel "dz(m) "

set key top outside horizontal center

plot "R01.txt" using 1:4 with linespoints title "R01", \
     "R02.txt" using 1:4 with linespoints title "R02", \
     "R03.txt" using 1:4 with linespoints title "R03", \
     "R04.txt" using 1:4 with linespoints title "R04", \
     "R05.txt" using 1:4 with linespoints title "R05", \
     "R06.txt" using 1:4 with linespoints title "R06", \
     "R07.txt" using 1:4 with linespoints title "R07", \
     "R08.txt" using 1:4 with linespoints title "R08", \
     "R09.txt" using 1:4 with linespoints title "R09", \
     "R10.txt" using 1:4 with linespoints title "R10", \
     "R11.txt" using 1:4 with linespoints title "R11", \
     "R12.txt" using 1:4 with linespoints title "R12", \
     "R13.txt" using 1:4 with linespoints title "R13", \
     "R14.txt" using 1:4 with linespoints title "R14", \
     "R15.txt" using 1:4 with linespoints title "R15", \
     "R16.txt" using 1:4 with linespoints title "R16", \
     "R17.txt" using 1:4 with linespoints title "R17", \
     "R18.txt" using 1:4 with linespoints title "R18", \
     "R19.txt" using 1:4 with linespoints title "R19", \
     "R20.txt" using 1:4 with linespoints title "R20", \
     "R21.txt" using 1:4 with linespoints title "R21", \
     "R23.txt" using 1:4 with linespoints title "R23", \
     "R22.txt" using 1:4 with linespoints title "R22", \
     "R24.txt" using 1:4 with linespoints title "R24", \
     "R25.txt" using 1:4 with linespoints title "R25", \
     "R26.txt" using 1:4 with linespoints title "R26", \
     "R27.txt" using 1:4 with linespoints title "R27", \
     "R28.txt" using 1:4 with linespoints title "R28", \
     "R29.txt" using 1:4 with linespoints title "R29", \
     "R30.txt" using 1:4 with linespoints title "R30", \
     "R31.txt" using 1:4 with linespoints title "R31", \
     "R32.txt" using 1:4 with linespoints title "R32"

set terminal png
set output "${file}.GLO.dt.png"

set terminal png size 2100,1300
set xlabel "Rpoch (seconds)"
set ylabel "dz(m) "

set key top outside horizontal center

plot "R01.txt" using 1:5 with linespoints title "R01", \
     "R02.txt" using 1:5 with linespoints title "R02", \
     "R03.txt" using 1:5 with linespoints title "R03", \
     "R04.txt" using 1:5 with linespoints title "R04", \
     "R05.txt" using 1:5 with linespoints title "R05", \
     "R06.txt" using 1:5 with linespoints title "R06", \
     "R07.txt" using 1:5 with linespoints title "R07", \
     "R08.txt" using 1:5 with linespoints title "R08", \
     "R09.txt" using 1:5 with linespoints title "R09", \
     "R10.txt" using 1:5 with linespoints title "R10", \
     "R11.txt" using 1:5 with linespoints title "R11", \
     "R12.txt" using 1:5 with linespoints title "R12", \
     "R13.txt" using 1:5 with linespoints title "R13", \
     "R14.txt" using 1:5 with linespoints title "R14", \
     "R15.txt" using 1:5 with linespoints title "R15", \
     "R16.txt" using 1:5 with linespoints title "R16", \
     "R17.txt" using 1:5 with linespoints title "R17", \
     "R18.txt" using 1:5 with linespoints title "R18", \
     "R19.txt" using 1:5 with linespoints title "R19", \
     "R20.txt" using 1:5 with linespoints title "R20", \
     "R21.txt" using 1:5 with linespoints title "R21", \
     "R23.txt" using 1:5 with linespoints title "R23", \
     "R22.txt" using 1:5 with linespoints title "R22", \
     "R24.txt" using 1:5 with linespoints title "R24", \
     "R25.txt" using 1:5 with linespoints title "R25", \
     "R26.txt" using 1:5 with linespoints title "R26", \
     "R27.txt" using 1:5 with linespoints title "R27", \
     "R28.txt" using 1:5 with linespoints title "R28", \
     "R29.txt" using 1:5 with linespoints title "R29", \
     "R30.txt" using 1:5 with linespoints title "R30", \
     "R31.txt" using 1:5 with linespoints title "R31", \
     "R32.txt" using 1:5 with linespoints title "R32"
ROF

rm R[0-9][0-9].txt
#

awk '{if($5=="C01") print $3, $6, $7, $8, $9 }' $file > C01.txt
awk '{if($5=="C02") print $3, $6, $7, $8, $9 }' $file > C02.txt
awk '{if($5=="C03") print $3, $6, $7, $8, $9 }' $file > C03.txt
awk '{if($5=="C04") print $3, $6, $7, $8, $9 }' $file > C04.txt
awk '{if($5=="C05") print $3, $6, $7, $8, $9 }' $file > C05.txt
awk '{if($5=="C06") print $3, $6, $7, $8, $9 }' $file > C06.txt
awk '{if($5=="C07") print $3, $6, $7, $8, $9 }' $file > C07.txt
awk '{if($5=="C08") print $3, $6, $7, $8, $9 }' $file > C08.txt
awk '{if($5=="C09") print $3, $6, $7, $8, $9 }' $file > C09.txt
awk '{if($5=="C10") print $3, $6, $7, $8, $9 }' $file > C10.txt
awk '{if($5=="C11") print $3, $6, $7, $8, $9 }' $file > C11.txt
awk '{if($5=="C12") print $3, $6, $7, $8, $9 }' $file > C12.txt
awk '{if($5=="C13") print $3, $6, $7, $8, $9 }' $file > C13.txt
awk '{if($5=="C14") print $3, $6, $7, $8, $9 }' $file > C14.txt
awk '{if($5=="C15") print $3, $6, $7, $8, $9 }' $file > C15.txt
awk '{if($5=="C16") print $3, $6, $7, $8, $9 }' $file > C16.txt
awk '{if($5=="C17") print $3, $6, $7, $8, $9 }' $file > C17.txt
awk '{if($5=="C18") print $3, $6, $7, $8, $9 }' $file > C18.txt
awk '{if($5=="C19") print $3, $6, $7, $8, $9 }' $file > C19.txt
awk '{if($5=="C20") print $3, $6, $7, $8, $9 }' $file > C20.txt
awk '{if($5=="C20") print $3, $6, $7, $8, $9 }' $file > C20.txt
awk '{if($5=="C21") print $3, $6, $7, $8, $9 }' $file > C21.txt
awk '{if($5=="C22") print $3, $6, $7, $8, $9 }' $file > C22.txt
awk '{if($5=="C23") print $3, $6, $7, $8, $9 }' $file > C23.txt
awk '{if($5=="C24") print $3, $6, $7, $8, $9 }' $file > C24.txt
awk '{if($5=="C25") print $3, $6, $7, $8, $9 }' $file > C25.txt
awk '{if($5=="C26") print $3, $6, $7, $8, $9 }' $file > C26.txt
awk '{if($5=="C27") print $3, $6, $7, $8, $9 }' $file > C27.txt
awk '{if($5=="C28") print $3, $6, $7, $8, $9 }' $file > C28.txt
awk '{if($5=="C29") print $3, $6, $7, $8, $9 }' $file > C29.txt
awk '{if($5=="C30") print $3, $6, $7, $8, $9 }' $file > C30.txt
awk '{if($5=="C31") print $3, $6, $7, $8, $9 }' $file > C31.txt
awk '{if($5=="C32") print $3, $6, $7, $8, $9 }' $file > C32.txt


gnuplot << COF

set terminal png
set output "${file}.BDS.dx.png"

set terminal png size 2100,1300
set xlabel "Cpoch (seconds)"
set ylabel "dx(m) "

set key top outside horizontal center

plot "C01.txt" using 1:2 with linespoints title "C01", \
     "C02.txt" using 1:2 with linespoints title "C02", \
     "C03.txt" using 1:2 with linespoints title "C03", \
     "C04.txt" using 1:2 with linespoints title "C04", \
     "C05.txt" using 1:2 with linespoints title "C05", \
     "C06.txt" using 1:2 with linespoints title "C06", \
     "C07.txt" using 1:2 with linespoints title "C07", \
     "C08.txt" using 1:2 with linespoints title "C08", \
     "C09.txt" using 1:2 with linespoints title "C09", \
     "C10.txt" using 1:2 with linespoints title "C10", \
     "C11.txt" using 1:2 with linespoints title "C11", \
     "C12.txt" using 1:2 with linespoints title "C12", \
     "C13.txt" using 1:2 with linespoints title "C13", \
     "C14.txt" using 1:2 with linespoints title "C14", \
     "C15.txt" using 1:2 with linespoints title "C15", \
     "C16.txt" using 1:2 with linespoints title "C16", \
     "C17.txt" using 1:2 with linespoints title "C17", \
     "C18.txt" using 1:2 with linespoints title "C18", \
     "C19.txt" using 1:2 with linespoints title "C19", \
     "C20.txt" using 1:2 with linespoints title "C20", \
     "C21.txt" using 1:2 with linespoints title "C21", \
     "C23.txt" using 1:2 with linespoints title "C23", \
     "C22.txt" using 1:2 with linespoints title "C22", \
     "C24.txt" using 1:2 with linespoints title "C24", \
     "C25.txt" using 1:2 with linespoints title "C25", \
     "C26.txt" using 1:2 with linespoints title "C26", \
     "C27.txt" using 1:2 with linespoints title "C27", \
     "C28.txt" using 1:2 with linespoints title "C28", \
     "C29.txt" using 1:2 with linespoints title "C29", \
     "C30.txt" using 1:2 with linespoints title "C30", \
     "C31.txt" using 1:2 with linespoints title "C31", \
     "C32.txt" using 1:2 with linespoints title "C32"

set terminal png
set output "${file}.BDS.dy.png"

set terminal png size 2100,1300
set xlabel "Cpoch (seconds)"
set ylabel "dy(m) "

set key top outside horizontal center

plot "C01.txt" using 1:3 with linespoints title "C01", \
     "C02.txt" using 1:3 with linespoints title "C02", \
     "C03.txt" using 1:3 with linespoints title "C03", \
     "C04.txt" using 1:3 with linespoints title "C04", \
     "C05.txt" using 1:3 with linespoints title "C05", \
     "C06.txt" using 1:3 with linespoints title "C06", \
     "C07.txt" using 1:3 with linespoints title "C07", \
     "C08.txt" using 1:3 with linespoints title "C08", \
     "C09.txt" using 1:3 with linespoints title "C09", \
     "C10.txt" using 1:3 with linespoints title "C10", \
     "C11.txt" using 1:3 with linespoints title "C11", \
     "C12.txt" using 1:3 with linespoints title "C12", \
     "C13.txt" using 1:3 with linespoints title "C13", \
     "C14.txt" using 1:3 with linespoints title "C14", \
     "C15.txt" using 1:3 with linespoints title "C15", \
     "C16.txt" using 1:3 with linespoints title "C16", \
     "C17.txt" using 1:3 with linespoints title "C17", \
     "C18.txt" using 1:3 with linespoints title "C18", \
     "C19.txt" using 1:3 with linespoints title "C19", \
     "C20.txt" using 1:3 with linespoints title "C20", \
     "C21.txt" using 1:3 with linespoints title "C21", \
     "C23.txt" using 1:3 with linespoints title "C23", \
     "C22.txt" using 1:3 with linespoints title "C22", \
     "C24.txt" using 1:3 with linespoints title "C24", \
     "C25.txt" using 1:3 with linespoints title "C25", \
     "C26.txt" using 1:3 with linespoints title "C26", \
     "C27.txt" using 1:3 with linespoints title "C27", \
     "C28.txt" using 1:3 with linespoints title "C28", \
     "C29.txt" using 1:3 with linespoints title "C29", \
     "C30.txt" using 1:3 with linespoints title "C30", \
     "C31.txt" using 1:3 with linespoints title "C31", \
     "C32.txt" using 1:3 with linespoints title "C32"

set terminal png
set output "${file}.BDS.dz.png"

set terminal png size 2100,1300
set xlabel "Cpoch (seconds)"
set ylabel "dz(m) "

set key top outside horizontal center

plot "C01.txt" using 1:4 with linespoints title "C01", \
     "C02.txt" using 1:4 with linespoints title "C02", \
     "C03.txt" using 1:4 with linespoints title "C03", \
     "C04.txt" using 1:4 with linespoints title "C04", \
     "C05.txt" using 1:4 with linespoints title "C05", \
     "C06.txt" using 1:4 with linespoints title "C06", \
     "C07.txt" using 1:4 with linespoints title "C07", \
     "C08.txt" using 1:4 with linespoints title "C08", \
     "C09.txt" using 1:4 with linespoints title "C09", \
     "C10.txt" using 1:4 with linespoints title "C10", \
     "C11.txt" using 1:4 with linespoints title "C11", \
     "C12.txt" using 1:4 with linespoints title "C12", \
     "C13.txt" using 1:4 with linespoints title "C13", \
     "C14.txt" using 1:4 with linespoints title "C14", \
     "C15.txt" using 1:4 with linespoints title "C15", \
     "C16.txt" using 1:4 with linespoints title "C16", \
     "C17.txt" using 1:4 with linespoints title "C17", \
     "C18.txt" using 1:4 with linespoints title "C18", \
     "C19.txt" using 1:4 with linespoints title "C19", \
     "C20.txt" using 1:4 with linespoints title "C20", \
     "C21.txt" using 1:4 with linespoints title "C21", \
     "C23.txt" using 1:4 with linespoints title "C23", \
     "C22.txt" using 1:4 with linespoints title "C22", \
     "C24.txt" using 1:4 with linespoints title "C24", \
     "C25.txt" using 1:4 with linespoints title "C25", \
     "C26.txt" using 1:4 with linespoints title "C26", \
     "C27.txt" using 1:4 with linespoints title "C27", \
     "C28.txt" using 1:4 with linespoints title "C28", \
     "C29.txt" using 1:4 with linespoints title "C29", \
     "C30.txt" using 1:4 with linespoints title "C30", \
     "C31.txt" using 1:4 with linespoints title "C31", \
     "C32.txt" using 1:4 with linespoints title "C32"

set terminal png
set output "${file}.BDS.dt.png"

set terminal png size 2100,1300
set xlabel "Epoch (seconds)"
set ylabel "dz(m) "

set key top outside horizontal center

plot "C01.txt" using 1:5 with linespoints title "C01", \
     "C02.txt" using 1:5 with linespoints title "C02", \
     "C03.txt" using 1:5 with linespoints title "C03", \
     "C04.txt" using 1:5 with linespoints title "C04", \
     "C05.txt" using 1:5 with linespoints title "C05", \
     "C06.txt" using 1:5 with linespoints title "C06", \
     "C07.txt" using 1:5 with linespoints title "C07", \
     "C08.txt" using 1:5 with linespoints title "C08", \
     "C09.txt" using 1:5 with linespoints title "C09", \
     "C10.txt" using 1:5 with linespoints title "C10", \
     "C11.txt" using 1:5 with linespoints title "C11", \
     "C12.txt" using 1:5 with linespoints title "C12", \
     "C13.txt" using 1:5 with linespoints title "C13", \
     "C14.txt" using 1:5 with linespoints title "C14", \
     "C15.txt" using 1:5 with linespoints title "C15", \
     "C16.txt" using 1:5 with linespoints title "C16", \
     "C17.txt" using 1:5 with linespoints title "C17", \
     "C18.txt" using 1:5 with linespoints title "C18", \
     "C19.txt" using 1:5 with linespoints title "C19", \
     "C20.txt" using 1:5 with linespoints title "C20", \
     "C21.txt" using 1:5 with linespoints title "C21", \
     "C23.txt" using 1:5 with linespoints title "C23", \
     "C22.txt" using 1:5 with linespoints title "C22", \
     "C24.txt" using 1:5 with linespoints title "C24", \
     "C25.txt" using 1:5 with linespoints title "C25", \
     "C26.txt" using 1:5 with linespoints title "C26", \
     "C27.txt" using 1:5 with linespoints title "C27", \
     "C28.txt" using 1:5 with linespoints title "C28", \
     "C29.txt" using 1:5 with linespoints title "C29", \
     "C30.txt" using 1:5 with linespoints title "C30", \
     "C31.txt" using 1:5 with linespoints title "C31", \
     "C32.txt" using 1:5 with linespoints title "C32"
COF

rm C[0-9][0-9].txt
