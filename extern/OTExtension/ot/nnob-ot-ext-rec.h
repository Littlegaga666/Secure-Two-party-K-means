/*
 * nnob-ot-ext-rec.h
 *
 *  Created on: Mar 23, 2015
 *      Author: mzohner
 *
 * Malicious OT extension routine from NNOB12
 */

#ifndef NNOB_OT_EXT_REC_H_
#define NNOB_OT_EXT_REC_H_

#include "ot-ext-rec.h"



typedef struct nnob_rcv_check_ctx {
	uint64_t otid;
	uint64_t numblocks;
	uint8_t* T0;
} nnob_rcv_check_t;

class NNOBOTExtRec : public OTExtRec {

public:
	NNOBOTExtRec(crypto* crypt, RcvThread* rcvthread, SndThread* sndthread, bool dobaseots=true, uint64_t num_ot_blocks=4096, bool verify_ot=true, bool use_fixed_key_aes_hashing=false)
		: OTExtRec(num_ot_blocks, verify_ot, use_fixed_key_aes_hashing) {
		uint32_t nbaseots = ceil_divide(crypt->get_seclvl().symbits * 8, 3);
		InitRec(crypt, rcvthread, sndthread, nbaseots);
		m_nChecks = nbaseots/2;
		m_bDoBaseOTs=dobaseots;
	}
	;


	~NNOBOTExtRec() {
	}
	;

	BOOL receiver_routine(uint32_t threadid, uint64_t numOTs);
	void ComputeBaseOTs(field_type ftype);

private:
	nnob_rcv_check_t EnqueueSeed(uint8_t* T0, uint64_t otid, uint64_t numblocks);
	void ComputeOWF(std::queue<nnob_rcv_check_t>* check_buf_q, channel* check_chan);
	void ReceiveAndFillMatrix(uint64_t** rndmat, channel* mat_chan);
	bool m_bDoBaseOTs;
};

#endif /* NNOB_OT_EXT_REC_H_ */
