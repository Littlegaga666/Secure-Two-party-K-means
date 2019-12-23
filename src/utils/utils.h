/**
 * \file    utils.h
 * \author  Jamie Cui
 * \brief   namespace bioutils data class Implementation, General Entry Points for utils Library
 */
#ifndef BIOSECURE_UTILS_H
#define BIOSECURE_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

/**
 * @brief   namespace bioutils data
 */
namespace bioutils{

    /**
     * Split string implementation (internal function)
     * @param str: string to split
     * @param ret: returned string vector
     * @param deli: delimiter
     */
    inline void split_string(const std::string &str, std::vector<std::string> &ret, const std::string &deli) {
        std::string::size_type pos1, pos2;
        pos2 = str.find(deli);
        pos1 = 0;
        while (std::string::npos != pos2) {
            if (str.substr(pos1, pos2 - pos1).size() && !isspace(str.substr(pos1).c_str()[0]))
                ret.push_back(str.substr(pos1, pos2 - pos1));
            pos1 = pos2 + deli.size();
            pos2 = str.find(deli, pos1);
        }
        if (pos1 != str.length() && str.substr(pos1).size() && !isspace(str.substr(pos1).c_str()[0]))
            ret.push_back(str.substr(pos1));
    }

    /**
     * Get the line number of a certain file
     * @param file
     * @return line numbers, -1 on error
     */
    inline uint32_t get_line_num(std::string file) {
        std::ifstream in(file);
        if (!in.is_open()) {
            std::cerr << "File open error: " << file << std::endl;
            return -1;
        }
        std::string temp;
        int count = 0;
        std::getline(in, temp);
        while (!temp.empty()) {
            count++;
            std::getline(in, temp);
        }
        in.close();
        return count;
    }
}

#endif //BIOSECURE_UTILS_H
