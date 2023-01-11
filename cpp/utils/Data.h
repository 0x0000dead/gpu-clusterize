#ifndef DATA_H
#define DATA_H

#include <vector>

struct VectorData {

    int k;
    int m;
    std::vector<std::vector<double>> d_ij;
    std::vector<std::vector<int>> l_h = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));

    std::vector<double> lambda_v;

    std::vector<int> y_i = std::vector<int>(m, 0);
    std::vector<std::vector<int>> x_ij = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));

    std::vector<double> g = std::vector<double>(m, 0);
    std::vector<double> prev_g = std::vector<double>(m, 0);
    std::vector<int> indexes_map = std::vector<int>(m, 0);

};

struct ArrayData {
    /*
    int k;
    int m;
    std::vector<std::vector<double>> d_ij;
    double UB;
    std::vector<std::vector<int>> faiss;

    x_ij = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));
    nu_ij = std::vector<std::vector<double>>(m, std::vector<double>(m, 0));
    g = std::vector<double>(m, 0);
    prev_g = std::vector<double>(m, 0);
    y_i = std::vector<int>(m, 0);
    indexes_map = std::vector<int>(m, 0);
    l_h = std::vector<std::vector<int>>(m, std::vector<int>(m, 0));
*/
};

#endif

