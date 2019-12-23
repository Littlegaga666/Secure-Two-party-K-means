/**
 * \file    alg_or.cpp
 * \author  Jamie
 * \brief   Odds-Ratio Test
 * \details Input Table
 * |         | A | a |
 * |---------|---|---|
 * | case    | a | b |
 * | control | c | d |
 * OR = (ad)/(bc)
 */
#include "algorithm_handler.h"

/**
 * Setup tables for or
 * @param data_pt
 * @param working_vec
 * @param nsize_ptr
 * @param nvals_ptr
 */
void setup_or_input(GeneralData* data_pt, WVector<float>* working_vec, uint32_t* nsize_ptr, uint32_t* nvals_ptr){
    WVector<float> *gs = &(data_pt->_gs);
    WVector<float> *ys = &(data_pt->_ys);
    *nvals_ptr =  data_pt->feature_num;
    *nsize_ptr = 4;

    working_vec->SetMSize((*nvals_ptr), (*nsize_ptr));

    /* Local computation */
    float* cur_pt = working_vec->GetRawPtr();
    for(int i=0; i<*nvals_ptr; i++){
        for(int j=0; j<data_pt->sample_num; j++){
            if(gs->GetValue(i, j) == 3)
                continue;
            if(ys->GetValue(0, j) == 1){
                cur_pt[0] += (2-gs->GetValue(i, j));     // a
                cur_pt[1] += (gs->GetValue(i, j));       // b
            }else if(ys->GetValue(0, j) == 2){
                cur_pt[2] += (2-gs->GetValue(i, j));     // c
                cur_pt[3] += gs->GetValue(i, j);         // d
            }
        }
        cur_pt += *nsize_ptr;
    }
}

/**
 * Execute or circuit
 * @param handler
 * @param working_vec
 * @param nvals
 * @param out_vals
 */
void exec_or_circuit(ALGORITHMHandler* handler, WVector<float>* working_vec, uint32_t nvals, uint32_t **out_vals){
    assert(working_vec->getMSize() == nvals * 4);
    uint32_t sharing = handler->m_sharing;
    uint32_t bitlen = handler->m_bitlen;

    /* Default Decleration */
    ABYParty* party = new ABYParty(static_cast<e_role>(handler->m_role), handler->m_address, handler->m_port,
               handler->m_seclvl, handler->m_bitlen, handler->m_nthreads, static_cast<e_mt_gen_alg>(handler->m_mt_alg));
    std::vector<Sharing*>& sharings = party->GetSharings();
    BooleanCircuit* circ = (BooleanCircuit*) sharings[sharing]->GetCircuitBuildRoutine();

    share* s_server[4];
    share* s_client[4];
    share* s_sum[4];

    if(handler->m_role == SERVER)
        for(int i=0; i<4; i++){
            uint32_t * pt = (uint32_t *)(working_vec->GetColPtr(i));
            s_server[i] = circ->PutSIMDINGate(nvals, pt, bitlen, SERVER);
            s_client[i] = circ->PutDummySIMDINGate(nvals, bitlen);
        }
    else
        for(int i=0; i< 4; i++){
            uint32_t * pt = (uint32_t *)(working_vec->GetColPtr(i));
            s_server[i] = circ->PutDummySIMDINGate(nvals, bitlen);
            s_client[i] = circ->PutSIMDINGate(nvals, pt, bitlen, CLIENT);
        }

    for(int i=0; i<4; i++)
        s_sum[i] = circ->PutFPGate(s_client[i], s_server[i], ADD, bitlen, nvals, no_status);

    share* temp1 = circ->PutFPGate(s_sum[0], s_sum[3], MUL, bitlen, nvals, no_status);
	share* temp2 = circ->PutFPGate(temp1, s_sum[1], DIV, bitlen, nvals, no_status);
	share* temp3 = circ->PutFPGate(temp2, s_sum[2], DIV, bitlen, nvals, no_status);
	share* s_out = circ->PutOUTGate(temp3, ALL);

    party->ExecCircuit();

    /* output information r*/
    uint32_t out_bitlen, out_nvals;
    s_out->get_clear_value_vec(out_vals, &out_bitlen, &out_nvals);
    assert(out_bitlen==bitlen);
    assert(out_nvals==nvals);

    for(int i=0; i<4; i++){
        delete s_server[i];
        delete s_client[i];
        delete s_sum[i];
    }

    delete temp1;
	delete temp2;
	delete temp3;
	delete s_out;

    delete party;
}

/**
 * Perform or
 * @param handler
 * @param data_pt
 * @return string format result
 */
std::string test_or_circuit(ALGORITHMHandler* handler, GeneralData* data_pt) {

    uint32_t nsize;
	uint32_t nvals;
    uint32_t* out_ptr;
    WVector<float> working_vec;

    setup_or_input(data_pt, &working_vec, &nsize, &nvals); // set up those vectors

    // Execute circuit
    exec_or_circuit(handler, &working_vec, nvals, &out_ptr);

    std::string ret;
    for(int i=0; i<nvals; i++){
        float or_v = *(float *)&out_ptr[i];
        ret += (data_pt->feature_id.at(i) + "\t" + std::to_string(or_v) + "\n");
    }

    /* turn on for debugging */
    // data_pt->Print();
//     working_vec.Print();
    // printf("%s\n", ret.c_str());

    delete [] out_ptr;

    return ret;
}