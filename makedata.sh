infpath=$1
scale=$2

v=(${scale//./ })
v2=""
for s in "${v[@]}"; do v2="$v2$s"; done

mins=(-1 10 100 1000)
maxs=(-1 100 1000 10000)

for i in {1..3}
do
    outFname="${infpath}_p${v2}_${i}.dat"
    echo "python input.py $outFname ${mins[$i]} ${maxs[$i]} $scale $infpath"
    python "input.py" $outFname ${mins[$i]} ${maxs[$i]} $scale $infpath
    echo
done