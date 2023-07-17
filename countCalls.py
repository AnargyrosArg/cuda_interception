import sys

file = open(sys.argv[1])


counters = dict()


for line in file.readlines():
    if line in counters.keys():
        counters[line] = counters[line]+1 
    else:
        counters[line]=1


print("{" + "\n".join("{!r}: {!r},".format(k, v) for k, v in counters.items()) + "}")
