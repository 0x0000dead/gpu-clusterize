//
// Created by Sergey on 12/11/2022.
//

#ifndef CPP_KMEDOIDS_H
#define CPP_KMEDOIDS_H
#include <vector>


class KMedoids {
public:
    // define the constructor with an initializer list
    KMedoids(int k, int m, std::vector<std::vector<double>>& d_ij, double UB);

    std::vector<double> solve(bool is_classic);

private:
    void compute_cost_dual_classic();

    void compute_y_and_g();

    void compute_cost_dual_subcolumn();

    int k, m;
    std::vector<std::vector<double>> d_ij;
    double UB;
    int lambda_val = 5;
    std::vector<double> lambda_v;
    std::vector<std::vector<int>> x_ij;
    std::vector<std::vector<double>> nu_ij;
    std::vector<double> g;
    std::vector<int> y_i;
    std::vector<int> indexes_map;
    std::vector<double> ro;
    std::vector<double> ro_sorted;

    std::vector<std::vector<int>> l_h;

    double LB;
    int s;
    double gamma;
    int beta;
    int b_max;
    double lagrangian;
    int j_glob = 0;
};


#endif //CPP_KMEDOIDS_H
