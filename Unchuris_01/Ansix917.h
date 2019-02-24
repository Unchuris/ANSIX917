#ifndef ANSIX917_H
#define ANSIX917_H

#define ANSIX917_KEY_SIZE 128
#define ANSIX917_SEED_SIZE 64

#include "./BinStr.h"

BinStr ANSIX917_sequence(int n);

#endif
