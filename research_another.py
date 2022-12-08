from sklearn_extra.cluster import KMedoids
import numpy as np


X = np.array([[3,5],[0,1],[1,8],[5,6],[2,4]])

kmedoids = KMedoids(n_clusters=2, method='pam', max_iter=1000000, random_state=2).fit(X)

print(kmedoids.labels_)
print(kmedoids.cluster_centers_)
print(kmedoids.inertia_)
print(kmedoids.n_iter_)
#kmedoids.predict([[0,0], [4,4]])
#kmedoids.cluster_centers_
#kmedoids.inertia_