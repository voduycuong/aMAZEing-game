// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"

/* Mailbox Data Buffer (each element is 32-bit) */
/*
 * The keyword attribute allows you to specify special attributes
 *
 * The aligned(N) attribute aligns the current data item on an address
 * which is a multiple of N, by inserting padding bytes before the data item
 *
 * __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
 * *
 * We must ensure that our our buffer is located at a 16 byte aligned address,
 * so only the high 28 bits contain the address
 * (last 4 bits is ZERO due to 16 byte alignment)
 *
 */

volatile unsigned int __attribute__((aligned(16))) mBuf[36];

/*
 * Read from the mailbox
 */
uint32_t mailbox_read(unsigned char channel)
{
    // Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to receive
        do
        {
            asm volatile("nop");
        } while (*MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = *MBOX0_READ;
    } while ((res & 0xF) != channel);
    return res;
}

/*
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
    // Sending message is buffer_addr & channel number
    //  Make sure you can send mail
    do
    {
        asm volatile("nop");
    } while (*MBOX1_STATUS & MBOX_FULL);
    // send the message
    *MBOX1_WRITE = msg;
}

/*
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mbox_call(uint32_t buffer_addr, unsigned char channel)
{
    unsigned int *buffer = (unsigned int *)((unsigned long)buffer_addr);

    // Check Buffer Address
    uart_puts("\nBuffer Address: ");
    uart_hex(buffer_addr);
    uart_sendc('\n');

    // Prepare Data (address of Message Buffer)
    uint32_t msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);
    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel))
    {
        /* is it a valid successful response (Response Code) ? */
        if (buffer[1] == MBOX_RESPONSE)
            uart_puts("Got successful response \n");
        return (buffer[1] == MBOX_RESPONSE);
    }
    return 0;
}

/*
 * Make a mailbox setup
 * buffer_addr: address of the being used mailbox buffer
 * tag_identifier: TAG indentifier value
 * **res_data: pointer of pointer, used to get the first address of response data
 * req_length: length of request value in bytes
 * res_length: length of response value in bytes
 * ...: list of parameters for request values (if necessary).
 */
void mbox_buffer_setup(uint32_t buffer_addr, uint32_t tag_identifier, uint32_t **res_data, uint32_t req_length, uint32_t res_length, ...)
{
    va_list ap;               // Type to hold information about variable arguments (type)
    va_start(ap, res_length); // Initialize a variable argument list (macro)

    uint32_t *buffer = (uint32_t *)((uint64_t)buffer_addr);
    uint32_t i = 0;

    buffer[i++] = 0;                                                  // mBuf[0]: will be filled later at the end.
    buffer[i++] = MBOX_REQUEST;                                       // Message Request Code (this is a request message)
    buffer[i++] = tag_identifier;                                     // TAG Identifier
    buffer[i++] = req_length >= res_length ? req_length : res_length; // Value buffer size in bytes
    buffer[i++] = 0;                                                  // REQUEST CODE = 0

    while (1)
    {
        int x = va_arg(ap, int); // Get next value
        if (x != 0)
            buffer[i++] = x;
        else
            break;
    }

    *res_data = (unsigned int *)&buffer[5];

    buffer[i++] = MBOX_TAG_LAST;
    buffer[0] = i * 4; // Message Buffer Size in bytes (4 bytes (32 bit) each)

    va_end(ap); // End using variable argument list
}