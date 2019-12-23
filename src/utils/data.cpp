/**
 * \file    data.cpp
 * \author  Jamie Cui
 * \brief   Data Implementation
 */
#include "data.h"

namespace bioutils {
    /* TODO: add raw_data format support */
    uint32_t GeneralData::ReadData(const std::string filename, const bool is_raw_data) {
        std::ifstream in;
        std::string line;
        std::vector<std::string> tmp_buffer;

        /* open file */
        in.open(filename);
        if (!in.is_open()) {
            std::cerr << "Error opening file: " + filename << std::endl;
            return -1;
        }

        /* read first line and get total sample number, and get total feature number */
        std::getline(in, line);
        split_string(line, tmp_buffer, "\t");
        sample_num = tmp_buffer.size() - 3;

        for (int i = 0; i < sample_num; i++)
            sample_id.push_back(tmp_buffer.at(i + 3));
        tmp_buffer.clear();

        while (std::getline(in, line) && !line.empty()) {
            if (line.at(0) == '#')
                trait_num++;
            else
                feature_num++;
        }
        in.clear();
        in.seekg(0, std::ios::beg); // back to the first line
        std::getline(in, line); // skip the first, let in point to second line

        _gs.SetMSize(feature_num, sample_num);
        _ys.SetMSize(trait_num, sample_num);

        for (int i = 0; i < trait_num; i++) {
            std::getline(in, line);
            split_string(line, tmp_buffer, "\t");
            trait_id.push_back(tmp_buffer.at(0));
            tmp_buffer.erase(tmp_buffer.begin());
            for (int j = 0; j < tmp_buffer.size(); j++) {
                try {
                    _ys.GetRowPtr(i)[j] = std::stof(tmp_buffer.at(j));
                } catch (const std::exception &e) {
                    std::cerr << "ReadData Error in ys, " << e.what() << std::endl;
                    return -1;
                }
            }
            tmp_buffer.clear();
        }

        for (int i = 0; i < feature_num; i++) {
            std::getline(in, line);
            split_string(line, tmp_buffer, "\t");

            feature_id.push_back(tmp_buffer.at(0));
            ref.push_back(tmp_buffer.at(1).at(0));
            alt.push_back(tmp_buffer.at(2).at(0));
            for (int j = 0; j < sample_num; j++) {
                try {
                    _gs.GetRowPtr(i)[j] = std::stof(tmp_buffer.at(j + 3));
                } catch (const std::exception &e) {
                    std::cerr << "ReadData Error in gs, " << e.what() << std::endl;
                    return -1;
                }
            }

            tmp_buffer.clear();
        }

//    std::cout << "Reading File has " << sample_num << " samples, " << feature_num << " features, " << trait_num
//        << " traits." << std::endl;
//    _ys.Print();
//    _gs.Print();

        in.close();
        return IntegrityCheck();
    }

    /* TODO: add raw_data format support */
    uint32_t GeneralData::SaveData(const std::string filename, const bool raw_data_format) {
        std::ofstream out;
        std::string line;

        out.open(filename);
        if (!out.is_open()) {
            std::cerr << "Error opening file: " + filename << std::endl;
            return -1;
        }

        /* 1st line */
        out << "rs_ID\tref\talt\t";
        for (int i = 0; i < sample_num; i++) {
            out << sample_id.at(i) << "\t";
        }
        out << std::endl;

        /* 2nd line */
        for (int i = 0; i < trait_id.size(); i++) {
            out << trait_id.at(i) << "\t";
            for (int j = 0; j < sample_num; j++)
                out << _ys.GetRowPtr(i)[j] << "\t";
            out << std::endl;
        }

        /* write everything else */
        for (int i = 0; i < feature_num; i++) {
            out << feature_id.at(i) << "\t";
            out << ref.at(i) << "\t";
            out << alt.at(i) << "\t";
            for (int j = 0; j < sample_num; j++)
                out << _gs.GetRowPtr(i)[j] << "\t";
            out << std::endl;
        }
        out.close();

        return 0;
    }

    bool GeneralData::IntegrityCheck() {
        if (_gs.getMCols() != sample_num) {
            std::cerr << "Error: Integrity check failed in _gs.getMCols(), <" << _gs.getMCols() << "!=" << sample_num
                      << ">" << std::endl;
            return false;
        }
        if (_gs.getMRows() != feature_num) {
            std::cerr << "Error: Integrity check failed in _gs.getMRows(), <" << _gs.getMRows() << "!=" << feature_num
                      << ">" << std::endl;
            return false;
        }
        if (_ys.getMCols() != sample_num) {
            std::cerr << "Error: Integrity check failed in _gs.getMRows(), <" << _ys.getMCols() << "!=" << sample_num
                      << ">" << std::endl;
            return false;
        }
        if (trait_id.size() != trait_num) {
            std::cerr << "Error: Integrity check failed in trait_id, <" << trait_id.size() << "!=" << trait_num << ">"
                      << std::endl;
            return false;
        }
        if (feature_id.size() != feature_num) {
            std::cerr << "Error: Integrity check failed in feature_id, <" << feature_id.size() << "!=" << feature_num
                      << ">" << std::endl;
            return false;
        }
        if (sample_id.size() != sample_num) {
            std::cerr << "Error: Integrity check failed in sample_id, <" << sample_id.size() << "!=" << sample_num
                      << ">" << std::endl;
            return false;
        }
        if (ref.size() != feature_num) {
            std::cerr << "Error: Integrity check failed in ref, <" << ref.size() << "!=" << feature_num << ">"
                      << std::endl;
            return false;
        }
        if (alt.size() != feature_num) {
            std::cerr << "Error: Integrity check failed in alt, <" << alt.size() << "!=" << feature_num << ">"
                      << std::endl;
            return false;
        }
        return true;
    }

    uint32_t GeneralData::RemoveFeature(uint32_t idx) {
        assert(feature_num > 0);
        assert(idx < feature_num);

        _gs.RemoveRow(idx);
        feature_id.erase(feature_id.begin() + idx);
        ref.erase(ref.begin() + idx);
        alt.erase(alt.begin() + idx);
        feature_num--;
        return 0;
    }

    GeneralData::GeneralData() {
        feature_num = 0;
        sample_num = 0;
        trait_num = 0;
    }

    GeneralData::~GeneralData() {
        ref.clear();
        alt.clear();
        sample_id.clear();
        feature_id.clear();
        trait_id.clear();
        sample_num = 0;
        feature_num = 0;
        trait_num = 0;
    }

    void GeneralData::Print() {
        PrintSum();
        _ys.Print();
        _gs.Print();
    }

    void GeneralData::PrintSum() {
        int y = 0;
        for (int i = 0; i < sample_num; i++)
            if (_ys.GetValue(0, i) == 1)
                y++;
        printf("trait num             : %d\n", trait_num);
        printf("sample num (case)     : %d\n", y);
        printf("sample num (control)  : %d\n", sample_num - y);
        printf("feature num           : %d\n", feature_num);
    }
}