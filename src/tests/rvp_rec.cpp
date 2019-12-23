#include <utils/utils.h>
#include <gmp.h>
#include <random>
#include <algorithm>
#include <ENCRYPTO_utils/socket.h>
#include "../../extern/libpaillier-0.8/paillier.h" // TODO: fix cmake

void param_rec(std::unique_ptr<CSocket> &sock, paillier_pubkey_t* pub){
    char* buf;
    uint32_t buf_len;

    sock->Receive(&buf_len, sizeof(uint32_t));
    buf = new char[buf_len];
    sock->Receive(buf, buf_len);

    mpz_import(pub->n, buf_len, 1, 1, 0, 0, buf);
    mpz_add_ui(pub->n_plusone, pub->n, 1);
    mpz_mul(pub->n_squared, pub->n, pub->n);
    sock->Receive(&(pub->bits), sizeof(int));
    std::cout << ">> paillier setup done" << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "bits\t: " << pub->bits << std::endl;
    std::cout << "n\t: " << pub->n << std::endl;
    std::cout << "n+1\t: " << pub->n_plusone << std::endl;
    std::cout << "n^2\t: " << pub->n_squared << std::endl;
    std::cout << "-----------------------" << std::endl;

    delete[] buf;
}

void cipher_snd(std::unique_ptr<CSocket> &sock, paillier_ciphertext_t* cipher){
    size_t buf_len = 0;
    char* buf = (char*)mpz_export(NULL, &buf_len, 1, 1, 0, 0, cipher->c);
    sock->Send(&buf_len, sizeof(uint32_t));
    sock->Send(buf, buf_len);
    free(buf);
}

void cipher_rec(std::unique_ptr<CSocket> &sock, paillier_ciphertext_t* &cipher){
    char* buf;
    uint32_t buf_len;
    sock->Receive(&buf_len, sizeof(uint32_t));
    buf = new char[buf_len];
    sock->Receive(buf, buf_len);
    mpz_import(cipher->c, buf_len, 1, 1, 0, 0, buf);
    delete[] buf;
}

std::vector<uint32_t> generate_mapping(uint32_t range){
	std::vector<uint32_t> ret;
	for(int i=0; i< range; i++)
		ret.push_back(i);
	shuffle(ret.begin(), ret.end(), std::default_random_engine(1));
	return ret;
}

void server_routine(mpz_t qa[], uint32_t nbits){
    std::unique_ptr<paillier_pubkey_t> pub(new paillier_pubkey_t());

    paillier_ciphertext_t* cipher[nbits];
	paillier_ciphertext_t* mask_cipher[nbits];
	paillier_ciphertext_t* tmp_cipher[nbits];
	paillier_plaintext_t* mask[nbits];
	gmp_randstate_t rand;
	gmp_randinit_default(rand);
	paillier_get_rand_t get_rand = paillier_get_rand_devurandom;
    std::cout << ">> Listening on 127.0.0.1" << std::endl;

    /* connect with client */
    CSocket* socket = new CSocket(false);
    socket->Bind("127.0.0.1", 8888);
    socket->Listen();
    auto sock = socket->Accept();

    /* Receiving pub key and parameters for paillier */
    param_rec(sock, pub.get());

    /* Receiving encrypted messages from client */
    for(int i=0; i<nbits; i++){
        cipher[i] = new paillier_ciphertext_t();
        cipher_rec(sock, cipher[i]);
    }
    std::cout << ">> Rec Enc(qb)" << std::endl;

    for(int i=0; i<nbits; i++){
        cipher_snd(sock, cipher[i]);
    }
    std::cout << ">> Snd Enc(qb), waiting for client..." << std::endl;

//    /* generate mapping */
//    std::vector<uint32_t > map = generate_mapping(nbits);
//    std::cout << ">> Shuffled map generated: ";
//    for(int i=0; i<nbits; i++)
//        std::cout << map[i] << " ";
//    std::cout << std::endl;
//
//    /* mask with Z and shuffle */
//	for(int i=0; i<nbits; i++) {
//		tmp_cipher[i] = new paillier_ciphertext_t();
//		mask[i] = new paillier_plaintext_t();
//        mask_cipher[i] = new paillier_ciphertext_t();
//        mpz_urandomm(mask[i]->m, rand, pub->n);
//		std::cout << ">> Z" << i << "\t: " << mask[i]->m << std::endl;
////		paillier_enc(mask_cipher[i], pub, mask[i], get_rand);
////		paillier_mul(pub, tmp_cipher[i], mask_cipher[i], cipher[map[i]]);
////        paillier_mul(pub, tmp_cipher[i], mask_cipher[i], cipher[i]);
//	}


//
//	for(int i=0; i<nbits; i++) {
//		cipher_rec(sock, cipher[i]);
//	}
//    std::cout << ">> Rec Enc(S')" << std::endl;
//
//	/* get Enc(qa+Z) */
//    for(int i=0; i<nbits; i++) {
//        mpz_neg(mask[i]->m, mask[i]->m);
////        mpz_add(mask[i]->m, mask[i]->m, qa[map[i]]);
//        mpz_add(mask[i]->m, mask[i]->m, qa[i]);
//        mpz_mod(mask[i]->m, mask[i]->m, pub->n);
//        std::cout << ">> qa-Z\t: " << mask[i]->m << std::endl;
//
//    }
//    for(int i=0; i<nbits; i++) {
//        if(i>0){
//            mpz_mul(mask[i]->m, mask[i]->m, mask[i-1]->m);
//            mpz_mod(mask[i]->m, mask[i]->m, pub->n);
//        }
//        std::cout << ">> S\t: " << mask[i]->m << std::endl;
//        mask_cipher[i] = paillier_enc(NULL, pub, mask[i], get_rand);
//        paillier_mul(pub, tmp_cipher[i], mask_cipher[i], cipher[i]);
//    }
//
//
//    for(int i=0; i<nbits; i++){
//        cipher_snd(sock, tmp_cipher[i]);
//    }

//	paillier_ciphertext_t* delta = cipher[0];
//	for(int i=1; i<nbits; i++) {
//		if(i==0){
//
//		}
//		paillier_mul(pub, delta, delta, cipher[i]);
//	}
//
//	cipher_snd(sock, delta);

	sock->Close();
    socket->Close();
}