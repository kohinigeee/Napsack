datafname=$1

sumps=(2500 25000 50000 250000)
# sumps=(1000000)

for i in ${sumps[@]};
do
    # echo $i
    ./main.sh $i $datafname
done