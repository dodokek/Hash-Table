import numpy as np
import math
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt

f = open("../csv_file.csv")

names = ["Length Hash", "Hashsum", "FirstAscii", "Ror Hash", "Rol Hash", "Murmur hash"]
clrs = ['black', 'red', 'brown', 'blue', 'cyan', 'green']


for i in range(0,6):
    ax = plt.gca()
    # ax.set_ylim([0, 150])

    if names[i] == "FirstAscii":
        ax.set_xlim([0,200])
    if names[i] == "Length Hash":
        ax.set_xlim([0,100])


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

    # if names[i] == "Length Hash" or names[i] == "FirstAscii":
    #     f.readline()
    #     continue

    # Man, i love python for its inreadable scripts like this one
    
    # print (sum(colls_2))
    index      = np.array (index_2)
    collisions = np.array (colls_2)
    print (names[i], np.var(collisions));
    plt.bar(index, collisions, align ='center', color = clrs[i])
    plt.xlabel("Index")
    plt.ylabel("Collisions")
    f.readline()

    plt.savefig(names[i] + ".png")
    plt.clf()


# Putting info and drawing graph