from sklearn_extra.cluster import KMedoids
import numpy as np


def read_data(path):
    all_points = []
    with open(path, 'r') as f:
        numbers = f.readline().split()
        numbers = [int(x) for x in numbers]
        # print(numbers)
        for line in f:
            all_points.append([float(x) for x in line.split()])

    return all_points


data = read_data("../data/ds1x4.txt")

# X = np.array([[3, 5], [0, 1], [1, 8], [5, 6], [2, 4]])
# data = [(3, 5), (5, 5), (0, 1), (1, 8), (5, 6), (2, 4), (3, 15), (3, 16), (3, 17), (2, 15)]
X = np.array(data)
print("Started")
import time
print(time.time())
start = time.time()
kmedoids = KMedoids(n_clusters=64, method='alternate', max_iter=1000000, random_state=2,).fit(X)
print(time.time())
end = time.time()
print(end - start)
print("Ended")
# print(kmedoids.labels_)
# print(kmedoids.cluster_centers_)
print(kmedoids.inertia_)
# print(kmedoids.n_iter_)
# kmedoids.predict([[0,0], [4,4]])
# kmedoids.cluster_centers_
# kmedoids.inertia_
