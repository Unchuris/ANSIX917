#ifndef BINSTR_H
#define BINSTR_H

#include <stdbool.h>

extern const int BITS_PER_WORD;
extern const int BITS_PER_BYTE;
extern const int MAX_BYTE;

struct binstr {
	bool *bits;
	unsigned int length;
};

typedef struct binstr *BinStr;

BinStr str_to_BinStr(char *bits, unsigned int length);

BinStr int_to_BinStr(int n);

BinStr hex_to_BinStr(char *str);

BinStr ASCII_to_BinStr(char *str);

BinStr empty_BinStr(int length);

void destroy_BinStr(BinStr str);

BinStr copyStr(BinStr str);

BinStr set(BinStr str1, BinStr str2);

BinStr flush(BinStr str);

BinStr cut(BinStr str, int n);

BinStr subString(BinStr str, int begin, int end);

int bytes(BinStr str);

BinStr getByte(BinStr str, int n);

BinStr NOT(BinStr str);

BinStr XOR(BinStr str1, BinStr str2);

BinStr OR(BinStr str1, BinStr str2);

BinStr AND(BinStr str1, BinStr str2);

BinStr rotateL(BinStr str, int n);

BinStr rotateR(BinStr str, int n);

char *toString(BinStr str);

char *toASCII(BinStr str);

int toDecimal(BinStr str);

BinStr append(BinStr str1, BinStr str2);

void printStr(BinStr str);

bool parity(BinStr str);

BinStr add(BinStr str1, BinStr str2);

BinStr reverse(BinStr str);

BinStr permutate(BinStr str, int *order, int len, int offset);

BinStr reversePermutate(BinStr str, int *order, int len, int offset);

BinStr *split(BinStr str, int split_len);

#endif
