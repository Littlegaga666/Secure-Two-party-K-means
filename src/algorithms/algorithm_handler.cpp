/**
 * \file    algorithm_handler.cpp
 * \author  Jamie
 * \brief   Algorithm Handler Implementation
 */
#include "algorithm_handler.h"

ALGORITHMHandler::ALGORITHMHandler(){
    m_bitlen = 32;
    m_neles = 31;
    m_secparam = 128;
    m_nthreads = 1;
	m_port = 7767;
	m_address = "127.0.0.1";
	m_test_op = -1;
	m_mt_alg = MT_OT;
	m_sharing = S_BOOL;
    m_seclvl = get_sec_lvl(m_secparam);
}

ALGORITHMHandler::~ALGORITHMHandler(){
    m_bitlen = 0;
    m_neles = 0;
    m_secparam = 0;
    m_nthreads = 0;
    m_port = 0;
    m_address = "";
    m_test_op = 0;
    m_mt_alg = 0;
    m_sharing = 0;
    m_seclvl = get_sec_lvl(256);
}

uint32_t ALGORITHMHandler::feature_alignment(GeneralData* data_pt){
    uint32_t server_len = 0;
    uint32_t client_len = 0;
    CSocket* socket = new CSocket(false);
    if(m_role == SERVER){
        server_len = data_pt->feature_num;
        socket->Bind(m_address, m_port);
        socket->Listen();
        auto sock = socket->Accept();
        sock->Receive(&client_len, sizeof(uint32_t));
        sock->Send(&server_len, sizeof(uint32_t));
        sock->Close();
    }else{
        client_len = data_pt->feature_num;
        while(!socket->Connect(m_address, m_port)) {
            sleep(1);
            socket->Close();
        }
        socket->Send(&client_len, sizeof(uint32_t));
        socket->Receive(&server_len, sizeof(uint32_t));
    }
    socket->Close();
    delete socket;

    return test_phasing_circuit(static_cast<e_role>(m_role), m_address, m_port, m_seclvl, server_len, client_len,
                        m_bitlen, 1.2, m_nthreads, static_cast<e_mt_gen_alg>(m_mt_alg),
                        static_cast<e_sharing>(m_sharing), -1,
                        0, 3, data_pt);
}

std::string ALGORITHMHandler::snp_missing_rate_filter(GeneralData* data){
    return test_mr_circuit(this, data);
}

std::string ALGORITHMHandler::minor_allele_frequency_filter(GeneralData* data){
    return test_maf_circuit(this, data);
}

std::string ALGORITHMHandler::hardy_weinberg_equilibrium(GeneralData* data){
    return test_hwe_circuit(this, data);
}

std::string ALGORITHMHandler::odds_ratio(GeneralData* data_pt){
    return test_or_circuit(this, data_pt);
}

std::string ALGORITHMHandler::chi_squared_test(GeneralData* data_pt){
    return test_chi_circuit(this, data_pt);
}

std::string ALGORITHMHandler::cochran_armitage_trend_test(GeneralData* data_pt){
    return test_catt_circuit(this, data_pt);
}

std::string ALGORITHMHandler::cochran_mantel_haenszel_test(GeneralData* data_pt){
    return test_cmh_circuit(this, data_pt);
}

uint32_t ALGORITHMHandler::linear_regression(GeneralData* data_pt){
    return test_lr_circuit(this, data_pt);
}


//uint32_t ALGORITHMHandler::pca(GeneralData* data_pt, uint32_t result_dim){
//    uint32_t output = test_pca_circuit(m_role, m_address, m_port, m_seclvl, m_bitlen, m_nthreads, m_mt_alg, m_sharing, data_pt, data_pt->feature_num, result_dim);
//    return 0;
//}
//
//uint32_t ALGORITHMHandler::pearson(GeneralData* data_pt){
//    uint32_t output = test_pearson_circuit(m_role, m_address, m_port, m_seclvl, m_bitlen, m_nthreads, m_mt_alg, m_sharing, data_pt);
//    return 0;
//}
//
//uint32_t ALGORITHMHandler::pearson_sign(GeneralData* data_pt){
//    uint32_t output = test_pearson_circuit_sign(m_role, m_address, m_port, m_seclvl, m_bitlen, m_nthreads, m_mt_alg, m_sharing, data_pt);
//    return 0;
//}
//
//void ALGORITHMHandler::logistic_regression(GeneralData* data_pt){
//    test_logistic_regression_circuit(m_role, m_address, m_port, m_seclvl, m_bitlen, m_nthreads,m_mt_alg, m_sharing, data_pt);
//}