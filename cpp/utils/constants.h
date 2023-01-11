#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
namespace CONSTANTS {
    constexpr int k = 5;
    //constexpr int m = 9600;
    constexpr double UB = 1000;

    constexpr double GAP_THRESHOLD = 1 - 1e-5;
    constexpr double GAMMA_INITIAL = 1.05;
    constexpr double GAMMA_DECREASE_RATE = 1.01;
    constexpr int B_MAX = 0;

    const std::string WRITE_FILE_NAME_CSV = "ds1x4_k64_b0_gamma_1.05_release_mode_07_1_bound.csv";
}

#endif //CONSTANTS_H
