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
    data = [(3, 5), (0, 1), (1, 8), (5, 6), (2, 4)]
    k = 3

    medoids, dist = brute_force_k_medoids(data, k)

    print(medoids, dist)


if __name__ == '__main__':
    main()
