import csv
import random
import sys

MAX_D = 100
MIN_D = 10

args = sys.argv
scale = 0.01 

if ( len(args) <= 1 ):
    print("Exe with output file name on first command")
    exit()
if ( len(args) >= 3 ):
    MIN_D = int(args[2])
if ( len(args) >= 4 ):
    MAX_D = int(args[3])
if ( len(args) >= 5 ):
    scale = float(args[4])
if ( len(args) >= 6 ):
    fpath = args[5]
    
outFname = args[1]

with open(fpath+".csv", "r", encoding="ms932") as file:
    data = csv.DictReader(file, delimiter=",")

    items = []

    for i in data:
        id = (i["商品ID"])
        price = (i["価格"])
        cal = (i["カロリー"]) 
        d = str(random.randint(MIN_D, MAX_D))

        price = int(i["価格"])*scale
        price = str(int(price))
        
        items.append([id, price, cal, d]) 

    outFile = open(outFname, mode="w", encoding="utf-8")
    for i in items:
        s = " ".join(i) 
        outFile.write(s)
        outFile.write("\n")

    print(f"Imporeted [{fpath}]")
    print(f"Exported to [{outFname}]\n")
    print(outFname)
    print(f"price scale:{scale}  range:{MIN_D} ~ {MAX_D}")
