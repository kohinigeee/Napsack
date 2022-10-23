datafname=$1

sumps=(2500 25000 50000)

for i in ${sumps[@]};
do
    # echo $i
    ./main.sh $i $datafname
done