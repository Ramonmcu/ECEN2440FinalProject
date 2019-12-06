/*
 * random.c
 *
 *  Created on: Nov 17, 2019
 *      Author: Alexandra Ferguson
 */

#include "msp.h"

int fib(int seed) {
    if(seed <= 0){
         return seed;
    }
    else {
        return fib(seed - 1) + fib(seed - 2);
    }
}

int shift(int fib_num){
    uint32_t shifted;
    shifted = fib_num;
    return shifted;
}

void naughty_or_nice(int shifted_value) {
    uint8_t naughty;
    if(shifted_value % 2 == 0){
        naughty = 1;
    }
    if(shifted_value % 2 == 1) {
        naughty = 2;
    }

    shifted_value = shifted_value << 2;
}

