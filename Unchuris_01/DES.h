#ifndef DES_H
#define DES_H

#define DES_KEY_SIZE 64
#define DES_BLOCK_SIZE 64

#include "./BinStr.h"
#include "./BlockCipher.h"

BlockCipher DES_initialize(BinStr key, char* mode);

void DES_destroy(BlockCipher DES);

#endif