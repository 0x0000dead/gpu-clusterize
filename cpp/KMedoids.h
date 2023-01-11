#ifndef CPP_KMEDOIDS_H
#define CPP_KMEDOIDS_H

#include <vector>
#include "utils/Data.h"

#include "utils/CostCalculator.h"
#include "utils/SubgradientCalculator.h"
#include "utils/LambdaCalculator.h"

#include <memory>

class KMedoids {
public:
    KMedoids(int k, int m, std::vector<std::vector<double>>& d_ij, double UB, std::vector<std::vector<int>>& faiss);

    std::vector<double> solve(bool is_classic);

private:
    std::unique_ptr<CostCalculator> cost_calculator;
    std::unique_ptr<SubgradientCalculator> subgradient_calculator;
    std::unique_ptr<LambdaCalculator> lambda_calculator;
    std::unique_ptr<VectorData> data;

    double UB;
    double LB;
    double linear_norm; // TODO: test with float
    double gamma;
    double alpha;

    int s;
    int beta;


};


#endif //CPP_KMEDOIDS_H
