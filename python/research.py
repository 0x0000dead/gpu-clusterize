# import kmedoids, numpy, time
# from sklearn.datasets import fetch_openml
# from sklearn.metrics.pairwise import euclidean_distances
# X, _ = fetch_openml('mnist_784', version=1, return_X_y=True, as_frame=False)
# X = X[:10000]
# diss = euclidean_distances(X)
# start = time.time()
# fp = kmedoids.fasterpam(diss, 100)
# print("FasterPAM took: %.2f ms" % ((time.time() - start)*1000))
# print("Loss with FasterPAM:", fp.loss)
# start = time.time()
# pam = kmedoids.pam(diss, 100)
# print("PAM took: %.2f ms" % ((time.time() - start)*1000))
# print("Loss with PAM:", pam.loss)
from sklearn.metrics.pairwise import pairwise_distances
import numpy as np
import random


def kMedoids(D, k, tmax=10000):
    # determine dimensions of distance matrix D
    m, n = D.shape

    if k > n:
        raise Exception('too many medoids')

    # find a set of valid initial cluster medoid indices since we
    # can't seed different clusters with two points at the same location
    valid_medoid_inds = set(range(n))
    invalid_medoid_inds = set([])
    rs, cs = np.where(D == 0)
    # the rows, cols must be shuffled because we will keep the first duplicate below
    index_shuf = list(range(len(rs)))
    np.random.shuffle(index_shuf)
    rs = rs[index_shuf]
    cs = cs[index_shuf]
    for r, c in zip(rs, cs):
        # if there are two points with a distance of 0...
        # keep the first one for cluster init
        if r < c and r not in invalid_medoid_inds:
            invalid_medoid_inds.add(c)
    valid_medoid_inds = list(valid_medoid_inds - invalid_medoid_inds)

    if k > len(valid_medoid_inds):
        raise Exception('too many medoids (after removing {} duplicate points)'.format(
            len(invalid_medoid_inds)))

    # randomly initialize an array of k medoid indices
    M = np.array(valid_medoid_inds)
    np.random.shuffle(M)
    M = np.sort(M[:k])

    # create a copy of the array of medoid indices
    Mnew = np.copy(M)

    # initialize a dictionary to represent clusters
    C = {}
    for t in range(tmax):
        # determine clusters, i. e. arrays of data indices
        J = np.argmin(D[:, M], axis=1)  # determine to which medoids belongs to
        P = np.min(D[:, M], axis=1)  # smallest distance through M medoids
        for kappa in range(k):
            C[kappa] = np.where(J == kappa)[0]
        # update cluster medoids
        for kappa in range(k):
            J = np.mean(D[np.ix_(C[kappa], C[kappa])], axis=1)
            j = np.argmin(J)
            Mnew[kappa] = C[kappa][j]
        np.sort(Mnew)
        # check for convergence
        if np.array_equal(M, Mnew):
            break
        M = np.copy(Mnew)
    else:
        # final update of cluster memberships
        J = np.argmin(D[:, M], axis=1)
        for kappa in range(k):
            C[kappa] = np.where(J == kappa)[0]

    # return results
    return M, C


# 3 points in dataset

data = np.array([[3, 5], [0, 1], [1, 8], [5, 6], [2, 4]])

# distance matrix
D = pairwise_distances(data, metric='euclidean')
# split into 2 clusters
M, C = kMedoids(D, 2)

print('medoids:')
for point_idx in M:
    print(data[point_idx])

print('')
print('clustering result:')
for label in C:
    for point_idx in C[label]:
        print('label {0}:???{1}'.format(label, data[point_idx]))
