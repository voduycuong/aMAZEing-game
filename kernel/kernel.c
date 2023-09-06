#include "mbox.h"
#include "framebf.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"

// void main()
// {
//     // set up serial console
//     uart_init();
//     // say hello
//     uart_puts("Hello World!\n");
    
//     // mailbox data buffer: Read ARM frequency
//     mBuf[0] = 17*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
//     mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    
//     mBuf[2] = 0x00030002; // TAG Identifier: Get clock rate
//     mBuf[3] = 8; // Value buffer size in bytes (max of request and response lengths)
//     mBuf[4] = 0; // REQUEST CODE = 0
//     mBuf[5] = 3; // clock id: ARM system clock
//     mBuf[6] = 0; // clear output buffer (response data are mBuf[5] & mBuf[6])

//     mBuf[7] = 0x00000001; // TAG Identifier: Get firmware revision
//     mBuf[8] = 4; // Value buffer size in bytes (max of request and response lengths)
//     mBuf[9] = 0; // REQUEST CODE = 0
//     mBuf[10] = 0; // clear output buffer (response data are mBuf[10])

//     mBuf[11] = 0x00030002; // TAG Identifier: Get clock rate
//     mBuf[12] = 8; // Value buffer size in bytes (max of request and response lengths)
//     mBuf[13] = 0; // REQUEST CODE = 0
//     mBuf[14] = 2; // clock id: UART clock
//     mBuf[15] = 0; // clear output buffer (response data are mBuf[14] & mBuf[15])

//     mBuf[16] = MBOX_TAG_LAST;
    
//     //Note: Board model and Board serial may give 0 values on QEMU. 
//     //Board revision, board MAC address: try it yourself, since it is assignment tasks.

//     if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
//         uart_puts("\nResponse Code for whole message: ");
//         uart_hex(mBuf[1]);

//         uart_puts("\n+ Response Code in Message TAG: ");
//         uart_hex(mBuf[4]);
//         uart_puts("\nDATA: ARM clock rate = ");
//         uart_dec(mBuf[6]);

//         uart_puts("\n+ Response Code in Message TAG: ");
//         uart_hex(mBuf[9]);
//         uart_puts("\nDATA: firmware revision = ");
//         uart_hex(mBuf[10]);

//         uart_puts("\n+ Response Code in Message TAG: ");
//         uart_hex(mBuf[13]);
//         uart_puts("\nDATA: UART clock rate = ");
//         uart_dec(mBuf[15]);

//     } else {
//         uart_puts("Unable to query!\n");
//     }

//     // echo everything back
//     while(1) {
//         //read each char
//         char c = uart_getc();
//         //send back
//         uart_sendc(c);
//     }
// }

void main() {
    // set up serial console
    uart_init();
    // say hello
    uart_puts("\n\nHello World!\n");
    // Initialize frame buffer
    framebf_init();
    // Run the application to display image
    run_application();
}