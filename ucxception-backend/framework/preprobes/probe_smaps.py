""""#!/bin/bash
now=$(date +%Y%m%d%H%M%S)
cat /proc/$1/smaps | awk -F ' ' -v date_time=$now -v mapped_file="" '{split($1,pos_hif,"-")} (index($1,"-") > 0){mapped_file=$6; printf("%s|%s_calc_size:%d\n",date_time,mapped_file,strtonum("0x"pos_hif[2])-strtonum("0x"pos_hif[1]))} (index($1,"-") <= 0) {print date_time"|"mapped_file"_"$0}'"""