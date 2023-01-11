#include "KMedoids.h"
#include "utils/constants.h"
#include <algorithm>
#include <cmath>
# include <vector>
#include <numeric>
#include <chrono>
#include <iostream>
#include <ostream>
#include <fstream>

KMedoids::KMedoids(int k, int m, std::vector<std::vector<double>>& d_ij, double UB, std::vector<std::vector<int>>& faiss) : UB(UB) {
    // reserve
    data = std::make_unique<VectorData>();
    data->k = k;
    data->m = m;
    data->g.reserve(m);
    data->prev_g.reserve(m);
    data->y_i.reserve(m);
    data->indexes_map.reserve(m);
    data->lambda_v.reserve(m);
//    data.prev_g = std::vector<double>(m, 0);
//    data.y_i = std::vector<int>(m, 0);
//    data.indexes_map = std::vector<int>(m, 0);
//    data.lambda_v = std::vector<double>(m, 0);

    data->l_h = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));
    data->d_ij = std::vector<std::vector<double>>(m, std::vector<double>(m, 0));
    data->x_ij = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));
    data->d_ij = d_ij;

    LB = INT_MIN;
    s = 0;
    gamma = CONSTANTS::GAMMA_INITIAL;

    if(faiss[0][0] != -1) {
        data->l_h = faiss;
    }
    else {
        for (int i = 0; i < m; i++) {
            std::vector<int> indexes(m);
            std::iota(indexes.begin(), indexes.end(), 0);
            std::sort(indexes.begin(), indexes.end(), [i, &d_ij](int a, int b) { return d_ij[a][i] < d_ij[b][i]; });
            for (int j = 0; j < m; j++) {
                data->l_h[j][i] = indexes[j];
            }
        }
    }

    for (int i = 0; i < m; i++) {
        data->lambda_v[i] = d_ij[i][data->l_h[0][i]];
    }
    cost_calculator = std::make_unique<CostCalculator>(data->m, data->lambda_v);
    subgradient_calculator = std::make_unique<SubgradientCalculator>(data->m);
    lambda_calculator = std::make_unique<LambdaCalculator>(data->m, false);

     std::cout << "l_h" << std::endl;
     for (int i = 0; i < 5; i++) {
             std::cout << data->l_h[0][i] << " ";
     }
     std::cout << std::endl;
     for (int i = 0; i < 5; i++) {
             std::cout << data->l_h[1][i] << " ";
     }
}

std::vector<double> KMedoids::solve(bool is_classic=true) {
    // create csv file for results
    if (!CONSTANTS::WRITE_FILE_NAME_CSV.empty()) {}
    std::ofstream file;
    std::ofstream file_for_y;
    std::ofstream file_for_x;
    file.open(CONSTANTS::WRITE_FILE_NAME_CSV);
    //file_for_x.open("ds1x4_k64_b0_gamma_1.05_x2.txt");
    //file_for_y.open("ds1x4_k64_b0_gamma_1.05_y2.txt");
    file << "iteration,lagrangian,LB,time,gamma,gap" << std::endl;

    for (int j = 0; j < data->m; j++) {
        data->prev_g[j] = 1;
    }
    std::cout << "m" << data->m << " k" << data->k << " UB" << UB << " LB" << LB << " gamma" << gamma << " s" << s <<
    " beta" << beta << " linear_norm" << linear_norm << " g[1]" << data->g[1] << " prev_g[1]" << data->prev_g[1] << " lambda_v[1]" << data->lambda_v[1]
    << " l_h[1] " << data->l_h[1][0] << " d_ij[1] " << data->d_ij[1][0] << std::endl;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        // [2]
        cost_calculator->compute_with_classic(*data);
        // [3]
        if (cost_calculator->lagrangian > LB) {
            LB = cost_calculator->lagrangian;
            beta = 0;
        }
        // [4]
        if (LB / UB >= CONSTANTS::GAP_THRESHOLD) {
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            file << s << "," << cost_calculator->lagrangian << "," << LB << "," << elapsed.count() << "," << gamma << "," << (UB-LB)/UB*100 << std::endl;
//            for (int i = 0; i < m; i++) {
//                for (int j = 0; j < m; j++) {
//                    file_for_x << x_ij[i][j] << " ";
//                }
//                file_for_x << std::endl;
//            }
//            file_for_x << std::endl;
//            for (int i = 0; i < m; i++) {
//                file_for_y << y_i[i] << " ";
//            }
//            file_for_y << std::endl;

            std::cout << "Difference is lower" << std::endl;
            break;
        }
        // [5]
        subgradient_calculator->compute_with_classic(*data);

//        finish = std::chrono::high_resolution_clock::now();
//         elapsed = finish - start;
//        std::cout << "compute y and g: " << elapsed.count() << std::endl;

        linear_norm = 0.;
//        linear_norm = (inner_product(data.g.begin(), data.g.end(), data.g.begin(), 0.0));
        for (int j = 0; j < data->m; j++) {
            linear_norm += data->g[j] * data->g[j];
        }
        if (linear_norm <= 1e-5) {
            std::cout << "Linear norm is lower" << std::endl;
            break;
        }

        if (beta >= CONSTANTS::B_MAX) {
            gamma = gamma / 1.01;
            beta = 0;
        }
        else {
            beta += 1;
        }

        if (gamma < 4e-4) {
//            for (int i = 0; i < data.m; i++) {
//                for (int j = 0; j < data.m; j++) {
//                    file_for_x << x_ij[i][j] << " ";
//                }
//                file_for_x << std::endl;
//            }
//            file_for_x << std::endl;
//
//            for (int i = 0; i < m; i++) {
//                file_for_y << y_i[i] << " ";
//            }
//            file_for_y << std::endl;
//            std::cout << "Gamma is lower" << std::endl;
            break;
        }

        double alpha1 = gamma * (1.05 * UB - cost_calculator->lagrangian) / linear_norm;

        lambda_calculator->compute_with_classic(*data, alpha1, s);



        std::cout << s << "  " << LB << " " << alpha1 << " " << linear_norm << std::endl;
        
        s += 1;
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        file << s << "," << cost_calculator->lagrangian << "," << LB << "," << elapsed.count() << "," << gamma << "," << (UB-LB)/UB*100 << std::endl;
        std::cout << "LB: " << LB << std::endl;
//        std::cout << "Lagr: " << lagrangian << std::endl;
//        std::cout << "lin_norm: " << lin_norm << std::endl;
//        std::cout << "gamma: " << gamma << std::endl;
        //save file
        file.flush();
    }
    file.close();

    return {LB, UB};
}