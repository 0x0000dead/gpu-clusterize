#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <istream>
#include "KMedoids.h"
#include <immintrin.h>
//#include "dist.cu"

using namespace std;
void calculate_distance_matrix_cuda(double *input_ij_1d, double *output_ij_1d, int num_dimensions);

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

#include <immintrin.h>
#include <cstdint>
#include <cmath>
#include <vector>

void print_matrix(std::vector<std::vector<double>> &matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
#include <string>
#include <strstream>
std::vector<std::vector<double>> read_distance_matrix(std::ifstream& rfile, int m ) {
    std::vector<std::vector<double>> distance_matrix(m, std::vector<double>(m, 0));
    if (rfile.is_open()) {
        std::string line;
        int i = 0;
        while (std::getline(rfile, line)) {
            std::strstream ss;
            ss << line;
            for (int j = 0; j < m; ++j) {
                ss >> distance_matrix[i][j];
            }
            ++i;
            std::cout << i << std::endl;
        }

    }
        rfile.close();
    return distance_matrix;
}

#include <chrono>
int main() {
    std::ifstream file("../../data/my_data_10.txt");
//    std::ifstream file("../../data/ds1x4.txt");
    std::ifstream file_distance("../../distance_matrix.txt");

    if (file.is_open()) {
        int dim, m;
//        double UB = 30;
        double UB = 11934.8;
//        double UB = 100000;
        int k = 64;
//        int k = 5;

        file >> m >> dim;
        std::vector<std::vector<double>> input_ij(m, std::vector<double>(m, 0));
        // 1d array of doulbe
        double *input_ij_1d = new double[m*m];
        double *distance_matrix_1d = new double[m*m];
        // double array
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < dim; j++) {
                file >> input_ij[i][j];
                input_ij_1d[i*dim + j] = input_ij[i][j];
                std::cout << input_ij_1d[i*dim + j] << " ";
            }
            std::cout << std::endl;
        }
        auto start = std::chrono::high_resolution_clock::now();
        calculate_distance_matrix_cuda(input_ij_1d, distance_matrix_1d, dim);
        vector<vector<double>> d_ij;
        for (int i = 0; i < m; i++) {
            vector<double> row;
            for (int j = 0; j < m; j++) {
                row.push_back(distance_matrix_1d[i*m + j]);
                std::cout << distance_matrix_1d[i*m + j] << " ";
            }
            std::cout << std::endl;
            d_ij.push_back(row);
        }
//        vector<vector<double>> d_ij = read_distance_matrix(file_distance, m);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Time matrix calculation: " << elapsed.count() << " s\n";

//        print_matrix(d_ij);
        KMedoids kmed(k, m, d_ij, UB);
         start = std::chrono::high_resolution_clock::now();
        std::vector<double> res = kmed.solve(true);
         finish = std::chrono::high_resolution_clock::now();


        elapsed = finish - start;
        std::cout << "LB: " << res[0] << std::endl;
        std::cout << "UB: " << res[1] << std::endl;
        std::cout << "Time: " << elapsed.count() << " s\n";
//        std::cout << "Time: " << duration_cast<std::chrono::microseconds>(finish - start).count() << " s\n";
        }

        return 0;
}

