#ifndef COSTCALCULATOR_H
#define COSTCALCULATOR_H

#include <vector>
#include <numeric>
#include <algorithm>
#include "Data.h"

// TODO: can be abstract class and inherit different types
class CostCalculator {
public:
    std::vector<double> ro;
    std::vector<double> ro_sorted;

    double lagrangian;

    CostCalculator(int m, const std::vector<double>& lambda_v);

    /**
     * Video Matting Using RVM(https://github.com/PeterL1n/RobustVideoMatting)
     * @param video_path: eg. xxx/xxx/input.mp4
     * @param output_path: eg. xxx/xxx/output.mp4
     * @param contents: vector of MattingContent to catch the detected results.
     * @param save_contents: false by default, whether to save MattingContent.
     * @param downsample_ratio: 0.25 by default.
     * See https://github.com/PeterL1n/RobustVideoMatting/blob/master/documentation/inference_zh_Hans.md
     * @param writer_fps: FPS for VideoWriter, 20 by default.
     * @param remove_noise: remove small connected area or not
     * @param minimum_post_process: if True, will run matting with minimum post process
     * in order to speed up the matting processes.
     * @param background: user's custom background setting, will return with this target
     * background if background Mat is not empty instead of green background.
     */
    void compute_with_classic(VectorData & data);

    /**
     * Video Matting Using RVM(https://github.com/PeterL1n/RobustVideoMatting)
     * @param video_path: eg. xxx/xxx/input.mp4
     * @param output_path: eg. xxx/xxx/output.mp4
     * @param contents: vector of MattingContent to catch the detected results.
     * @param save_contents: false by default, whether to save MattingContent.
     * @param downsample_ratio: 0.25 by default.
     * See https://github.com/PeterL1n/RobustVideoMatting/blob/master/documentation/inference_zh_Hans.md
     * @param writer_fps: FPS for VideoWriter, 20 by default.
     * @param remove_noise: remove small connected area or not
     * @param minimum_post_process: if True, will run matting with minimum post process
     * in order to speed up the matting processes.
     * @param background: user's custom background setting, will return with this target
     * background if background Mat is not empty instead of green background.
    */
    void compute_with_column_generation(VectorData & data);

private:
    int m;

};


#endif //COSTCALCULATOR_H
