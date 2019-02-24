#include "./BinStr.h"

struct blockcipher {
	BinStr key;
	BinStr *roundKeys;
	int blockSize;
	char *encryptionMode;
	BinStr(*encrypt)(BinStr, BinStr *);
	BinStr(*decrypt)(BinStr, BinStr *);
};

typedef struct blockcipher *BlockCipher;

BinStr BlockEncrypt(BinStr string, BinStr IV, BlockCipher cipher);

BinStr BlockDecrypt(BinStr string, BinStr IV, BlockCipher cipher);

