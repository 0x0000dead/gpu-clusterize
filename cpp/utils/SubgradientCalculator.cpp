#include "SubgradientCalculator.h"
#include <iostream>
SubgradientCalculator::SubgradientCalculator(int m) {
    nu_ij = std::vector<std::vector<double>>(m, std::vector<double>(m, 0));
}

void SubgradientCalculator::compute_with_classic(VectorData & data) {
    // TODO reserve only once
    data.y_i = std::vector<int>(data.m, 0);
    for (int i = 0; i < data.k; ++i) {
        data.y_i[data.indexes_map[i]] = 1;
    }
    std::cout << "HER" << " " << data.indexes_map[0]<<" " << data.indexes_map[1] << " " << data.indexes_map[2]<< " "<<data.k<<" "<<data.m<<" "
    <<data.y_i[data.indexes_map[0]];

    for (int i = 0; i < data.m; ++i) {
        for (int j = 0; j < data.m; ++j) {
            nu_ij[i][j] = data.d_ij[i][j] - data.lambda_v[j];
            if (i != j) {
                if (data.y_i[i] == 1 && (data.d_ij[i][j] - data.lambda_v[j]) < 0) {
                    data.x_ij[i][j] = 1;
                }
                else {
                    data.x_ij[i][j] = 0;
                }
            }
        }
    }


    for (int j = 0; j < data.m; j++) {
        double total_sum = 0;
        for (int i = 0; i < data.m; i++) {
            total_sum += data.x_ij[i][j];
        }
        data.g[j] = 1 - total_sum - data.y_i[j];
        if(true) data.prev_g[j] = data.g[j];
    }
}

void SubgradientCalculator::compute_with_column_generation(VectorData & data) {
    int j_local = 0;
    int h_local = 0;
    bool is_two;
    bool is_break = false;
    data.x_ij = std::vector<std::vector<int>>(data.m, std::vector<int>(data.m, 0));
    while (true) {
        is_two = false;
        h_local = 0;
        while (true) {
            if (data.d_ij[data.l_h[h_local][j_local]][j_local] - data.lambda_v[j_local] >= 1e-8) {
                if (j_local < data.m - 1) {
                    is_two = true;
                    j_local += 1;
                    break;
                } else {
                    is_break = true;
                    break;
                }
            } else {
                if (data.y_i[data.l_h[h_local][j_local]] == 1 && data.l_h[h_local][j_local] != j_local) {
                    data.x_ij[data.l_h[h_local][j_local]][j_local] = 1;
                }
                if (h_local < data.m - 1) {
                    h_local += 1;
                } else if (j_local < data.m - 1) {
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
    for (int j = 0; j < data.m; j++) {
        double total_sum = 0;
        for (int i = 0; i < data.m; i++) {
            total_sum += data.x_ij[i][j];
        }
        data.g[j] = 1 - total_sum - data.y_i[j];
        //if(true) data.prev_g[j] = data.g[j];
    }
}