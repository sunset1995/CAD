#!/bin/bash
printf "\ncompile now...\n"
time ./compileAll.sh

printf "\nrun...\n"
time ./inputAll.sh
