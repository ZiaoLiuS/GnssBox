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

year=2022
doy=001

station="CUT"

# # download obs and nav data
# touch station.list
# echo "COCO" >> station.list
# get_obs_nav.sh -s station.list -b "2020 01 01" -e "2020 01 01" -o ./data/ -v 3 --nav_type Mixed
# # now, download sinex file from ftp
# get_eph.sh -a 'grg' -s "ign" -t "snx" -b "2020 01 01" -e "2020 01 01" -o ./data/

dir_obs=./data

base_file=${dir_obs}/CUT000AUS_R_20220010000_01D_30S_MO.rnx
rover_file=${dir_obs}/CUT200AUS_R_20220010000_01D_30S_MO.rnx
nav_file=${dir_obs}/${station}*${year}${doy}*MN.rnx
out_file=${dir_name}/${station}${year}${doy}.out

echo $base_file
echo $rover_file
echo $nav_file
echo $out_file

echo --roverObsFile $rover_file --baseObsFile $base_file --navFile $nav_file --outputFile $out_file

# compute spp solution
../../cmake-build-debug/apps/rtk --roverObsFile $rover_file --baseObsFile $base_file --navFile $nav_file --outputFile $out_file