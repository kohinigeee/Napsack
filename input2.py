import csv
import random
import sys

args = sys.argv

SUMP=int(args[1])

MAX_P=80
MIN_P=5

MAX_C=100
MIN_C=20

N = 70

outFname="self"

if ( len(args) >= 3 ):
    outFname=args[2]
    
outFname=f"{outFname}_{SUMP}.dat"
items=[]


for i in range(1, N+1):
    id=i
    price=random.randint(MIN_P, MAX_P)
    cal=random.randint(MIN_C, MAX_C)
    stock=int(SUMP/price)
    maxd=int(stock*0.9)
    mind=int(stock*0.6)
    d=random.randint(mind,maxd)

    items.append([str(id), str(price), str(cal), str(d)])

outFile = open(outFname, mode="w", encoding="utf-8")

for i in items:
    s = " ".join(i) 
    outFile.write(s)
    outFile.write("\n")

print(f"Exported to [{outFname}]\n")
print(f"price range= {MIN_P}~{MAX_P}")
print(f"calory range= {MIN_C}~{MAX_C}")
