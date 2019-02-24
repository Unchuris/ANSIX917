#include "./BlockCipher.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

BinStr OFBencrypt(BinStr string, BinStr IV, BlockCipher cipher) {
	assert(string != NULL && IV != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0 &&
		IV->length == cipher->blockSize);
	BinStr msg = empty_BinStr(0);
	BinStr prev = copyStr(IV);
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr bin_str = subString(string, i, i + cipher->blockSize - 1);
		prev = set(prev, (*cipher->encrypt)(prev, cipher->roundKeys));
		bin_str = set(bin_str, XOR(prev, bin_str));
		msg = set(msg, append(msg, bin_str));
		destroy_BinStr(bin_str);
	}
	destroy_BinStr(prev);
	return msg;
}

BinStr OFBdecrypt(BinStr string, BinStr IV, BlockCipher cipher) {
	assert(string != NULL && IV != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0 &&
		IV->length == cipher->blockSize);
	BinStr msg = empty_BinStr(0);
	BinStr prev = copyStr(IV);
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr bin_str = subString(string, i, i + cipher->blockSize - 1);
		prev = set(prev, (*cipher->encrypt)(prev, cipher->roundKeys));
		bin_str = set(bin_str, XOR(prev, bin_str));
		msg = set(msg, append(msg, bin_str));
		destroy_BinStr(bin_str);
	}
	destroy_BinStr(prev);
	return msg;
}

BinStr CTRencrypt(BinStr string, BinStr nonce, BlockCipher cipher) {
	assert(string != NULL && nonce != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0 &&
		nonce->length == cipher->blockSize / 2);
	BinStr msg = empty_BinStr(0);
	int counter_int = 0;
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr counter = int_to_BinStr(counter_int);
		counter = set(counter, cut(counter, cipher->blockSize / 2));
		BinStr bin_str = append(nonce, counter);
		bin_str = (*cipher->encrypt)(bin_str, cipher->roundKeys);
		BinStr sub = subString(string, i, i + cipher->blockSize - 1);
		bin_str = set(bin_str, XOR(bin_str, sub));
		msg = set(msg, append(msg, bin_str));
		destroy_BinStr(bin_str);
		destroy_BinStr(sub);
		destroy_BinStr(counter);
	}
	return msg;
}

BinStr CTRdecrypt(BinStr string, BinStr nonce, BlockCipher cipher) {
	assert(string != NULL && nonce != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0 &&
		nonce->length == cipher->blockSize / 2);
	BinStr msg = empty_BinStr(0);
	int counter_int = 0;
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr counter = int_to_BinStr(counter_int);
		counter = set(counter, cut(counter, cipher->blockSize / 2));
		BinStr bin_str = append(nonce, counter);
		bin_str = (*cipher->encrypt)(bin_str, cipher->roundKeys);
		BinStr sub = subString(string, i, i + cipher->blockSize - 1);
		bin_str = set(bin_str, XOR(bin_str, sub));
		msg = set(msg, append(msg, bin_str));
		destroy_BinStr(bin_str);
		destroy_BinStr(sub);
		destroy_BinStr(counter);
	}
	return msg;
}

BinStr CBCencrypt(BinStr string, BinStr IV, BlockCipher cipher) {
	assert(string != NULL && IV != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0 &&
		IV->length == cipher->blockSize);
	BinStr cip = empty_BinStr(0);
	BinStr prev = copyStr(IV);
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr bin_str = subString(string, i, i + cipher->blockSize - 1);
		bin_str = set(bin_str, XOR(prev, bin_str));
		bin_str = set(bin_str, (*cipher->encrypt)(bin_str, cipher->roundKeys));
		destroy_BinStr(prev);
		prev = bin_str;
		cip = set(cip, append(cip, bin_str));
	}
	destroy_BinStr(prev);
	return cip;
}

BinStr CBCdecrypt(BinStr string, BinStr IV, BlockCipher cipher) {
	assert(string != NULL && IV != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0 &&
		IV->length == cipher->blockSize);
	BinStr msg = empty_BinStr(0);
	BinStr prev = copyStr(IV);
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr bin_str = subString(string, i, i + cipher->blockSize - 1);
		bin_str = set(bin_str, (*cipher->decrypt)(bin_str, cipher->roundKeys));
		bin_str = set(bin_str, XOR(bin_str, prev));
		msg = set(msg, append(msg, bin_str));
		destroy_BinStr(bin_str);
		prev = set(prev, subString(string, i, i + cipher->blockSize - 1));
	}
	destroy_BinStr(prev);
	return msg;
}

BinStr ECBencrypt(BinStr string, BlockCipher cipher) {
	assert(string != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0);
	BinStr msg = empty_BinStr(0);
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr bin_str = subString(string, i, i + cipher->blockSize - 1);
		bin_str = set(bin_str, (*cipher->encrypt)(bin_str, cipher->roundKeys));
		msg = set(msg, append(msg, bin_str));
		destroy_BinStr(bin_str);
	}
	return msg;
}

BinStr ECBdecrypt(BinStr string, BlockCipher cipher) {
	assert(string != NULL && cipher != NULL &&
		string->length % cipher->blockSize == 0);
	BinStr msg = empty_BinStr(0);
	for (int i = 0; i < string->length; i += cipher->blockSize) {
		BinStr bin_str = subString(string, i, i + cipher->blockSize - 1);
		bin_str = set(bin_str, (*cipher->decrypt)(bin_str, cipher->roundKeys));
		msg = set(msg, append(msg, bin_str));
		destroy_BinStr(bin_str);
	}
	return msg;
}

BinStr BlockEncrypt(BinStr msg, BinStr IV, BlockCipher cipher) {
	assert(msg != NULL && cipher != NULL);
	if (strcmp(cipher->encryptionMode, "ECB") == 0) {
		return ECBencrypt(msg, cipher);
	}
	else if (strcmp(cipher->encryptionMode, "CBC") == 0) {
		return CBCencrypt(msg, IV, cipher);
	}
	else if (strcmp(cipher->encryptionMode, "CTR") == 0) {
		return CTRencrypt(msg, IV, cipher);
	}
	else if (strcmp(cipher->encryptionMode, "OFB") == 0) {
		return OFBencrypt(msg, IV, cipher);
	}
	else {
		return NULL;
	}
}

BinStr BlockDecrypt(BinStr cip, BinStr IV, BlockCipher cipher) {
	assert(cip != NULL && cipher != NULL);
	if (strcmp(cipher->encryptionMode, "ECB") == 0) {
		return ECBdecrypt(cip, cipher);
	}
	else if (strcmp(cipher->encryptionMode, "CBC") == 0) {
		return CBCdecrypt(cip, IV, cipher);
	}
	else if (strcmp(cipher->encryptionMode, "CTR") == 0) {
		return CTRdecrypt(cip, IV, cipher);
	}
	else if (strcmp(cipher->encryptionMode, "OFB") == 0) {
		return OFBdecrypt(cip, IV, cipher);
	}
	else {
		return NULL;
	}
}