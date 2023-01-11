#ifndef HELLOWORLD_MATRIX_H
#define HELLOWORLD_MATRIX_H

#include <string>
#include <vector>
#include <fstream>
#include <istream>

class Points {
public:
    Points(const std::string &f_name);
    Points(std::ifstream &f_stream);
    Points(const std::vector<std::vector<double>> &points, int m, int dim=2);

    ~Points();

    void print_points() const;
    void print_points(std::string &f_name) const;
    void print_points(std::ofstream &f_stream) const;

    void print_distance_matrix(int k=10, int p=10) const;

    std::vector<std::vector<double>> get_distance_matrix(bool is_fast=true, const std::string& fname= "");
    std::vector<std::vector<double>> get_distance_matrix(int m, int n);

    int m;
    int dim;

private:
    std::vector<std::vector<double>> distance_matrix;
    std::vector<std::vector<double>> all_points;

    void read_points(std::ifstream &f_stream);
    std::vector <std::vector<double>> calculate_distance_matrix();
    std::vector <std::vector<double>> read_distance_matrix(const std::string& fname);
    std::vector<std::vector<int>> Points::faster_read_matrix(const std::string& fname, int numRows, int numCols);

    };


#endif //HELLOWORLD_MATRIX_H
