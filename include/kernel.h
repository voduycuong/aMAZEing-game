#ifndef _KERNEL_H
#define _KERNEL_H

#include "mbox.h"
#include "uart.h"

void wait_msec(unsigned int n);
void set_wait_timer(int set, unsigned int msVal);

#endif