#!/bin/bash

RED='\033[0;31m'
NC='\033[0m' # No Color
GREEN='\033[0;32m'
BLUE='\033[0;34m'

echo -e "${BLUE}Testing has started!${NC}"
echo
for file in *.in; do
    printf "%s\n" $file
    ../build/hwm < $file > tmp.txt
    echo "$(<tmp.txt)"
    printf "Correct answer: "
    filename=${file::-3}
    answer="answers/${filename}.ans"
    echo "$(<$answer)"
    diff --brief $answer tmp.txt >/dev/null
    comp_value=$?

    if [ $comp_value -eq 1 ]
    then
        echo -e "${GREEN}OK${NC}"
    else
        echo -e "${RED}FAIL${NC}"
    fi
        printf "\n" 
done

rm tmp.txt