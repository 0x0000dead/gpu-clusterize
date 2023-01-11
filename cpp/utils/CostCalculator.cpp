#include "CostCalculator.h"

#include <iostream>
CostCalculator::CostCalculator(int m, const std::vector<double>& lambda_v) : m(m) {
    ro_sorted = std::vector<double>(m, 0);
    ro = std::vector<double>(m, 0);
    lagrangian = 0;

    // fill initial cost with -lambda value, according [22]
    for (int i = 0; i < m; ++i) {
        ro[i] = -lambda_v[i];
    }
}

void CostCalculator::compute_with_classic(VectorData & data) {
    for (int i = 0; i < m; i++) {
        double total_sum = 0;
        for (int j = 0; j < m; j++) {
            if (i != j) {
                total_sum += std::min(0.0, data.d_ij[i][j] - data.lambda_v[j]);
            }
        }
        ro[i] = total_sum - data.lambda_v[i];
    }
    std::cout << "HERE" << " " << ro[0]<< " " << ro[1]<< " " << ro[2]<< " " << ro[3]<< " " << ro[4] << std::endl;

    std::iota(data.indexes_map.begin(), data.indexes_map.end(), 0);
    std::cout << "HER" << " " << data.indexes_map[0]<<" " << data.indexes_map[1] << " " << data.indexes_map[2]<< std::endl;
    std::sort(data.indexes_map.begin(), data.indexes_map.end(), [this](int i1, int i2) {return ro[i1] < ro[i2]; });
    for (int i = 0; i < m; ++i) {
        ro_sorted[i] = ro[data.indexes_map[i]];
    }
    std::cout << "HERE" << " " << ro_sorted[0]<< " " << ro_sorted[1]<< " " << ro_sorted[2]<< " " << ro_sorted[3]<< " " << ro_sorted[100] << std::endl;

    lagrangian = std::accumulate(ro_sorted.begin(), ro_sorted.begin() + data.k, 0.0) +
                 std::accumulate(data.lambda_v.begin(), data.lambda_v.end(), 0.0);
}


void CostCalculator::compute_with_column_generation(VectorData & data) {
    bool is_break = false;
    lagrangian = 0;
    for (int i = 0; i < m; i++) {
        ro[i] = -data.lambda_v[i];
    }
    int j_glob = 0;
    bool is_two;
    int h = 0;
    while (true) {
        lagrangian = lagrangian + data.lambda_v[j_glob];
        is_two = false;
        h = 0;

        while (true) {
            if (data.d_ij[data.l_h[h][j_glob]][j_glob] - data.lambda_v[j_glob] >= 1e-8) {
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
                ro[data.l_h[h][j_glob]] = ro[data.l_h[h][j_glob]] + data.d_ij[data.l_h[h][j_glob]][j_glob] - data.lambda_v[j_glob];
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
    std::iota(data.indexes_map.begin(), data.indexes_map.end(), 0);
    std::sort(data.indexes_map.begin(), data.indexes_map.end(), [this](int i1, int i2) {return ro[i1] < ro[i2]; });

    ro_sorted = ro;
    std::sort(ro_sorted.begin(), ro_sorted.end());
    lagrangian = lagrangian + std::accumulate(ro_sorted.begin(), ro_sorted.begin() + data.k, 0.0);
}
