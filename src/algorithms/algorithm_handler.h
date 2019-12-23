/**
 * \file    algorithm_handler.h
 * \author  Jamie
 * \brief   Algorithm Handler Definitions
 */
#ifndef BIOSECURE_ALGORITHM_HANDLER_H
#define BIOSECURE_ALGORITHM_HANDLER_H

#include <ENCRYPTO_utils/crypto/crypto.h>
#include <ENCRYPTO_utils/parse_options.h>
#include <ENCRYPTO_utils/socket.h>
#include "abycore/circuit/booleancircuits.h"
#include "abycore/circuit/circuit.h"
#include "abycore/aby/abyparty.h"
#include "abycore/sharing/sharing.h"

#include <utils/utils.h>
#include <utils/data.h>
#include <utils/handler.h>

using namespace bioutils;

class ALGORITHMHandler : public Handler{
public:
	ALGORITHMHandler();
	~ALGORITHMHandler();

	/**
	 * Feature alignment
	 * @param data_pt
	 * @return -1 on error, 0 otherwise
	 */
    uint32_t feature_alignment(GeneralData* data_pt);

    /**
     * Calculates odds ratio on General Data
     * @param data_pt
     * @return string format result
     */
    std::string odds_ratio(GeneralData* data_pt);

    /**
     * Calculates chi-squared test on General Data
     * @param data_pt
     * @return string format result
     */
    std::string chi_squared_test(GeneralData* data_pt);

    /**
     * Calculates Cochran Armitage Trend Test on General Data
     * @param data_pt
     * @return string format result
     */
    std::string cochran_armitage_trend_test(GeneralData* data_pt);

    /**
     * Applying SNP missing rate filter on General Data
     * @warning: This function is not safe
     * @param data
     * @return string format result
     */
    std::string snp_missing_rate_filter(GeneralData* data);

    /**
     * Applying Minor Allele Frequency on General Data
     * @warning: This function is not safe
     * @param data
     * @return string format result
     */
    std::string minor_allele_frequency_filter(GeneralData* data);

    /**
     * Applying Hardy Weinberg Equilibrium test on General Data
     * @param data
     * @return string format result
     */
    std::string hardy_weinberg_equilibrium(GeneralData* data);

    /**
     * Applying Cochran Mantel Haenszel test on General Data
     * @param data_pt
     * @return string format result
     */
    std::string cochran_mantel_haenszel_test(GeneralData* data_pt);

    /**
     * Applying Linear Regression on General Data
     * @param data_pt
     * @return -1 on error, 0 otherwise
     */
    uint32_t linear_regression(GeneralData* data_pt);

//    uint32_t pca(GeneralData* data_pt, uint32_t result_dim);
//    uint32_t pearson_sign(GeneralData* data);
//    uint32_t pearson(GeneralData* data);
//    uint32_t target_missing_rate(GeneralData* data);
//    uint32_t heheterozygosity_rate(GeneralData* data);

    seclvl m_seclvl;
private:
    uint32_t m_test_op;
};

uint32_t test_phasing_circuit(e_role role, const std::string& address, uint16_t port, seclvl seclvl,
    uint32_t server_neles, uint32_t client_neles, uint32_t bitlen, double epsilon, uint32_t nthreads,
    e_mt_gen_alg mt_alg, e_sharing sharing, int ext_stash_size, uint32_t maxbinsize,
    uint32_t nhashfuns, GeneralData* data_pt);

std::string test_mr_circuit(ALGORITHMHandler *handler, GeneralData* data_pt, float SNP_MR_UPPER_BOUND = 0.2);
std::string test_maf_circuit(ALGORITHMHandler *handler, GeneralData* data_pt, float MAF_LOWER_BOUND = 0.02);
std::string test_hwe_circuit(ALGORITHMHandler *handler, GeneralData* data_pt);

std::string test_or_circuit(ALGORITHMHandler *handler, GeneralData* data_pt);
std::string test_chi_circuit(ALGORITHMHandler *handler, GeneralData* data_pt);
std::string test_catt_circuit(ALGORITHMHandler *handler, GeneralData* data_pt);

std::string test_cmh_circuit(ALGORITHMHandler *handler, GeneralData* data_pt);
uint32_t test_lr_circuit(ALGORITHMHandler* handler, GeneralData* data_pt);

//uint32_t test_pearson_circuit(e_role role, const std::string& address, uint16_t port, seclvl seclvl, uint32_t bitlen,
//    uint32_t nthreads, e_mt_gen_alg mt_alg, e_sharing sharing, GeneralData* data_pt);
//uint32_t test_pearson_circuit_sign(e_role role, const std::string& address, uint16_t port, seclvl seclvl,
//    uint32_t bitlen, uint32_t nthreads,	e_mt_gen_alg mt_alg, e_sharing sharing, GeneralData* data_pt);
//uint32_t test_pca_circuit(e_role role, const std::string& address, uint16_t port, seclvl seclvl, uint32_t bitlen,
//    uint32_t nthreads, e_mt_gen_alg mt_alg, e_sharing sharing, GeneralData* data_pt, uint32_t dim, uint32_t result_dim);
//void test_logistic_regression_circuit(e_role role, const std::string& address, uint16_t port, seclvl seclvl,
//    uint32_t bitlen, uint32_t nthreads, e_mt_gen_alg mt_alg, e_sharing sharing, GeneralData* data_pt);

#endif
