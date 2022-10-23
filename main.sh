#!/bin/sh

sump=$1
fname=$2

proname1="Napseg"
proname2="NapSimple"

g++ "$proname1.cpp" -o "$proname1.exe"
g++ "$proname2.cpp" -o "$proname2.exe"

./"$proname2.exe" $sump $fname
echo
./"$proname1.exe" $sump $fname
echo