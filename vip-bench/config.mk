CC  = gcc
CXX = g++
LD  = g++

OPT_CFLAGS = -O2 -g -std=c++11 -Wno-comment -maes

NA_CFLAGS =
NA_LIBS =
NA_SIM =
NA_DIFF = diff

DO_CFLAGS =
DO_LIBS =
DO_SIM =
DO_DIFF = diff

ENC_CFLAGS = 
ENC_LIBS = ~/Privacy/vip-bench/se.cpp ~/Privacy/functional-library/src/interface/interface.cpp ~/Privacy/functional-library/src/datatypes/enc_impl.cpp ~/Privacy/functional-library/src/interface/parameters.cpp ~/Privacy/functional-library/src/rng/rng_interface.cpp ~/Privacy/functional-library/src/rng/mersenne-twister-1/mersenne-twister.cpp ~/Privacy/functional-library/src/ciphers/simon/Simon_Speck_Ciphers/C/simon.c ~/Privacy/functional-library/src/ciphers/aes/tinyCrypt_without_Git/lib/utils.o ~/Privacy/functional-library/src/ciphers/aes/tinyCrypt_without_Git/lib/aes_encrypt.o ~/Privacy/functional-library/src/ciphers/aes/tinyCrypt_without_Git/lib/aes_decrypt.o ~/Privacy/functional-library/src/ciphers/simon/simon_interface.cpp ~/Privacy/functional-library/src/ciphers/aes/aes_interface.cpp ~/Privacy/functional-library/src/ciphers/qarma/qarma_interface.cpp ~/Privacy/functional-library/src/ciphers/xor/xor_interface.cpp 

ENC_SIM =
ENC_DIFF = diff

