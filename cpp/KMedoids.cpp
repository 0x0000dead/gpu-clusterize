//
// Created by Sergey on 12/11/2022.
//

#include "KMedoids.h"
#include <iostream>
#include <algorithm>
#include <cmath>
# include <vector>
#include <numeric>
#include <chrono>

KMedoids::KMedoids(int k, int m, std::vector<std::vector<double>>& d_ij, double UB) :  k(k), m(m), d_ij(d_ij), UB(UB) {
    // initialize the variables
    x_ij = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));
    nu_ij = std::vector<std::vector<double>>(m, std::vector<double>(m, 0));
    g = std::vector<double>(m, 0);
    y_i = std::vector<int>(m, 0);
    indexes_map = std::vector<int>(m, 0);

    ro_sorted = std::vector<double>(m, 0);

    l_h = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));

    for (int i = 0; i < m; i++) {
        std::vector<int> indexes(m);
        std::iota(indexes.begin(), indexes.end(), 0);
        std::sort(indexes.begin(), indexes.end(), [i, &d_ij](int a, int b) { return d_ij[a][i] < d_ij[b][i]; });
        for (int j = 0; j < m; j++) {
            l_h[j][i] = indexes[j];
        }
    }
//     std::cout << "l_h" << std::endl;
//     for (int i = 0; i < m; i++) {
//             std::cout << d_ij[i][l_h[0][i]] << " ";
//     }

    lambda_v = std::vector<double>(m, lambda_val);
    ro = std::vector<double>(m, -lambda_val);
    for (int i = 0; i < m; i++) {
        lambda_v[i] = d_ij[i][l_h[0][i]];
        ro[i] = -lambda_v[i];
    }

    LB = INT_MIN;
    s = 0;
    gamma = 1.55;
    beta = 0;
    b_max = 3;
    lagrangian = 0;
}

void KMedoids::compute_cost_dual_classic() {
    ro = std::vector<double>(m, 0);
    for (int i = 0; i < m; i++) {
        double total_sum = 0;
        for (int j = 0; j < m; j++) {
            if (i != j) {
                total_sum += std::min(0.0, d_ij[i][j] - lambda_v[j]);
            }
        }
        ro[i] = total_sum - lambda_v[i];
    }
    std::iota(indexes_map.begin(), indexes_map.end(), 0);
    std::sort(indexes_map.begin(), indexes_map.end(), [this](int i1, int i2) {return ro[i1] < ro[i2]; });
    for (int i = 0; i < m; i++) {
        ro_sorted[i] = ro[indexes_map[i]];
    }
    lagrangian = std::accumulate(ro_sorted.begin(), ro_sorted.begin() + k, 0.0) + std::accumulate(lambda_v.begin(), lambda_v.end(), 0.0);
}


void KMedoids::compute_y_and_g(const bool is_classic) {
    y_i = std::vector<int>(m, 0);
    for (int i = 0; i < k; i++) {
        y_i[indexes_map[i]] = 1;
    }
    if (is_classic) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                nu_ij[i][j] = d_ij[i][j] - lambda_v[j];
                if (i != j) {
                    if (y_i[i] == 1 && (d_ij[i][j] - lambda_v[j]) < 0) {
                        x_ij[i][j] = 1;
                    }
                    else {
                        x_ij[i][j] = 0;
                    }
                }
            }
        }
    }
    else {
        int j_local = 0;
        int h_local = 0;
        bool is_two;
        bool is_break = false;
        x_ij = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));
        while (true) {
            is_two = false;
            h_local = 0;
            while (true) {
                if (d_ij[l_h[h_local][j_local]][j_local] - lambda_v[j_local] >= 1e-8) {
                    if (j_local < m - 1) {
                        is_two = true;
                        j_local += 1;
                        break;
                    } else {
                        is_break = true;
                        break;
                    }
                } else {
                    if (y_i[l_h[h_local][j_local]] == 1 && l_h[h_local][j_local] != j_local) {
                        x_ij[l_h[h_local][j_local]][j_local] = 1;
                    }
                    if (h_local < m - 1) {
                        h_local += 1;
                    } else if (j_local < m - 1) {
                        is_two = true;
                        j_local += 1;
                        break;
                    } else {
                        is_break = true;
                        break;
                    }
                }
            }
            if (is_two) {
                continue;
            } else if (is_break) {
                break;
            }
        }
    }

    for (int j = 0; j < m; j++) {
        double total_sum = 0;
        for (int i = 0; i < m; i++) {
            total_sum += x_ij[i][j];
        }
        g[j] = 1 - total_sum - y_i[j];
    }
}

void KMedoids::compute_cost_dual_subcolumn() {
    bool is_break = false;
    lagrangian = 0;
    for (int i = 0; i < m; i++) {
        ro[i] = -lambda_v[i];
    }
    j_glob = 0;
    bool is_two;
    int h = 0;
    while (true) {
        lagrangian = lagrangian + lambda_v[j_glob];
        is_two = false;
        h = 0;

        while (true) {
            if (d_ij[l_h[h][j_glob]][j_glob] - lambda_v[j_glob] >= 1e-8) {
                if (j_glob < m - 1) {
                    is_two = true;
                    j_glob += 1;
                    break;
                }
                else {
                    is_break = true;
                    break;
                }
            }
            else {
                ro[l_h[h][j_glob]] = ro[l_h[h][j_glob]] + d_ij[l_h[h][j_glob]][j_glob] - lambda_v[j_glob];
                if (h < m - 1) {
                    h += 1;
                }
                else if (j_glob < m - 1) {
                    is_two = true;
                    j_glob += 1;
                    break;
                }
                else {
                    is_break = true;
                    break;
                }
            }
        }
        if (is_two) {
            continue;
        }
        else if (is_break) {
            break;
        }
    }
    std::iota(indexes_map.begin(), indexes_map.end(), 0);
    std::sort(indexes_map.begin(), indexes_map.end(), [this](int i1, int i2) {return ro[i1] < ro[i2]; });

    ro_sorted = ro;
    std::sort(ro_sorted.begin(), ro_sorted.end());
    lagrangian = lagrangian + std::accumulate(ro_sorted.begin(), ro_sorted.begin() + k, 0.0);

 }

#include <iostream>
#include <ostream>
#include <fstream>

std::vector<double> KMedoids::solve(bool is_classic=true) {
    // create csv file for results
    std::ofstream file;
    file.open("ds1x4_k64_b3_gamma_1.55_bound.csv");
    file << "iteration,lagrangian,LB,time,gamma,gap" << std::endl;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        if (is_classic) {
            compute_cost_dual_classic();
        }
        else {
            compute_cost_dual_subcolumn();
        }

//        std::cout << "compute cost dual: " << elapsed.count() << std::endl;

        if (lagrangian > LB) {
            LB = lagrangian;
            beta = 0;
        }
        if (LB / UB >= 1 - 1e-4) {
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            file << s << "," << lagrangian << "," << LB << "," << elapsed.count() << "," << gamma << "," << (UB-LB)/UB*100 << std::endl;
            std::cout << "Difference is lower" << std::endl;
            break;
        }
//        start = std::chrono::high_resolution_clock::now();
        compute_y_and_g(false); // is classic
//        finish = std::chrono::high_resolution_clock::now();
//        elapsed = finish - start;
//        std::cout << "compute y and g: " << elapsed.count() << std::endl;

        double lin_norm = 0;
        for (int j = 0; j < m; j++) {
            lin_norm += g[j] * g[j];
        }
        if (lin_norm <= 1e-5) {
            std::cout << "Linear norm is lower" << std::endl;
            break;
        }

        if (beta >= b_max) {
            gamma = gamma / 1.01;
            beta = 0;
        }
        else {
            beta += 1;
        }

        if (gamma < 1e-3) {
            std::cout << "Gamma is lower" << std::endl;
            break;
        }
        double alpha = gamma * (1.05 * UB - lagrangian) / lin_norm;
        std::vector<double> b_upper_bound(m, 0);

        for (int j = 0; j < m; j++) {
            b_upper_bound[j] = d_ij[j][l_h[s+1][j]];
        }
        for (int j = 0; j < m; j++) {
            lambda_v[j] += alpha * g[j];
            if (lambda_v[j] > b_upper_bound[j]) {
                lambda_v[j] = b_upper_bound[j];
            }
        }

        std::cout << LB << " " << lambda_v[0] << " " << alpha << " " <<lin_norm << std::endl;
        
        s += 1;
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        file << s << "," << lagrangian << "," << LB << "," << elapsed.count() << "," << gamma << "," << (UB-LB)/UB*100 << std::endl;
//        std::cout << "LB: " << LB << std::endl;
//        std::cout << "Lagr: " << lagrangian << std::endl;
//        std::cout << "lin_norm: " << lin_norm << std::endl;
//        std::cout << "gamma: " << gamma << std::endl;
        //save file
        file.flush();
    }
    file.close();

    return {LB, UB};
}