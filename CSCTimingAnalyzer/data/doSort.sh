INFILE=$1
sort -n ${INFILE} | cut -f2- -d' ' > tmp.txt ; mv tmp.txt ${INFILE}
