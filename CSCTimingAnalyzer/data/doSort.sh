# Usage: . doSort.sh timing_corrections_by_chamber.dat

INFILE=$1
sort -n ${INFILE} | cut -f2- -d' ' > ${INFILE%.*}_sorted.dat
rename _unsorted_sorted "" ${INFILE%.*}_sorted.dat

unset INFILE
