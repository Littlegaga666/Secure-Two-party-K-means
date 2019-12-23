/**
 * \file    handler.cpp
 * \author  Jamie Cui
 * \brief   Abstract Parent for MPC-handlers
 */
#include "handler.h"

namespace bioutils {

    Handler::Handler() {
        m_bitlen = 32;
        m_neles = 31;
        m_secparam = 128;
        m_nthreads = 1;
        m_port = 7766;
        m_address = "127.0.0.1";
        m_mt_alg = 0;
        m_sharing = 0;
    }

    Handler::~Handler() {
        // reset to default
        m_bitlen = 32;
        m_neles = 31;
        m_secparam = 128;
        m_nthreads = 1;
        m_port = 7766;
        m_address = "127.0.0.1";
        m_mt_alg = 0;
        m_sharing = 0;
    }
}