#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <algorithm>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>

namespace cpp_utils {

/**
 * Returns the current working directory i.e. where the executable is located.
 * @return Path the the executable as std::string.
 */
std::string get_current_directory();

/**
 * Checks whether the indicated file exists.
 * @param[in] file The path to the file including the filename.
 * @return True if file exists, false otherwise.
 */
bool check_if_file_exists(const std::string &file);

/**
 * Writes the given data into the indicated file. The data is appended at the last line of the file.
 * @param[in] data An std::array of data, the order of elements will be preserved when writing to file.
 * \param[in] file The path to the target file including the filename.
 * \param[in] If true a new line is appended at the end of the current line.
 */
template<typename T, std::size_t S> void write_data_to_file(const std::array<T, S>& data, const std::string& file, bool newline=false){
    std::ofstream s;
    s.open(file,std::ios::app);

    std::ostringstream oss;
    oss.imbue(std::locale::classic());
    oss.precision(std::numeric_limits<double>::digits10);
    for(unsigned i=0;i<data.size();i++){
        oss<<data[i]<<" ";
    }
    if(newline){
        oss<<std::endl;
    }
    s<<oss.str();
}

template<typename T> void write_data_to_file(const std::vector<T>& data, const std::string& file, bool newline=false){
    std::ofstream s;
    s.open(file,std::ios::app);

    std::ostringstream oss;
    oss.imbue(std::locale::classic());
    oss.precision(std::numeric_limits<double>::digits10);
    for(unsigned i=0;i<data.size();i++){
        oss<<data[i]<<" ";
    }
    if(newline){
        oss<<std::endl;
    }
    s<<oss.str();
}

/**
 * Writes the given data into the indicated file. The data is appended at the last line of the file.
 * @param[in] data A scalar value.
 * \param[in] file The path to the target file including the filename.
 * \param[in] If true a new line is appended at the end of the current line.
 */
template<typename T> void write_data_to_file(T data, const std::string& file,bool newline=false){
    std::ofstream s;
    s.open(file,std::ios::app);

    std::ostringstream oss;
    oss.imbue(std::locale::classic());
    oss.precision(std::numeric_limits<double>::digits10);
    oss<<data<<" ";
    if(newline){
        oss<<std::endl;
    }
    s<<oss.str();
}

/**
 * Appends an endline to the current last line of the indicated file.
 * @param[in] file file The path to the target file including the filename.
 */
void write_endl_to_file(const std::string& file);


template<typename T> bool read_data_from_file(const std::string& file,std::vector<T>& data){
    std::ifstream ifile(file, std::ios::in);
    data.resize(0);

    //check to see that the file was opened correctly:
    if (!ifile.is_open()) {
        std::cerr << "There was a problem opening the input file!\n";
        return false;
    }

    double num = 0.0;
    //keep storing values from the text file so long as data exists:
    while (ifile >> num) {
        data.push_back(num);
    }
    return true;
}

/**
 * Splits the given string into multiple substrings at the indicated delimiter.
 * @param[in] s String that will be split.
 * @param[in] delimiter Character at which the string s will be split.
 * @return A vector of partial strings, the delimiter charackter is not contained.
 */
std::vector<std::string> split_string(std::string s, const std::string& delimiter);

/**
 * Transforms a given character sequence into a corresponding int variable.
 * @param[in] str Sequence of chars.
 * @param[in] h Step parameter for recursive call.
 * @return Int value that corresponds to the given character sequence.
 */
constexpr unsigned int str_to_int(const char* str, int h=0){
    return !str[h] ? 5381 : (str_to_int(str, h+1) * 33) ^ str[h];
}

}
