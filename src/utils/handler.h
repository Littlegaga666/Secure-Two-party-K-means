/**
 * \file    handler.h
 * \author  Jamie Cui
 * \brief   Abstract Parent for MPC-handlers
 */
#ifndef BIOSECURE_HANDLER_H
#define BIOSECURE_HANDLER_H

#include "utils.h"

/**
 * This is an abstract class for handling different functionalities.
 */
namespace bioutils {
    class Handler {
    public:
        uint32_t m_role;        // role (0---server; 1---client)
        uint32_t m_mt_alg;      // algorithms used for multiplication triples generation; MT_OT = 0; MT_PAILLIER = 1; MT_DKG = 2
        uint32_t m_sharing;     // sharing types: S_BOOL(GMW protocol)/S_ARITH(GMW protocol)/S_YAO(Yao protocol);  S_BOOL = 0; S_YAO = 1; S_ARITH = 2;
        uint16_t m_port;        // port for mpc
        uint32_t m_bitlen;      // circuit bitlen
        uint32_t m_neles;       // Do ... m_neles ... times
        uint32_t m_secparam;    // security parameters
        uint32_t m_nthreads;    // threads for parallization
        std::string m_address;  // address for mpc

        /* functions */
        Handler();              // constructor
        ~Handler();             // destructor
    };
}

#endif