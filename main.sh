#!/bin/sh

sump=$1
fname=$2

# pronames=("NapSimple" "Napseg2" "Napseg3")
# pronames=("Napseg2" "Napseg3")
pronames=("Napseg3")

for name in ${pronames[@]};
do
    g++ "$name.cpp" -o "$name.exe"
    ./"$name.exe" $sump $fname
    echo
done