
#include "Points.h"
#include <iostream>
#include <strstream>

Points::Points(const std::string &f_name) {
    std::ifstream file(f_name);
    if (file.is_open()) {
        file >> m >> dim;
    }
    // reserve space for vector of vector all_points
    all_points.reserve(m);

    read_points(file);
}

void Points::read_points(std::ifstream &f_stream) {
    if (f_stream.is_open()) {
        f_stream >> m >> dim;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < dim; j++) {
                f_stream >> all_points[i][j];
            }
        }
    }
}

Points::Points(std::ifstream &f_stream) {
    read_points(f_stream);
}


void Points::print_distance_matrix(int k, int p) const {
    k = k == -1 ? distance_matrix.size() : k;
    p = p == -1 ? distance_matrix[0].size() : p;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < p; j++) {
            std::cout << distance_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<double>> Points::get_distance_matrix(bool is_sorted, const std::string& fname) {
    // todo: add sorted for realtime calculation
    if (fname.empty()) {
        if (is_sorted) {
            auto tmp = faster_read_matrix(fname, m, m);

        }
        return read_distance_matrix(fname);
    }
    else return calculate_distance_matrix();
}

std::vector <std::vector<double>> Points::calculate_distance_matrix() {
    int size = all_points.size();
 /// TODO REALLOCATE
    std::vector <std::vector<double>> distance_matrix(size, std::vector<double>(size));

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            double sum = 0;
            for (int k = 0; k < all_points[i].size(); ++k) {
                sum += (all_points[i][k] - all_points[j][k]) * (all_points[i][k] - all_points[j][k]);
            }
            distance_matrix[i][j] = sqrt(sum);
            distance_matrix[j][i] = sqrt(sum);
        }
        distance_matrix[i][i] = INT_MAX;
    }
    return distance_matrix;
}


std::vector <std::vector<double>> Points::read_distance_matrix(const std::string& fname) {
    std::ifstream rfile(fname);
    std::vector <std::vector<double>> distance_matrix(m, std::vector<double>(m, 0));
    if (rfile.is_open()) {
        std::string line;
        int i = 0;
        while (std::getline(rfile, line)) {
            std::strstream ss;
            ss << line;
            for (int j = 0; j < m; ++j) {
                ss >> distance_matrix[i][j];
            }
            ++i;
            std::cout << i << std::endl;
        }

    }
    rfile.close();
    return distance_matrix;
}

std::vector<std::vector<int>> Points::faster_read_matrix(const std::string& fname, int numRows, int numCols)
{
    FILE * fpIn = fopen(fname.c_str(), "r");
    std::vector<std::vector<int>> tmp_matrix(numRows, std::vector<int>(numCols, 0));
    if (fpIn == NULL)
    {
        printf("Couldn't open file [%s] for input!\n", fname.c_str());
        return {};
    }

    int* K = new int[numRows*numCols];

    // We'll hold the current number in (numberBuf) until we're ready to parse it
    char numberBuf[128] = {'\0'};
    int numCharsInBuffer = 0;

    int curRow = 0, curCol = 0;
    while(curRow < numRows)
    {
        char tempBuf[4*1024];  // an arbitrary size
        const size_t bytesRead = fread(tempBuf, 1, sizeof(tempBuf), fpIn);
        if (bytesRead <= 0)
        {
            if (bytesRead < 0) perror("fread");
            break;
        }

        for (size_t i=0; i<bytesRead; i++)
        {
            const char c = tempBuf[i];
            if ((c=='.')||(c=='+')||(c=='-')||(isdigit(c)))
            {
                if ((numCharsInBuffer+1) < sizeof(numberBuf)) numberBuf[numCharsInBuffer++] = c;
                else
                {
                    printf("Error, number string was too long for numberBuf!\n");
                }
            }
            else
            {
                if (numCharsInBuffer > 0)
                {
                    // Parse the current number-chars we have assembled into (numberBuf) and reset (numberBuf) to empty
                    numberBuf[numCharsInBuffer] = '\0';
                    if (curCol < numCols) K[curRow*numCols+curCol] = strtod(numberBuf, NULL);
                    else
                    {
                        printf("Error, too many values in row %i!  (Expected %i, found at least %i)\n", curRow, numCols, curCol);
                    }
                    curCol++;
                }
                numCharsInBuffer = 0;

                if (c == '\n')
                {
                    curRow++;
                    curCol = 0;
                    if (curRow >= numRows) break;
                }
            }
        }
    }
    fclose(fpIn);

    if (curRow != numRows) printf("Warning:  I read %i lines in the file, but I expected there would be %i!\n", curRow, numRows);

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            tmp_matrix[i][j] = K[i*numCols + j];
        }
    }
    delete[] K;
    return tmp_matrix;
}