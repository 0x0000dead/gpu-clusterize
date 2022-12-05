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