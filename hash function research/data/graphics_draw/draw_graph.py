import numpy as np
import math
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt

f = open("../csv_file.csv")

names = ["Length Hash", "Hashsum", "FirstAscii", "Ror Hash", "Rol Hash", "Murmur hash"]
clrs = ['black', 'red', 'brown', 'blue', 'cyan', 'green']


for i in range(0,1):
    ax = plt.gca()
    # ax.set_ylim([0, 150])


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
    index      = ["Length Hash","FirstAscii", "Hashsum", "Ror Hash", "Rol Hash", "Murmur hash"]
    collisions = np.array ([589413, 8954, 28424, 189, 178, 93, 15])
    kek = np.array([1, 2, 3, 4, 5, 6])

    plt.bar(kek, collisions, align ='center')
    plt.xticks(kek, index)
    
    plt.xlabel("Index")
    plt.ylabel("Collisions")
    f.readline()

    plt.savefig("bebra.png")
    # plt.clf()


# Putting info and drawing graph