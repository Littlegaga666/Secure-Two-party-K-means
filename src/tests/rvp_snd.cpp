#include <utils/utils.h>
#include <gmp.h>
#include <ENCRYPTO_utils/socket.h>
#include "../../extern/libpaillier-0.8/paillier.h" // TODO: fix cmake

void param_snd(CSocket* socket, paillier_pubkey_t* &pub, paillier_prvkey_t* &prv, paillier_get_rand_t get_rand){
    char* buf;
    size_t buf_len = 0;
    int modulubits = 8;

    paillier_keygen(modulubits, &pub, &prv, get_rand);
    std::cout << ">> paillier setup done" << std::endl;

    /* sending pub key to other party */
    buf = (char*)mpz_export(NULL, &buf_len, 1, 1, 0, 0, pub->n);
    socket->Send(&buf_len, sizeof(uint32_t));
    socket->Send(buf, buf_len);
    socket->Send(&(pub->bits), sizeof(int));
    std::cout << "-----------------------" << std::endl;
    std::cout << "bits\t: " << pub->bits << std::endl;
    std::cout << "n\t: " << pub->n << std::endl;
    std::cout << "n+1\t: " << pub->n_plusone << std::endl;
    std::cout << "n^2\t: " << pub->n_squared << std::endl;
    std::cout << "lambda\t: " << prv->lambda << std::endl;
    std::cout << "x\t: " << prv->x << std::endl;
    std::cout << "-----------------------" << std::endl;

    delete buf;
}

void cipher_snd(CSocket* socket, paillier_ciphertext_t* cipher){
    size_t buf_len = 0;
    char* buf = (char*)mpz_export(NULL, &buf_len, 1, 1, 0, 0, cipher->c);
    socket->Send(&buf_len, sizeof(uint32_t));
    socket->Send(buf, buf_len);
    free(buf);
}

void cipher_rec(CSocket* socket, paillier_ciphertext_t* &cipher){
    char* buf;
    uint32_t buf_len;
    socket->Receive(&buf_len, sizeof(uint32_t));
    buf = new char[buf_len];
    socket->Receive(buf, buf_len);
    mpz_import(cipher->c, buf_len, 1, 1, 0, 0, buf);
    delete[] buf;
}

void client_routine(mpz_t qb[], uint32_t nbits){
    paillier_pubkey_t* pub = new paillier_pubkey_t();
    paillier_prvkey_t* prv = new paillier_prvkey_t();
    paillier_ciphertext_t* cipher[nbits];
    paillier_plaintext_t* plain[nbits];
    paillier_get_rand_t get_rand = paillier_get_rand_devurandom;

    /* connect with server */
    CSocket* socket = new CSocket(false);
    while(!socket->Connect("127.0.0.1", 8888))
        socket->Close();
    std::cout << ">> connected" << std::endl;

    /* paillier key setup */
    param_snd(socket, pub, prv, get_rand);

    /* Bob encrypts qb */
    for(int i=0; i<nbits; i++){
        plain[i] = new paillier_plaintext_t();
        cipher[i] = new paillier_ciphertext_t();
        mpz_set(plain[i]->m, qb[i]);
        paillier_enc(cipher[i], pub, plain[i], get_rand);
    }

    /* sending ciphers to server */
    for(int i=0; i<nbits; i++) {
        cipher_snd(socket, cipher[i]);
        std::cout << "Enc(" << plain[i]->m << ")" << std::endl;
    }
    std::cout << ">> Snd Enc(qb), waiting for server..." << std::endl;

    std::cout << "-----------------------" << std::endl;
    std::cout << "bits\t: " << pub->bits << std::endl;
    std::cout << "n\t: " << pub->n << std::endl;
    std::cout << "n+1\t: " << pub->n_plusone << std::endl;
    std::cout << "n^2\t: " << pub->n_squared << std::endl;
    std::cout << "lambda\t: " << prv->lambda << std::endl;
    std::cout << "x\t: " << prv->x << std::endl;
    std::cout << "-----------------------" << std::endl;
    for(int i=0; i<nbits; i++) {
	    cipher_rec(socket, cipher[i]);
        paillier_dec(plain[i], pub, prv, cipher[i]);
        std::cout << "Enc(" << plain[i]->m << ")" << std::endl;
    }
    std::cout << ">> Rec Enc(qb+Z)" << std::endl;
//	for(int i=0; i<nbits; i++) {
//	    plain[i] = paillier_dec(NULL, pub, prv, cipher[i]);
//	    std::cout << ">> qb+Z\t: " << plain[i]->m << std::endl;
////	    if(i>0){
////		    mpz_mul(plain[i]->m, plain[i]->m, plain[i-1]->m);
////		    mpz_mod(plain[i]->m, plain[i]->m, pub->n);
////	    }
//    }
//
//	for(int i=0; i<nbits; i++) {
//	    std::cout << ">> S'\t: " << plain[i]->m << std::endl;
//	}
//
//    /* re-encrypts final message */
//	for(int i=0; i<nbits; i++){
//		cipher[i] = paillier_enc(NULL, pub, plain[i], get_rand);
//	}
//
//	for(int i=0; i<nbits; i++) {
//		cipher_snd(socket, cipher[i]);
//	}
//    std::cout << ">> Snd Enc(S'), waiting for server..." << std::endl;
//
//    for(int i=0; i<nbits; i++) {
//        cipher_rec(socket, cipher[i]);
//        plain[i] = paillier_dec(NULL, pub, prv, cipher[i]);
//        std::cout << plain[i]->m << std::endl;
//    }
//    std::cout << ">> Rec final result" << std::endl;



    socket->Close();
}
