/**
 * \file    encoder.h
 * \author  Jamie Cui
 * \brief   This file defines GENERAL ENCODER, contains mostly file operations
 * \warning GENERAL ENCODER is NOT THREAD-SAFE
 */
#ifndef BIOSECURE_ENCODER_H
#define BIOSECURE_ENCODER_H

#include <cassert>
#include <thread>
#include <mutex>
#include "utils.h"

#define THREAD_NUM 4            // Number of threads used for preprocess
#define MAX_FILE_LENGTH 500     // Maximum file length for self-defined subfiles

namespace bioutils {
    class GeneralEncoder {
    public:
        /**
         * Split file into self-defined subfiles
         * @param filename: Absolute or relative path to the input file
         * @param subfile: Returned subfile names/paths (absolute/relative)
         * @return -1 on error, 0 otherwise
         */
        uint32_t SplitFile(const std::string filename, std::vector<std::string> &subfile);

        /**
         * Combine subfiles into one file
         * @param filename: Absolute or relative path to the subfiles
         * @param subfile: Returned combined name/path (absolute/relative)
         * @return -1 on error, 0 otherwise
         */
        uint32_t CombineFile(const std::string filename, std::vector<std::string> &subfile);

        /**
         * Preprocess genFlie and phFile into standard bioSecure file format, format defined in "data.h"
         * @param genFlie: Absolute or relative path to a genotype file
         * @param phFile: Absolute or relative path to a phenotype file
         * @param bioFile: Absolute or relative path to the result biofile
         * @return -1 on error, 0 otherwise
         */
        uint32_t Preprocess(const std::string genFlie, const std::string phFile, const std::string bioFile);

        /**
         * Preprocessing genFlie and multiple phFile into standard bioSecure file format, format defined in "data.h"
         * @param genFlie: Absolute or relative path to a genotype file
         * @param phFiles: Absolute or relative path to multiple phenotype files
         * @param bioFile: Absolute or relative path to the result biofile
         * @return -1 on error, 0 otherwise
         */
        uint32_t
        Preprocess(const std::string genFlie, const std::vector<std::string> phFiles, const std::string bioFile);

        /**
         * Check if file is empty
         * @param filename: Absolute or relative path to a file
         * @return True/False
         */
        bool IsFileEmpty(const std::string filename);

        /**
         * Check if file is legit
         * @param filename: Absolute or relative path to a file
         * @return True/False
         */
        bool IsFileLegit(const std::string filename);
    };

    static std::mutex mtx; // Global mutex locks for threads in "encoder.cpp"
    static std::ifstream in_gen;
    static std::ifstream in_ph;
    static std::ofstream out;
}

#endif
