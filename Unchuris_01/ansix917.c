#include <stdio.h>

#include "./Ansix917.h"
#include "./DES.h"
#include <assert.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

BinStr TDES(BinStr str, BlockCipher DES1, BlockCipher DES2) {
	BinStr new = BlockEncrypt(str, NULL, DES1);
	new = set(new, BlockDecrypt(str, NULL, DES2));
	new = set(new, BlockEncrypt(str, NULL, DES1));
	return new;
}

BinStr ANSIX917(BinStr key, BinStr seed, int n) {
	if (key != NULL && seed != NULL && n >= 0 &&
		key->length == ANSIX917_KEY_SIZE && seed->length == ANSIX917_SEED_SIZE) {

		BinStr cur_seed = copyStr(seed);
		BinStr key1 = subString(key, 0, DES_KEY_SIZE - 1);
		BinStr key2 = subString(key, DES_KEY_SIZE, (DES_KEY_SIZE * 2) - 1);
		BlockCipher DES1 = DES_initialize(key1, "ECB");
		BlockCipher DES2 = DES_initialize(key2, "ECB");

		BinStr new = empty_BinStr(0);
		while (new->length < n) {

			BinStr cur_time = int_to_BinStr((unsigned)time(NULL));
			cur_time = set(cur_time, cut(cur_time, DES_BLOCK_SIZE));
			cur_time = set(cur_time, TDES(cur_time, DES1, DES2));
			BinStr old_time = copyStr(cur_time);
			cur_time = set(cur_time, XOR(cur_time, cur_seed));
			cur_time = set(cur_time, TDES(cur_time, DES1, DES2));
			new = set(new, append(new, cur_time));

			cur_time = set(cur_time, XOR(cur_time, old_time));
			cur_seed = set(cur_seed, TDES(cur_time, DES1, DES2));
			destroy_BinStr(cur_time);
			destroy_BinStr(old_time);
		}
		destroy_BinStr(cur_seed);

		destroy_BinStr(key1);
		destroy_BinStr(key2);
		DES_destroy(DES1);
		DES_destroy(DES2);

		new = set(new, cut(new, n));
		return new;
	}
	return empty_BinStr(0);
}

void gen_random(char *s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = 0;
}

BinStr ANSIX917_sequence(int n) {
	char* key = malloc(sizeof(char) * ANSIX917_KEY_SIZE);
	char* seed = malloc(sizeof(char) * ANSIX917_SEED_SIZE);
	gen_random(key, ANSIX917_KEY_SIZE);
	gen_random(seed, ANSIX917_SEED_SIZE);
	BinStr rez = ANSIX917(
		str_to_BinStr(key, ANSIX917_KEY_SIZE),
		str_to_BinStr(seed, ANSIX917_SEED_SIZE),
		n);
	return rez;
}

void CleanStdin() {
	char c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

int getSequenceSize() {
	int n = 0;
	char c = 0;
	while (true) {
		//_flushall(); 
		printf("Введите длину выходной последовательности: ");
		if (scanf("%i%c", &n, &c) == 2 && c == '\n' && n > 0 && n < 100000) {
			return n;
		}
		else {
			printf("Ошибка!\n");
		}
		CleanStdin();
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	printStr(ANSIX917_sequence(getSequenceSize()));
	getchar();
	return 0;
}

