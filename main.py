import numpy as np
from sklearn.metrics.pairwise import pairwise_distances
import numba
from numba import jit
import time


class KMedoids:
    def __init__(self, k, m, d_ij, UB):
        self.d_ij = d_ij
        self.m = m
        self.k = k
        self.UB = UB

        self.ro_sorted = None
        self.indexes_map = None

        self.lambda_v = -np.ones(m)

        self.LB = -np.inf
        self.gamma_0, self.b_max, self.s, self.beta = 0, 0, 0, 0

        self.x_ij = np.zeros((m, m))
        self.nu_ij = np.zeros((m, m))
        self.gamma = 1.05
        self.g = np.zeros(m)
        self.ro = -self.lambda_v
        self.lagrangian = 0
        self.j_glob = 0

        self.l_h = np.argsort(d_ij, axis=0)
        self.d_ij_sorted = np.sort(d_ij, axis=0)

        self.y_i = np.zeros(m)

    def compute_cost_dual_classic(self):
        for i in range(self.m):
            total_sum = 0
            for j in range(self.m):
                if i != j:
                    print(self.d_ij[i][j])
                    total_sum += min(0, self.d_ij[i][j] - self.lambda_v[j])
            self.ro[i] = total_sum - self.lambda_v[i]  # TODO here?
        self.indexes_map = np.argsort(self.ro)
        self.ro_sorted = np.sort(self.ro)
        self.lagrangian = np.sum(self.ro_sorted[:self.k]) + np.sum(self.lambda_v)

    def compute_cost_dual_subcolumn(self):
        is_break = False
        while True:
            self.lagrangian = self.lagrangian + self.lambda_v[self.j_glob]  # 2 step
            is_two = False
            h = 0
            while True:
                if (self.d_ij[self.l_h[h][self.j_glob]][self.j_glob] - self.lambda_v[self.j_glob]) >= 10 ** -6:  # 3 step
                    if self.j_glob < self.m - 1:  # 6 step
                        is_two = True
                        self.j_glob += 1
                        break
                    else:
                        is_break = True
                        break
                else:
                    self.ro[self.l_h[h][self.j_glob]] = self.ro[self.l_h[h][self.j_glob]] + \
                                                        self.d_ij[self.l_h[h][self.j_glob]][self.j_glob] - \
                                                        self.lambda_v[self.j_glob]
                    if h < self.m - 1:  # 5 step
                        h += 1
                    elif self.j_glob < self.m - 1:  # 6 step
                        is_two = True
                        self.j_glob += 1
                        break
                    else:
                        is_break = True
                        break

            if is_two:
                continue
            elif is_break:
                break
        # 7 step
        self.ro_sorted = np.sort(self.ro)
        self.indexes_map = np.argsort(self.ro)
        self.lagrangian = self.lagrangian + np.sum(self.ro_sorted[:k])

    def compute_y_and_g(self):
        self.y_i = np.zeros(m)
        for i in range(k):
            self.y_i[self.indexes_map[i]] = 1

        for i in range(m):
            for j in range(m):
                self.nu_ij[i][j] = d_ij[i][j] - self.lambda_v[j]
                # if i != j:
                if self.y_i[i] == 1 and (d_ij[i][j] - self.lambda_v[j]) < 0:
                    self.x_ij[i][j] = 1
                else:
                    self.x_ij[i][j] = 0

        for j in range(m):
            total_sum = 0
            for i in range(m):
                total_sum += self.x_ij[i][j] - self.y_i[j]
            self.g[j] = 1 - total_sum

    def solve(self, is_classic=True):
        a = 0
        while True:
            #print("Lambda: ", self.lambda_v)
            if is_classic:
                self.compute_cost_dual_classic()
            else:
                self.compute_cost_dual_subcolumn()
            print(self.lagrangian, self.ro)
            a += 1
            if a == 2: break
            if self.lagrangian > self.LB:
                self.LB = self.lagrangian
                self.beta = 0
            if self.LB / self.UB >= 1 - 10 ** (-5):
                break

            self.compute_y_and_g()

            if np.linalg.norm(self.g) ** 2 <= 10 ** (-5):
                break

            # 8 step
            if self.beta > self.b_max:
                self.gamma = self.gamma / 1.01
                self.beta = 0
            else:
                self.beta = self.beta + 1

            # 9 step
            if self.gamma < 10 ** (-3):
                break

            alpha = self.gamma * (1.05 * self.UB - self.lagrangian) / (np.linalg.norm(self.g) ** 2)

            self.lambda_v = self.lambda_v + alpha * self.g
            self.s = self.s + 1

            #print(self.lambda_v)
        return self.lambda_v, self.LB, self.UB


# @jit(nopython=True)
def calculate_matrix(size, all_points, is_sklearn=False):
    # if is_sklearn:
    # distance_matrix = pairwise_distances(all_points, metric='euclidean')
    # else:
    distance_matrix = np.zeros((size, size))
    for i in range(size):
        distance_matrix[i, :] = np.sqrt(np.sum((all_points[i] - all_points) ** 2, axis=1))

    return distance_matrix


def read_data(path):
    all_points = []
    with open(path, 'r') as f:
        numbers = f.readline().split()
        numbers = [int(x) for x in numbers]
        print(numbers)
        for line in f:
            all_points.append([float(x) for x in line.split()])

        start = time.time()
        distance_matrix = calculate_matrix(numbers[0], np.array(all_points), is_sklearn=False)
        end = time.time()
        print("Time for calculating distance matrix: ", end - start)
        # print(distance_matrix)

        # save distance matrix to file
        # np.savetxt('distance_matrix.txt', distance_matrix, fmt='%f')
    return distance_matrix, numbers[0]


if __name__ == '__main__':
    # if i closest medoid to j
    # yi = 1 if i is medoid

    # create ro size of 5

    # sort ro
    # map indexes before sorting to indexes after sorting
    # d_ij, m = read_data("./data/ds1x0.txt")
    d_ij, m = read_data("./data/my_example.txt")
    k = 2
    UB = 100
    #print(d_ij)
    kmed = KMedoids(k, m, d_ij, UB)
    kmed1 = KMedoids(k, m, d_ij, UB)

    print("Is_classic")
    lambda_s, LB, UB = kmed.solve(is_classic=True)
    print("is sub column")
    lambda_s, LB, UB = kmed1.solve(is_classic=False)
    #print(lambda_s, LB, UB)
