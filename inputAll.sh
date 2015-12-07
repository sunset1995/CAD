#!/bin/bash
printf "run case 1"
time ./nowVersion < iccad2015_input.case1 > output.dp.case1
printf "run case 2"
time ./nowVersion < iccad2015_input.case2 > output.dp.case2
printf "run case 3"
time ./nowVersion < iccad2015_input.case3 > output.dp.case3
printf "run case 4"
time ./nowVersion < iccad2015_input.case4 > output.dp.case4
printf "run case 5"
time ./nowVersion < iccad2015_input.case5 > output.dp.case5
