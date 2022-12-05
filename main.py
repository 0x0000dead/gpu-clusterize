import numpy as np


def ro_function(lambda_s):
    return 0


def L_function(lambda_s):
    return 0


def compute_y(lambda_s):
    return 0


def compute_g(lambda_s):
    return 0


def Z_function(lambda_s):
    return 0


def subgradient_algorithm_maximization_lagrangian_dual_function(k, m, d_ij, UB):
    # 1: Initialization: set UB and LB to -infinity, gamma_0, b_max, lambda_j_0, s = 0 and betta = 0
    # i - data item
    # i = 3
    # j = 3
    # k = 2
    # m = 5

    lambda_v = np.zeros(m)
    # lambda_v = np.array([1, 2, 3, 4, 5])

    # d_ij = np.array([
    #     [0, 1, 2, 4, 5],
    #     [1, 0, 5, 6, 7],
    #     [2, 5, 0, 8, 9],
    #     [4, 6, 8, 0, 10],
    #     [5, 7, 9, 10, 0]
    # ])

    LB =  -np.inf
    gamma_0, b_max, lambda_s, s, beta = 0, 0, 0, 0, 0
    # 2: Compute reduced costs ro(lambda_s) and Lagrangian dual function value L(lambda_s)
    s = 0
    ro = np.zeros(m)
    y_i = np.zeros(m)
    x_ij = np.zeros((m, m))
    nu_ij = np.zeros((m, m))
    gamma = 1.05
    # UB = 1000
    g = np.zeros(m)
    while True:
        for i in range(m):
            total_sum = 0
            for j in range(m):
                if i != j:
                    total_sum += min(0, d_ij[i][j] - lambda_v[j]) - lambda_v[i]
            ro[i] = total_sum
        indexes_map = np.argsort(ro)
        ro_sorted = np.sort(ro)
        print("lagr sorted")
        lagrangian = np.sum(ro_sorted[:k]) + np.sum(lambda_v)

        if lagrangian > LB:
            LB = lagrangian
            beta = 0
        if LB / UB >= 1 - 10 ** (-5):
            break

        for i in range(k):
            y_i[indexes_map[i]] = 1

        for i in range(m):
            for j in range(m):
                nu_ij[i][j] = d_ij[i][j] - lambda_v[j]
                if i != j:
                    if y_i[i] == 1 and d_ij[i][j] - lambda_v[j] < 0:
                        x_ij[i][j] = 1

        for j in range(m):
            total_sum = 0
            for i in range(m):
                total_sum += x_ij[i][j] - y_i[j]
            g[j] = 1 - total_sum



        if np.linalg.norm(g) ** 2 <= 10 ** (-5):
            break

        # 8 step
        if beta > b_max:
            gamma = gamma / 1.01
            beta = 0
        else:
            beta = beta + 1

        # 9 step
        if gamma < 10 ** (-3):
            break

        alpha = gamma * (1.05 * UB - lagrangian) / (np.linalg.norm(g) ** 2)

        lambda_v = lambda_v + alpha * g
        s = s + 1
        print(s)
        print(y_i)
        print(lambda_v)
        print(lagrangian)
        print(x_ij)
    return lambda_s, LB, UB

def read_data(path):
    all_points = []
    with open(path, 'r') as f:
        # read first string numbers
        numbers = f.readline().split()
        # as int
        numbers = [int(x) for x in numbers]
        print(numbers)
        for line in f:
            all_points.append([float(x) for x in line.split()])
        print((all_points[0]))
        # calculate distance matrix
        distance_matrix = np.zeros((numbers[0], numbers[0]))
        # distance_matrix = distance_matrix(all_points, all_points)
        from scipy.spatial.distance import pdist
        all_points = np.array(all_points)
        for i in range(numbers[0]):
            distance_matrix[i, :] = np.sqrt(np.sum((all_points[i] - all_points) ** 2, axis=1))
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
    d_ij, m = read_data("./data/ds1x4.txt")
    k = 100
    lambda_s, LB, UB = subgradient_algorithm_maximization_lagrangian_dual_function(m=m, d_ij=d_ij, k=k,UB=11934.8)
    print(lambda_s, LB, UB)
