#ifndef SUBGRADIENTCALCULATOR_H
#define SUBGRADIENTCALCULATOR_H

#include <vector>
#include "Data.h"

class SubgradientCalculator {
public:
    SubgradientCalculator(int m);

    void compute_with_classic(VectorData & data);

    void compute_with_column_generation(VectorData & data);

private:
    std::vector<std::vector<double>> nu_ij;

};


#endif //SUBGRADIENTCALCULATOR_H
