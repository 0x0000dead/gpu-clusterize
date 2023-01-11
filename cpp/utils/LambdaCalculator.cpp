#include "LambdaCalculator.h"

LambdaCalculator::LambdaCalculator(int m, bool is_use_upper_bound, bool is_use_lower_bound) : is_use_upper_bound(is_use_upper_bound), is_use_lower_bound(is_use_lower_bound) {
    b_upper_bound = std::vector<double>(m, 0);
    b_lower_bound = std::vector<double>(m, 0);
}

void LambdaCalculator::compute_with_classic(VectorData & data, const double alpha, const int s) {
    for (int j = 0; j < data.m; j++) {
        data.lambda_v[j] += alpha * data.g[j];
        if (is_use_upper_bound) {
            b_upper_bound[j] = data.d_ij[j][data.l_h[s + 1][j]];

            if (data.lambda_v[j] > b_upper_bound[j]) {
                data.lambda_v[j] = b_upper_bound[j];
            }
        }
    }
}

void LambdaCalculator::compute_with_prev_algorithm(VectorData & data, const double alpha, const int s) {
    double beta_1 = 0.7;
    double beta_2 = 0.3;
    for (int j = 0; j < data.m; j++) {
        data.lambda_v[j] = data.lambda_v[j] + alpha * (beta_1 * data.prev_g[j] - beta_2 * data.g[j]);
        data.prev_g[j] = data.g[j];
        if (is_use_upper_bound) {
            b_upper_bound[j] = data.d_ij[j][data.l_h[s + 1][j]];

            if (data.lambda_v[j] > b_upper_bound[j]) {
                data.lambda_v[j] = b_upper_bound[j];
            }
        }
    }
}

void LambdaCalculator::compute_with_volume_algorithm(VectorData & data, const double alpha, const int s) {
    return;
}
