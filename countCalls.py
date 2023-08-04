import sys
import matplotlib.pyplot as plt
from matplotlib import ticker
file = open(sys.argv[1])


counters = dict()


for line in file.readlines():
    if not (line.startswith("cu") and line.endswith("()\n")):
        continue
    line = line.rstrip("\n")
    if line in counters.keys():
        counters[line] = counters[line]+1 
    else:
        counters[line]=1


print("{" + "\n".join("{!r}: {!r},".format(k, v) for k, v in counters.items()) + "}")

counters = sorted(counters.items(), key=lambda x:x[1])
counters = dict(counters)
width = 0.8
plt.figure(figsize=(12,4))
plt.bar(counters.keys(), counters.values(),width)
plt.yscale("log")
plt.xticks(rotation=90)
plt.yticks(rotation=90)
plt.show()
plt.savefig('histogram.png',bbox_inches="tight")
