#ifndef SIMON_INTERFACE_CPP
#define SIMON_INTERFACE_CPP

#include "simon_interface.h"
#include "Simon_Speck_Ciphers/C/cipher_constants.h"

SimSpk_Cipher my_simon_cipher;
uint32_t result;
uint8_t my_IV[] = {0x32,0x14,0x76,0x58};
uint8_t my_counter[] = {0x2F,0x3D,0x5C,0x7B};

//TEST UTILITIES
uint8_t simon128_128_key[] = {0x00, 0x01, 0x02, 0x03,0x04, 0x05, 0x06,0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
uint8_t simon128_128_plain[] = {0x20, 0x74, 0x72, 0x61, 0x76, 0x65, 0x6c, 0x6c, 0x65, 0x72, 0x73, 0x20, 0x64, 0x65, 0x73, 0x63};
uint8_t simon128_128_cipher[] = {0xbc, 0x0b, 0x4e, 0xf8, 0x2a, 0x83, 0xaa, 0x65, 0x3f, 0xfe, 0x54, 0x1e, 0x1e, 0x1b, 0x68, 0x49};


void simon128_set_key(bit128_t key){
    
    Simon_Init(&my_simon_cipher, cfg_128_128, ECB, SECRET_KEY.value, my_IV, my_counter);

}

bit128_t simon128_encrypt_128(bit128_t plaintext){
    bit128_t ciphertext;
	uint8_t temp_ciphertext[16];
	Simon_Encrypt(my_simon_cipher, plaintext.value, temp_ciphertext);

    return bit128_t(temp_ciphertext);
}

bit128_t simon128_decrypt_128(bit128_t ciphertext){
	uint8_t temp_plaintext[16];
	Simon_Decrypt(my_simon_cipher, ciphertext.value, temp_plaintext);
	
    return bit128_t(temp_plaintext);
}


#endif
