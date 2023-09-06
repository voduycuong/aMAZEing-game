#ifndef _RAND_H
#define _RAND_H

#include "gpio.h"

void rand_init();
unsigned int rand(unsigned int min, unsigned int max);

#endif
