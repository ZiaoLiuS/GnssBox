#!/bin/bash
#
#  使用方法：
#####################################################################

user_name=`echo $USER | sed 's/ /_/g'`
dir_name=./results_${user_name}
if [ ! -d $dir_name ]
then
    mkdir $dir_name
fi

year=2021
doy=001

station="ABMF"

# # download obs and nav data
# touch station.list
# echo "COCO" >> station.list
# get_obs_nav.sh -s station.list -b "2020 01 01" -e "2020 01 01" -o ./data/ -v 3 --nav_type Mixed
# # now, download sinex file from ftp
# get_eph.sh -a 'grg' -s "ign" -t "snx" -b "2020 01 01" -e "2020 01 01" -o ./data/

dir_obs=./data

obs_file=${dir_obs}/${station}*${year}${doy}*MO.rnx
nav_file=${dir_obs}/${station}*${year}${doy}*MN.rnx
out_file=${dir_name}/${station}${year}${doy}.out

echo $obs_file
echo $nav_file
echo $out_file

# compute spp solution
../../cmake-build-debug/apps/spp --obsFile $obs_file --navFile $nav_file --outputFile $out_file

station=$( echo $station | tr '[A-Z]' '[a-z]' )
refXYZ=`awk -v var=$station '{if($1==var) print $5, $6, $7}' ${dir_obs}/GRG*${year}001*.xyz`

diff_file=${out_file}.diff

# compute the diffrence between spp solution and the reference xyz
../../cmake-build-debug/apps/xyz_diff --xyzFile $out_file.filter.spp.out --xCol 8 --yCol 9 --zCol 10 --refXYZ "$refXYZ" --outputFile $diff_file

# plot scatter picture
gnuplot << EOF
set terminal png
set output "spp_test.png"

set xlabel "second"
set ylabel "error"

set yrange [-15:15]

file="${diff_file}"

plot file using 3:4 title "dx", \
     file using 3:5 title "dy", \
     file using 3:6 title "dz"
EOF


gnuplot << EOF
set terminal png
set output "spp_test2.png"

set xlabel "dx"
set ylabel "dy"

file="${diff_file}"

plot file using 4:5 title "dx-dy"   

EOF

