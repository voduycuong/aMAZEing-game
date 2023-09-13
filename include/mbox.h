#ifndef _MBOX_H
#define _MBOX_H

#include "gpio.h"
#include "uart.h"

/* a properly aligned buffer */
extern volatile unsigned int mBuf[36];
#define ADDR(X) (uint32_t)((unsigned long)X)

/* Registers */
#define VIDEOCORE_MBOX (MMIO_BASE + 0x0000B880)
#define MBOX0_READ ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x00))
#define MBOX0_PEEK ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x10))
#define MBOX0_SENDER ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x14))
#define MBOX0_STATUS ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x18))
#define MBOX0_CONFIG ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x1C))
#define MBOX1_WRITE ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x20))
#define MBOX1_PEEK ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x30))
#define MBOX1_SENDER ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x34))
#define MBOX1_STATUS ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x38))
#define MBOX1_CONFIG ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x3C))

// Request/Response code in Buffer content
#define MBOX_RESPONSE 0x80000000
#define MBOX_REQUEST 0

// Status Value (from Status Register)
#define MBOX_FULL 0x80000000
#define MBOX_EMPTY 0x40000000

/* channels */
#define MBOX_CH_POWER 0 // Power management
#define MBOX_CH_FB 1    // Frame buffer
#define MBOX_CH_VUART 2 // Virtual UART
#define MBOX_CH_VCHIQ 3 // VCHIQ
#define MBOX_CH_LEDS 4  // LEDs
#define MBOX_CH_BTNS 5  // Buttons
#define MBOX_CH_TOUCH 6 // Touch screen
#define MBOX_CH_PROP 8  // Property tags (ARM -> VC)

/* tags */
#define MBOX_TAG_GETFIRMWARE 0x00000001  // Get firmware revision
#define MBOX_TAG_GETMODEL 0x00010001     // Get board model
#define MBOX_TAG_GETREVISION 0x00010002  // Get board revision
#define MBOX_TAG_GETMACADDR 0x00010003   // Get board MAC address
#define MBOX_TAG_GETSERIAL 0x00010004    // Get board serial
#define MBOX_TAG_GETARMMEMORY 0x00010005 // Get ARM memory
#define MBOX_TAG_GETCLKRATE 0x00030002   // Get clock rate
#define MBOX_TAG_GETVOLT 0x00030003      // Get voltage
#define MBOX_TAG_GETPHYWH 0x00040003     // Get physical (display) width/height

#define MBOX_TAG_SETCLKRATE 0x00038002 // Set clock rate

// New Tags for Screen Display
#define MBOX_TAG_SETPHYWH 0x48003
#define MBOX_TAG_SETVIRTWH 0x48004
#define MBOX_TAG_SETVIRTOFF 0x48009
#define MBOX_TAG_SETDEPTH 0x48005
#define MBOX_TAG_SETPXLORDR 0x48006
#define MBOX_TAG_GETFB 0x40001
#define MBOX_TAG_GETPITCH 0x40008

#define MBOX_TAG_LAST 0

/* Function Prototypes */
int mbox_call(uint32_t buffer_addr, unsigned char channel);

#endif