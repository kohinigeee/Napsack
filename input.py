import csv
import random

MAX_D = 2000 
MIN_D = 1000 

fpath = "menu_hot_20170509"
# fpath = "menu_sai_20190630.csv"

outFname = "extend2_"+fpath+".dat"
with open(fpath+".csv", "r", encoding="ms932") as file:
    data = csv.DictReader(file, delimiter=",")

    items = []

    for i in data:
        id = (i["商品ID"])
        price = (i["価格"])
        cal = (i["カロリー"]) 
        d = str(random.randint(MIN_D, MAX_D))

        items.append([id, price, cal, d]) 

    outFile = open(outFname, "w", encoding="utf-8")
    for i in items:
        s = " ".join(i) 
        outFile.write(s)
        outFile.write("\n")
        
    print("Finished export")
