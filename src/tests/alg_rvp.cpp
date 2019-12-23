/**
 * @author: Jamie
 * @brief: The Random Value Protocol
 */
#include <utils/utils.h>
#include <mpir.h>

enum ROLE{
    SERVER,
    CLIENT
};

void client_routine(mpz_t Qb[], uint32_t nbits);
void server_routine(mpz_t Qa[], uint32_t nbits);

int main(int argc, char** argv){
	ROLE role;
	if(argv[1][0] == 48)
		role = SERVER;
	else
		role = CLIENT;

	gmp_randstate_t rand;
	gmp_randinit_default(rand);
	uint32_t nbits = 8;

	mpz_t Q;
    mpz_t qa[nbits];
    mpz_t qb[nbits];

    mpz_init(Q);
    mpz_urandomb(Q, rand, nbits);


	for(int i=0; i<nbits; i++) {
		mpz_init(qa[i]);
        mpz_init(qb[i]);
        mpz_t tmp_9;
        mpz_init(tmp_9);
        mpz_set_ui(tmp_9, 143);
        mpz_urandomm(qa[i], rand, tmp_9);
        mpz_ui_sub(qb[i], mpz_tstbit(Q,i), qa[i]);
        mpz_mod_ui(qb[i], qb[i], 16);
	}

	std::cout << "Q = " << Q << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "i\tQ(bit)\tqa\tqb" << std::endl;
	for(int i=0 ;i<nbits; i++){
	    std::cout << i << "\t" <<  mpz_tstbit(Q,i) << "\t" << qa[i] << "\t" << qb[i] << std::endl;
	}
    std::cout << "-----------------------" << std::endl;

    if(role == SERVER) {
	    server_routine(qa, nbits);
    }
    else {
	    client_routine(qb, nbits);
    }
}


