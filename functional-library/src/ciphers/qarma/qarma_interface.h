#ifndef _QARMA_INTERFACE_H
#define _QARMA_INTERFACE_H

#include "../../interface/parameters.h"
#include <iostream>
#include <bitset>


void qarma64_set_key(bit128_t key);


bit128_t qarma64_encrypt_128(bit128_t plaintext);

bit128_t qarma64_decrypt_128(bit128_t ciphertext);

#endif