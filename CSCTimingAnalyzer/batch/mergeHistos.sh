#!/bin/bash

INPUT_DIR=$1
OUTPUT_FILE=$2

input_files=(`ls -1 ${INPUT_DIR}/histos*.root`)

hadd -f ${OUTPUT_FILE} ${input_files[*]}
