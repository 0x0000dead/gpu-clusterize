#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <vector>
#include <iostream>
const int N(10);

#define check(ans) { _check((ans), __FILE__, __LINE__); }
inline void _check(cudaError_t code, char* file, int line)
{
    if (code != cudaSuccess) {
        fprintf(stderr, "CUDA Error: %s %s %d\n", cudaGetErrorString(code), file, line);
        exit(code);
    }
}

int div_up(int a, int b) {
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
}

__global__ void calc_distances_kernel(double* distances, double** atoms, int num_dimensions);

__global__ void calc_distances_kernel(double* distances, double* atoms, int num_dimensions)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx >= N) return;

    for (int j = idx + 1; j < N; ++j) {
        double sum = 0;
        for (int k = 0; k < num_dimensions; ++k) {
            sum+= (atoms[idx * num_dimensions + k] - atoms[j * num_dimensions + k]) * (atoms[idx * num_dimensions + k] - atoms[j * num_dimensions + k]);
        }
        distances[idx * N + j] = sqrt(sum);
        distances[j * N + idx] = sqrt(sum);
    }
}

void calculate_distance_matrix_cuda(double *input_ij_1d, double *output_ij_1d, int num_dimensions) {
    double* distances_d;
    double* all_points_d;

    check(cudaMalloc(&distances_d, N * N * sizeof(double)));
    check(cudaMalloc(&all_points_d, N * num_dimensions * sizeof(double)));
    check(cudaMemcpy(all_points_d, input_ij_1d, N * num_dimensions * sizeof(double), cudaMemcpyHostToDevice));

    const int threads_per_block(512);
    dim3 n_blocks(div_up(N, threads_per_block));

    calc_distances_kernel <<< n_blocks, threads_per_block >>> (distances_d, all_points_d, num_dimensions);

    check(cudaPeekAtLastError());
    check(cudaDeviceSynchronize());


    check(cudaMemcpy(output_ij_1d, distances_d, N * N * sizeof(double), cudaMemcpyDeviceToHost));


    check(cudaFree(all_points_d));
    check(cudaFree(distances_d));
}
