import itertools
import math


def distance(point1, point2):
    return math.sqrt((point1[0] - point2[0]) ** 2 + (point1[1] - point2[1]) ** 2)


def brute_force_k_medoids(data, k):
    medoids = []

    for combination in itertools.combinations(data, k):
        total_distance = 0
        for datapoint in data:
            closest_medoid = min(combination, key=lambda x: distance(datapoint, x))
            total_distance += distance(datapoint, closest_medoid)

        if not medoids or total_distance < medoids[1]:
            medoids = [combination, total_distance]

    return medoids[0], medoids[1]


def main():
    data = [(3,5), (5,5), (0,1), (1,8), (5,6), (2,4), (3,15), (3,16), (3,17), (2,15)]
    # 2, ((3, 5), (3, 16)) 17.67004637770997
    # 3, ((3, 5), (0, 1), (3, 16)) 12.670046377709971
    # 4, ((3, 5), (0, 1), (1, 8), (3, 16)) 9.06449510224598
    # 5, ((3, 5), (5, 5), (0, 1), (1, 8), (3, 16)) 5.82842712474619
    #data = [(3, 5), (0, 1), (1, 8), (5, 6), (2, 4)]

    k = 5

    medoids, dist = brute_force_k_medoids(data, k)

    print(medoids, dist)


if __name__ == '__main__':
    main()
