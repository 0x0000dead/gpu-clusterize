//
// Created by Sergey on 1/11/2023.
//

#ifndef HELPER_H
#define HELPER_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
namespace helper {
    // write to file
    void write_to_file(std::string file_name, std::vector<double> & data) {
        std::ofstream file;
        file.open(file_name);
        for (int i = 0; i < data.size(); ++i) {
            file << data[i] << std::endl;
        }
        file.close();
    }
}
#endif //HELPER_H
