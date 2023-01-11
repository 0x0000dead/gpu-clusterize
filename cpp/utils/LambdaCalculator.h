#ifndef LAMBDACALCULATOR_H
#define LAMBDACALCULATOR_H

#include "Data.h"

class LambdaCalculator {
public:
    LambdaCalculator(int m, bool is_use_upper_bound=true, bool is_use_lower_bound=false);

    void compute_with_classic(VectorData & data, const double alpha, const int s);
    void compute_with_volume_algorithm(VectorData & data, const double alpha, const int s);
    void compute_with_prev_algorithm(VectorData & data, const double alpha, const int s);

private:
    std::vector<double> b_upper_bound;
    std::vector<double> b_lower_bound;

    bool is_use_upper_bound;
    bool is_use_lower_bound;
};


#endif //LAMBDACALCULATOR_H
