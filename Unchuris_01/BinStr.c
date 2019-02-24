#include "./BinStr.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int BITS_PER_WORD = 32;
const int BITS_PER_BYTE = 8;
const int MAX_BYTE = 256;

char HEX_CONVERT[][16] = { "0000", "0001", "0010", "0011", "0100", "0101",
						  "0110", "0111", "1000", "1001", "1010", "1011",
						  "1100", "1101", "1110", "1111" };

BinStr str_to_BinStr(char *bits, unsigned int length) {
	BinStr new = malloc(sizeof(struct binstr));
	new->bits = malloc(sizeof(bool) * length);
	for (int i = 0; i < length; i++) {
		if (bits[i] == '1') {
			new->bits[i] = 1;
		}
		else {
			new->bits[i] = 0;
		}
	}
	new->length = length;
	return new;
}

BinStr int_to_BinStr(int n) {
	assert(n >= 0);
	if (n == 0) {
		return empty_BinStr(1);
	}
	else {
		int length = log(n) / log(2) + 1;
		BinStr new = empty_BinStr(length);
		unsigned int powerOfTwo = 1;
		for (int i = 0; i < length - 1; i++) {
			powerOfTwo *= 2;
		}
		for (int i = 0; i < length; i++) {
			if (n >= powerOfTwo) {
				n -= powerOfTwo;
				new->bits[i] = 1;
			}
			powerOfTwo /= 2;
		}
		return new;
	}
}

BinStr hex_to_BinStr(char *str) {
	assert(str != NULL);
	int length = strlen(str);
	BinStr new = empty_BinStr(length * 4);
	for (int i = 0; i < length; i++) {
		char *hex;
		if ('0' <= str[i] && str[i] <= '9') {
			hex = HEX_CONVERT[(str[i] - '0')];
		}
		else if ('A' <= str[i] && str[i] <= 'F') {
			hex = HEX_CONVERT[(str[i] - 'A') + 10];
		}
		else if ('a' <= str[i] && str[i] <= 'f') {
			hex = HEX_CONVERT[(str[i] - 'a') + 10];
		}
		for (int j = 0; j < 4; j++) {
			if (hex[j] == '1') {
				new->bits[i * 4 + j] = true;
			}
		}
	}
	return new;
}

BinStr ASCII_to_BinStr(char *str) {
	assert(str != NULL);
	int length = strlen(str);
	BinStr new = empty_BinStr(0);
	for (int i = 0; i < length; i++) {
		BinStr app = int_to_BinStr((int)str[i]);
		app = set(app, cut(app, BITS_PER_BYTE));
		new = set(new, append(new, app));
		destroy_BinStr(app);
	}
	return new;
}

BinStr empty_BinStr(int length) {
	assert(length >= 0);
	BinStr new = malloc(sizeof(struct binstr));
	new->bits = malloc(sizeof(bool) * length);
	for (int i = 0; i < length; i++) {
		new->bits[i] = 0;
	}
	new->length = length;
	return new;
}

void destroy_BinStr(BinStr str) {
	assert(str != NULL);
	free(str->bits);
	free(str);
	return;
}

BinStr copyStr(BinStr str) {
	assert(str != NULL);
	BinStr new = empty_BinStr(str->length);
	for (int i = 0; i < str->length; i++) {
		new->bits[i] = str->bits[i];
	}
	return new;
}

BinStr set(BinStr str1, BinStr str2) {
	if (str1 != NULL) {
		destroy_BinStr(str1);
	}
	return str2;
}

BinStr flush(BinStr str) {
	assert(str != NULL);
	int leading = 0;
	while (!str->bits[leading]) {
		leading++;
	}
	if (leading == 0) {
		return str;
	}
	else if (leading == str->length) {
		return empty_BinStr(1);
	}
	else {
		char *new = malloc(sizeof(char) * (str->length - leading));
		for (int i = leading; i < str->length; i++) {
			if (str->bits[i]) {
				new[i - leading] = '1';
			}
			else {
				new[i - leading] = '0';
			}
		}
		BinStr newStr = str_to_BinStr(new, str->length - leading);
		free(new);
		return newStr;
	}
}

BinStr cut(BinStr str, int n) {
	assert(str != NULL && n > 0);
	BinStr new = empty_BinStr(n);
	for (int i = 1; i <= n && i <= str->length; i++) {
		new->bits[n - i] = str->bits[str->length - i];
	}
	return new;
}

BinStr subString(BinStr str, int begin, int end) {
	if (str != NULL && begin <= end && end <= str->length) {
		BinStr new = empty_BinStr(end - begin + 1);
		for (int i = 0; i < new->length; i++) {
			new->bits[i] = str->bits[i + begin];
		}
		return new;
	} 
	return empty_BinStr(0);
}

int bytes(BinStr str) {
	assert(str != NULL);
	if (str->length % BITS_PER_BYTE == 0) {
		return str->length / BITS_PER_BYTE;
	}
	else {
		return 1 + (str->length / BITS_PER_BYTE);
	}
}

BinStr getByte(BinStr str, int n) {
	assert(str != NULL && n <= str->length / BITS_PER_BYTE && n >= 0);
	BinStr new = empty_BinStr(BITS_PER_BYTE);
	int firstBit = (n * BITS_PER_BYTE) - (str->length % BITS_PER_BYTE);
	for (int i = firstBit; i <= firstBit + BITS_PER_BYTE; i++) {
		if (i >= 0 && i < str->length) {
			new->bits[i - firstBit] = str->bits[i];
		}
	}
	return new;
}

BinStr NOT(BinStr str) {
	assert(str != NULL);
	char *bits = malloc(sizeof(char) * (str->length));
	for (int i = 0; i < str->length; i++) {
		bits[i] = !str->bits[i];
	}
	BinStr new = str_to_BinStr(bits, str->length);
	free(bits);
	return new;
}

BinStr XOR(BinStr str1, BinStr str2) {
	assert(str1 != NULL && str2 != NULL && str1->length == str2->length);
	char *bits = malloc(sizeof(char) * (str1->length));
	for (int i = 0; i < str1->length; i++) {
		if (str1->bits[i] ^ str2->bits[i] == 0) {
			bits[i] = '0';
		}
		else {
			bits[i] = '1';
		}
	}
	BinStr new = str_to_BinStr(bits, str1->length);
	free(bits);
	return new;
}

BinStr OR(BinStr str1, BinStr str2) {
	assert(str1 != NULL && str2 != NULL && str1->length == str2->length);
	char *bits = malloc(sizeof(char) * (str1->length));
	for (int i = 0; i < str1->length; i++) {
		if (str1->bits[i] | str2->bits[i] == 0) {
			bits[i] = '0';
		}
		else {
			bits[i] = '1';
		}
	}
	BinStr new = str_to_BinStr(bits, str1->length);
	free(bits);
	return new;
}

BinStr AND(BinStr str1, BinStr str2) {
	assert(str1 != NULL && str2 != NULL && str1->length == str2->length);
	char *bits = malloc(sizeof(char) * (str1->length));
	for (int i = 0; i < str1->length; i++) {
		if (str1->bits[i] & str2->bits[i] == 0) {
			bits[i] = '0';
		}
		else {
			bits[i] = '1';
		}
	}
	BinStr new = str_to_BinStr(bits, str1->length);
	free(bits);
	return new;
}

BinStr rotateL(BinStr str, int n) {
	assert(str != NULL && n >= 0);
	n %= str->length;
	BinStr new = subString(str, n, str->length - 1);
	BinStr back = subString(str, 0, n - 1);
	new = set(new, append(new, back));
	destroy_BinStr(back);
	return new;
}

BinStr rotateR(BinStr str, int n) {
	assert(str != NULL && n >= 0);
	n %= str->length;
	BinStr new = subString(str, n, str->length - 1);
	BinStr back = subString(str, 0, n - 1);
	new = set(new, append(back, new));
	destroy_BinStr(back);
	return new;
}

char *toString(BinStr str) {
	assert(str != NULL);
	char *new = malloc(sizeof(char) * (str->length));
	for (int i = 0; i < str->length; i++) {
		new[i] = str->bits[i];
	}
	new[str->length] = '\0';
	return new;
}

int toDecimal(BinStr str) {
	assert(str != NULL);
	if (str->length > 31) {
		return -1;
	}
	else {
		int sum = 0;
		int power2 = 1;
		for (int i = str->length - 1; i >= 0; i--) {
			if (str->bits[i] == 1) {
				sum += power2;
			}
			power2 *= 2;
		}
		return sum;
	}
}

char *toASCII(BinStr str) {
	char *new = malloc(sizeof(char) * ((str->length / BITS_PER_BYTE) + 2));
	for (int i = 0; i < (str->length / BITS_PER_BYTE) + 1; i++) {
		new[i] = (char)toDecimal(getByte(str, i));
	}
	new[(str->length / 8) + 1] = '\0';
	return new;
}

BinStr append(BinStr str1, BinStr str2) {
	assert(str1 != NULL && str2 != NULL);
	BinStr new = empty_BinStr(str1->length + str2->length);
	for (int i = 0; i < str1->length; i++) {
		new->bits[i] = str1->bits[i];
	}
	for (int i = 0; i < str2->length; i++) {
		new->bits[i + str1->length] = str2->bits[i];
	}
	return new;
}

void printStr(BinStr str) {
	if (str == NULL) {
		printf("NULL");
	}
	else {
		for (int i = 0; i < str->length; i++) {
			printf("%d", str->bits[i]);
		}
	}
}

bool parity(BinStr str) {
	assert(str != NULL);
	bool result = 0;
	for (int i = 0; i < str->length; i++) {
		result = result ^ str->bits[i];
	}
	return result;
}

BinStr add(BinStr str1, BinStr str2) {
	BinStr new = empty_BinStr(str1->length + str2->length);
	int offset = 0;
	for (int i = 1; i <= new->length; i++) {
		if (str1->length >= i && str1->bits[str1->length - i]) offset++;
		if (str2->length >= i && str2->bits[str2->length - i]) offset++;
		if ((offset % 2 != 0 || offset == 1) && offset != 0) {
			new->bits[new->length - i] = true;
			offset--;
		}
		offset = offset / 2;
	}
	return set(new, flush(new));
}

BinStr reverse(BinStr str) {
	assert(str != NULL);
	BinStr new = empty_BinStr(str->length);
	for (int i = 0; i < str->length; i++) {
		new->bits[i] = str->bits[str->length - i - 1];
	}
	return new;
}

BinStr permutate(BinStr str, int *order, int len, int offset) {
	assert(str != NULL && order != NULL && len >= 0 && offset >= 0);
	BinStr new = empty_BinStr(len);
	for (int i = 0; i < len; i++) {
		assert(order[i] - offset < str->length);
		new->bits[i] = str->bits[order[i] - offset];
	}
	return new;
}

BinStr reversePermutate(BinStr str, int *order, int len, int offset) {
	assert(str != NULL && order != NULL && len >= 0 && offset >= 0);
	BinStr new = empty_BinStr(len);
	for (int i = 0; i < len; i++) {
		assert(order[i] - offset < str->length);
		new->bits[order[i] - offset] = str->bits[i];
	}
	return new;
}

BinStr *split(BinStr str, int split_len) {
	assert(str != NULL && split_len > 0 && str->length % split_len == 0);
	BinStr *list = malloc(sizeof(BinStr) * str->length / split_len);
	for (int i = 0; i < str->length; i += split_len) {
		list[i] = subString(str, i, i + split_len - 1);
	}
	return list;
}