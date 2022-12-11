#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <istream>
#include "KMedoids.h"

using namespace std;

std::vector<std::vector<double>> calculate_distance_matrix(std::vector<std::vector<double>> &all_points) {
    int size = all_points.size();

    std::vector<std::vector<double>> distance_matrix(size, std::vector<double>(size));

    for (int i = 0; i < size; ++i) {
        for (int j = i+1; j < size; ++j) {
            double sum = 0;
            for (int k = 0; k < all_points[i].size(); ++k) {
                sum += (all_points[i][k] - all_points[j][k]) * (all_points[i][k] - all_points[j][k]);
            }
            distance_matrix[i][j] = sqrt(sum);
            distance_matrix[j][i] = sqrt(sum);
        }
        distance_matrix[i][i] = INT_MAX;
    }
    return distance_matrix;
}
void print_matrix(std::vector<std::vector<double>> &matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
// measure time
#include <chrono>
int main() {
    std::ifstream file("../../data/my_data_10.txt");

    if (file.is_open()) {
        int dim, m;
        double UB = 30;
        int k = 2;

        file >> m >> dim;
        std::vector<std::vector<double>> input_ij(m, std::vector<double>(m, 0));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < dim; j++) {
                file >> input_ij[i][j];
            }
        }
        vector<vector<double>> d_ij = calculate_distance_matrix(input_ij);
        print_matrix(d_ij);
        KMedoids kmed(k, m, d_ij, UB);
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<double> res = kmed.solve(false);
        auto finish = std::chrono::high_resolution_clock::now();


        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "LB: " << res[0] << std::endl;
        std::cout << "UB: " << res[1] << std::endl;
        std::cout << "Time: " << elapsed.count() << " s\n";
//        std::cout << "Time: " << duration_cast<std::chrono::microseconds>(finish - start).count() << " s\n";
        }

        return 0;
}

