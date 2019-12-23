/**
 * \file    data.h
 * \author  Jamie Cui
 * \brief   Data Header file
 */

#ifndef BIOSECURE_DATA_H
#define BIOSECURE_DATA_H

#include "utils.h"
#include "WVector.h"

/*
    Here we define a general form of data, which is compatial with gwas format.
    All string format information should start with "#"
    [NOTE] Features are not restricted to SNP, gs could be float.

    AA = 2;
    aa = 0;
    Aa = 1;

  feature_id[] REF[] ALT[]
        |       |   |
        V       V   V
    |-------------------------------------------------------------------|
    |   Rs_ID  REF ALT NA1     NA2     NA3     NA4     NA5     NA6      |<- sample_id[]
    |   #name   0.123   0.213   0.123   0       11.23   23.12           |<- ys[][]
    |   #name   0.123   0.213   0.123   0       11.23   23.12           |<- ys[][]
    |   rs1    A   G   0       1       3       2       1       0        | \
    |   rs2    A   G   0       1       3       2       1       0        | |
    |   rs3    A   G   0       1       3       2       1       0        | | <- gs[sample][feature]
    |   rs4    A   G   0       1       3       2       1       0        | |
    |   rs5    A   G   0       1       3       2       1       0        | |
    |   rs6    A   G   0       1       3       2       1       0        | /
    |-------------------------------------------------------------------|

 */

namespace bioutils {
    class GeneralData {
    public:
        /* Auxiliary information */
        std::vector<std::string> trait_id;
        std::vector<std::string> feature_id;
        std::vector<std::string> sample_id;
        std::vector<char> ref;
        std::vector<char> alt;

        /* Number counts */
        uint32_t feature_num;
        uint32_t sample_num;
        uint32_t trait_num;

        /* Real data, normally do not use raw pointers */
        WVector<float> _gs;
        WVector<float> _ys;

        GeneralData();

        ~GeneralData();

        /**
         * Check the Integrity of this class, only for non-raw-data format
         * @return True/False
         */
        bool IntegrityCheck();

        /**
         * Read data from file
         * @param filename: Absolute or relative path to the input file
         * @param is_raw_data: Dose the file contain only numeric values? (FEATURE NOT SUPPORTED YET)
         * @return -1 on error, 0 otherwise
         */
        uint32_t ReadData(const std::string filename, const bool is_raw_data = false);

        /**
         * Write data to file
         * @param filename: Absolute or relative path to the input file
         * @param is_raw_data: Dose the file contain only numeric values? (FEATURE NOT SUPPORTED YET)
         * @return -1 on error, 0 otherwise
         */
        uint32_t SaveData(const std::string filename, const bool is_raw_data = false);

        /**
         * Remove one feature from data
         * @param idx: Feature index, later feature will be deleted
         * @return -1 on error, 0 otherwise
         */
        uint32_t RemoveFeature(uint32_t idx);

        /**
         * Print general data content
         */
        void Print();

        /**
         * Print summary for general data
         */
        void PrintSum();
    };
}

#endif //BIOSECURE_GENERAL_DATA_H
