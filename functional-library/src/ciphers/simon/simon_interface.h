#ifndef SIMON_INTERFACE_H
#define SIMON_INTERFACE_H

#include "../../interface/parameters.h"
#include <iostream>
#include <bitset>
#include "Simon_Speck_Ciphers/C/simon.h"



void simon128_set_key(bit128_t);

bit128_t simon128_encrypt_128(bit128_t plaintext);

bit128_t simon128_decrypt_128(bit128_t ciphertext);


#endif
