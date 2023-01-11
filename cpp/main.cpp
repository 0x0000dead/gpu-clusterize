#define PRINT_DEBUG 0

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <istream>
#include "KMedoids.h"
#include <cmath>
#include <string>
#include <chrono>

using namespace std;

void calculate_distance_matrix_cuda(double *input_ij_1d, double *output_ij_1d, int num_dimensions, int M);


int main() {
//    std::ifstream file("../../data/my_data_10.txt");
    std::ifstream file("../../data/ds1x4.txt");
    std::ifstream faiss_file("../../data/faiss_ds1x4.txt");
    std::string fname = "../../data/faiss_ds1x4.txt";
    if (file.is_open()) {
        int dim, m;
//        double UB = 30;
        double UB = 11934.8;
//        double UB = 24840.3;
//        double UB = 15863.8;
//        double UB = 100000;
//        int k = 100;
        int k = 64;
//        int k = 2;

        file >> m >> dim;
        std::vector<std::vector<double>> input_ij(m, std::vector<double>(m, 0));
        std::vector<std::vector<int>> faiss_ij(m, std::vector<int>(m, -1));

        double *input_ij_1d = new double[m*m];
        double *distance_matrix_1d = new double[m*m];
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < dim; j++) {
                file >> input_ij[i][j];
                input_ij_1d[i*dim + j] = input_ij[i][j];
            }
//            for (int j = 0; j < m; j++) {
//                faiss_file >> faiss_ij[i][j];
//            }
//            std::cout << i << " ";
        }
//        int* matrix = MATRIX::faster_read_matrix(fname, m, m);
        // matrix to faiss_ij
//        for (int i = 0; i < m; i++) {
//            for (int j = 0; j < m; j++) {
//                faiss_ij[i][j] = matrix[i*m + j];
//            }
//        }
//      delete[] matrix;
        auto finish = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Time to read file: " << elapsed.count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
        calculate_distance_matrix_cuda(input_ij_1d, distance_matrix_1d, dim, m);
        delete[] input_ij_1d;
        std::vector<std::vector<double>> d_ij(m, std::vector<double>(m, 0));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                if(i==j) {
                    d_ij[i][j] = INT_MAX;
                }
                else {
                    d_ij[i][j] = distance_matrix_1d[i * m + j];
                }
#if PRINT_DEBUG
                std::cout << distance_matrix_1d[i*m + j] << " ";
#endif
            }
#if PRINT_DEBUG
            std::cout << std::endl;
#endif
        }
//        vector<vector<double>> d_ij = MATRIX::read_distance_matrix(file_distance, m);
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "Time matrix calculation: " << elapsed.count() << " s\n";
//        55 2 146 108 129
//        98 114 80 64 28

// my
//        55 98 0 34 136
//        2 114 65 103 49


        KMedoids kmed(k, m, d_ij, UB,faiss_ij );
        start = std::chrono::high_resolution_clock::now();
        std::vector<double> res = kmed.solve(false);
        finish = std::chrono::high_resolution_clock::now();

        elapsed = finish - start;
        std::cout << "LB: " << res[0] << std::endl;
        std::cout << "UB: " << res[1] << std::endl;
        std::cout << "Time: " << elapsed.count() << " s\n";
//        std::cout << "Time: " << duration_cast<std::chrono::microseconds>(finish - start).count() << " s\n";
        }

        return 0;
}

