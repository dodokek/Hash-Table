import numpy as np
import math
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt

f = open("../csv_file.csv")

names = ["Length Hash", "Hashsum", "FirstAscii", "Ror Hash", "Rol Hash", "Murmur hash"]

for i in range(6):

    index_1 = f.readline().split(" ")
    index_2 = []
    for elem in index_1:
        try:
            elem = int(elem)
            index_2.append(elem)

        except:
            continue
    

    colls_1 = f.readline().split(" ")
    colls_2 = []
    for elem in colls_1:
        try:
            elem = int(elem)
            colls_2.append(elem)

        except:
            continue

    # Man, i love python for its inreadable scripts like this one
    index      = np.array (index_2)
    collisions = np.array (colls_2)

    plt.bar(index, collisions, align ='center', alpha=0.5)
    # plt.title("Graph ", i)

    plt.savefig(names[i] + ".png")
    plt.clf()

    f.readline()

# Putting info and drawing graph