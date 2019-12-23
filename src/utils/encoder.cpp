/**
 * \file    encoder.cpp
 * \author  Jamie Cui
 * \brief   This file implements GENERAL ENCODER, see "encoder.h" for definition
 * \warning GENERAL ENCODER is NOT THREAD-SAFE
 */
#include "encoder.h"

namespace bioutils {
    /**
     * Critical section (exclusive access to line preprocess signaled by locking mtx)
     */
    void thread_preprocess() {
        while (true) {
            std::string geneReadLine;
            bool is_end;
            mtx.lock();
            std::getline(in_gen, geneReadLine);
            is_end = in_gen.eof();
            mtx.unlock();
            if (!is_end) {
                std::vector<std::string> line_vec;
                std::string outString;
                split_string(geneReadLine, line_vec, "\t");

                char ref = line_vec.at(1).at(0);
                char alt = '/';
                if (line_vec.at(2).at(0) != '0') {
                    alt = line_vec.at(2).at(0);
                }

                outString += (line_vec.at(0) + "\t" + line_vec.at(1) + "\t" + line_vec.at(2) + "\t");

                std::string temp1, temp2_1, temp2_2, temp3;
                temp1.push_back(ref);
                temp1.push_back(ref);
                temp2_1.push_back(ref);
                temp2_1.push_back(alt);
                temp2_2.push_back(alt);
                temp2_2.push_back(ref);
                temp3.push_back(alt);
                temp3.push_back(alt);

                for (int j = 3; j < line_vec.size(); j++) {
                    if (line_vec.at(1).length() != 1 || line_vec.at(2).length() != 1) {
                        printf("Error in %s\n", geneReadLine.c_str());
                        exit(-1);
                    }

                    if (line_vec.at(j) == temp1) {
                        outString += std::to_string(0) + "\t";
                    } else if (line_vec.at(j) == temp2_1 || line_vec.at(j) == temp2_2) {
                        outString += std::to_string(1) + "\t";
                    } else if (line_vec.at(j) == temp3) {
                        outString += std::to_string(2) + "\t";
                    } else {
                        outString += std::to_string(3) + "\t";
                    }
                    mtx.lock();
                    mtx.unlock();
                }
                outString += "\n";
                mtx.lock();
                out << outString;;
                mtx.unlock();
            } else {
                break;
            }
        }


    }

    uint32_t GeneralEncoder::SplitFile(const std::string filename, std::vector<std::string> &subfile) {
        std::ifstream in;
        std::ofstream out;
        std::string headline;
        std::string case_control;
        std::string line;
        char filename1[60];
        int line_pt = 0;

        in.open(filename);
        if (!in.is_open()) {
            std::cerr << "Error opening file: " + filename << std::endl;
            return -1;
        }

        std::getline(in, headline);
        std::getline(in, case_control);

        while (std::getline(in, line)) {
            if ((line_pt % MAX_FILE_LENGTH) == 0) {
                out.close();
                sprintf(filename1, ".%s_%d.sub", filename.c_str(), line_pt / MAX_FILE_LENGTH);
                out.open(filename1);
                subfile.push_back(filename1);
                out << headline << std::endl;
                out << case_control << std::endl;
            }
            out << line << std::endl;
            line_pt++;
        }
        in.close();

        return 0;
    }

    uint32_t GeneralEncoder::CombineFile(const std::string filename, std::vector<std::string> &subfile) {
        std::ifstream in;
        std::ofstream out;
        std::string line;
        std::string headline;
        std::string case_control;

        out.open(filename);
        if (!out.is_open()) {
            std::cout << "Error opening file: " + filename << std::endl;
            return -1;
        }

        for (int i = 0; i < subfile.size(); i++) {
            in.open(subfile.at(i));
            if (!in.is_open()) {
                std::cout << "Error opening file: " + subfile.at(i) << std::endl;
                return -1;
            }
            std::getline(in, headline);
            std::getline(in, case_control);

            if (i == 0) {
                out << headline << std::endl;
                out << case_control << std::endl;
            }

            while (std::getline(in, line)) {
                out << line << std::endl;
            }
            in.close();
        }
        return 0;
    }

    uint32_t
    GeneralEncoder::Preprocess(const std::string genFlie, const std::string phFiles, const std::string bioFile) {
        std::vector<std::string> temp;
        temp.push_back(phFiles);
        return Preprocess(genFlie, temp, bioFile);
    }

    uint32_t GeneralEncoder::Preprocess(const std::string genFlie, const std::vector<std::string> phFile,
                                        const std::string bioFile) {
        out.open(bioFile);

        //////////////////////////////////////////////////
        //                   Header                     //
        //////////////////////////////////////////////////
        in_gen.open(genFlie);
        std::string geneReadLine;
        if (!in_gen.is_open()) {
            std::cerr << "Error open genotype file: " << genFlie << std::endl;
            exit(-1);
        }
        getline(in_gen, geneReadLine);
        out << geneReadLine << std::endl;

        //////////////////////////////////////////////////
        //              Processing Traits               //
        //////////////////////////////////////////////////
        for (auto file : phFile) {
            std::string line;
            std::string cur_trait = "#";
            std::vector<std::string> tmp_vec;

            in_ph.open(file);
            if (!in_ph.is_open()) {
                std::cerr << "Error open phenotype file: " << file << std::endl;
                exit(-1);
            }

            std::getline(in_ph, line);
            cur_trait += line.substr(line.find("\t") + 1);

            while (std::getline(in_ph, line) && !line.empty()) {
                if (line.find("\t") == std::string::npos)
                    split_string(line, tmp_vec, " ");   // split on " "
                else
                    split_string(line, tmp_vec, "\t");  // split on "\t"
                cur_trait += "\t" + tmp_vec.at(1);
                tmp_vec.clear();
            }

            out << cur_trait << std::endl;
            in_ph.close();
        }

        //////////////////////////////////////////////////
        //      Parallel Processing every line          //
        //////////////////////////////////////////////////
        std::thread threads[THREAD_NUM];
        for (int i = 0; i < THREAD_NUM; ++i)
            threads[i] = std::thread(thread_preprocess);
        for (auto &th : threads) th.join();

        in_gen.close();
        in_ph.close();
        out.close();
        return 0;
    }

    bool GeneralEncoder::IsFileLegit(const std::string filename) {
        // TODO
        return true;
    }

    bool GeneralEncoder::IsFileEmpty(const std::string filename) {
        std::ifstream in;
        in.open(filename);
        std::string line;
        std::getline(in, line); // skip header
        std::getline(in, line);
        while (line.at(0) == '#') { // skip while line begins with "#" (traits line)
            std::getline(in, line);
        }
        return line.empty();
    }
}
