#include "i2c.h"
#define I2C_SPEED 100000

/**
 * I2C Mode Selection: Master or Slave
 */
#define I2C_MASTER_MODE

// #define I2C_SLAVE_MODE
#define I2C_SLAVE_ADDR 21

void i2c_master_init()
{
    unsigned int r;
    /* map I2C_Master1 to GPIO pins 2 and 3 */
    r = GPFSEL1;
    r &= ~((7 << 6) | (7 << 9));      // clear bits 11-6 (FSEL2, FSEL3)
    r |= (0b100 << 6) | (0b100 << 9); // set value 0b100 (select ALT0: SDA1/SCL1)
    GPFSEL1 = r;
    /* enable GPIO 2, 3 */

#ifdef RPI3    // RPI3
    GPPUD = 0; // No pull up/down control
    // Toogle clock to flush GPIO setup
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }                                // waiting 150 cycles
    GPPUDCLK0 = (1 << 2) | (1 << 3); // enable clock for GPIO 2, 3
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }              // waiting 150 cycles
    GPPUDCLK0 = 0; // flush GPIO setup
#else              // RPI4
    r = GPIO_PUP_PDN_CNTRL_REG0;
    r &= ~((3 << 4) | (3 << 6)); // No resistor is selected for GPIO 2, 3
    GPIO_PUP_PDN_CNTRL_REG0 = r;
#endif

    // Setting clock divider
    I2C_Master1->DIV = CORE_CLOCK_SPEED / I2C_SPEED;
}

/**
 * Receive data
 */
i2c_status i2c_master_recv(uint8_t address, uint8_t *buffer, uint32_t size)
{
    int count = 0;
    // Setup
    I2C_Master1->A = address;                 // slave address
    I2C_Master1->C = C_CLEAR;                 // clear FIFO
    I2C_Master1->S = S_CLKT | S_ERR | S_DONE; // clear CLKT ERR DONE bits
    I2C_Master1->DLEN = size;                 // Data Length
    // Enable I2C, Start Transfer, Read operation
    I2C_Master1->C = C_I2CEN | C_ST | C_READ;
    //
    while (!(I2C_Master1->S & S_DONE))
    {
        while (I2C_Master1->S & S_RXD)
        {
            *buffer++ = I2C_Master1->FIFO & 0xFF;
            count++;
        }
    }
    while (count < size && I2C_Master1->S & S_RXD)
    {
        *buffer++ = I2C_Master1->FIFO & 0xFF;
        count++;
    }
    uint32_t status = I2C_Master1->S;
    I2C_Master1->S = S_DONE;
    if (status & S_ERR)
    {
        return I2CS_ACK_ERROR;
    }
    else if (status & S_CLKT)
    {
        return I2CS_CLOCK_TIMEOUT;
    }
    else if (count < size)
    {
        return I2CS_DATA_LOSS;
    }
    return I2CS_SUCCESS;
}

/**
 * Send data
 */
i2c_status i2c_master_send(uint8_t address, uint8_t *buffer, uint32_t size)
{
    int count = 0;
    I2C_Master1->A = address;
    I2C_Master1->C = C_CLEAR;
    I2C_Master1->S = S_CLKT | S_ERR | S_DONE;
    I2C_Master1->DLEN = size;
    I2C_Master1->C = C_I2CEN | C_ST;

    while (!(I2C_Master1->S & S_DONE))
    {
        while (count < size && I2C_Master1->S & S_TXD)
        {
            I2C_Master1->FIFO = *buffer++;
            count++;
        }
    }

    uint32_t status = I2C_Master1->S;
    I2C_Master1->S = S_DONE;
    if (status & S_ERR)
    {
        return I2CS_ACK_ERROR;
    }
    else if (status & S_CLKT)
    {
        return I2CS_CLOCK_TIMEOUT;
    }
    else if (count < size)
    {
        return I2CS_DATA_LOSS;
    }
    return I2CS_SUCCESS;
}

void i2c_slave_init()
{
    unsigned int r;

#ifdef RPI3 // RPI3
    /* map I2C_Slave function to GPIO pins 18 and 19, ALT3 */
    r = GPFSEL1;
    r &= ~((7 << 24) | (7 << 27));      // clear bits (FSEL18, FSEL19)
    r |= (0b111 << 24) | (0b111 << 27); // set value 0b111 (select ALT3: BSCSL SDA/BSCSLSCL)
    GPFSEL1 = r;
    /* enable GPIO 18, 19 */
    GPPUD = 0; // No pull up/down control
    // Toogle clock to flush GPIO setup
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }                                  // waiting 150 cycles
    GPPUDCLK0 = (1 << 18) | (1 << 19); // enable clock for GPIO 18, 19
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }              // waiting 150 cycles
    GPPUDCLK0 = 0; // flush GPIO setup

#else // RPI4
    /* map I2C_Slave function to GPIO pins 10 and 11, ALT3 */
    r = GPFSEL1;
    r &= ~((7 << 0) | (7 << 3));      // clear bits (FSEL10, FSEL11)
    r |= (0b111 << 0) | (0b111 << 3); // set value 0b111 (select ALT3: BSCSL SDA/BSCSL SCL)
    GPFSEL1 = r;
    /* enable GPIO 10, 11 */
    r = GPIO_PUP_PDN_CNTRL_REG0;
    r &= ~((3 << 20) | (3 << 22)); // No resistor is selected for GPIO 10, 11
    GPIO_PUP_PDN_CNTRL_REG0 = r;

#endif

    I2C_Slave->CR = 0;     /* clear control register */
    I2C_Slave->RSR = 0;    /* clear underrun and overrun errors */
    I2C_Slave->SLV = 0;    /* clear I2C slave address */
    I2C_Slave->IMSC = 0xf; /* mask off all interrupts */
    I2C_Slave->ICR = 0x0f; /* clear all interrupts */

    // Setting slave address
    I2C_Slave->SLV = I2C_SLAVE_ADDR;

    // Enable I2C Slave
    I2C_Slave->CR = SC_RXE | SC_TXE | SC_I2C | SC_EN;
}

/**
 * Send data
 */
i2c_status i2c_slave_recv(uint8_t *buffer, uint32_t size)
{
    I2C_Slave->SLV = I2C_SLAVE_ADDR;
    I2C_Slave->CR &= 0x3fff; /* clear unused higher bits of CR */
    I2C_Slave->RSR = 0;      /* clear underrun and overrun errors */
    int count = 0;
    int active = 1;
    while (active)
    {
        active = 0;
        // Sending data
        while (count < size && (!(I2C_Slave->FR & BSC_FR_TXFF)))
        {
            I2C_Slave->DR = buffer[count++];
            active = 1;
        }

        if (!active)
        {
            active = I2C_Slave->FR & (BSC_FR_RXBUSY | BSC_FR_TXBUSY);
        }

        if (active)
        {
            // waiting some cycles
            uint32_t r = 200;
            while (r--)
            {
                asm volatile("nop");
            }
        }
    }
    return (count << 16) | (I2C_Slave->FR & 0xFFFF);
}

/**
 * Receive data
 */
i2c_status i2c_slave_send(uint8_t *buffer, uint32_t size)
{
    I2C_Slave->SLV = I2C_SLAVE_ADDR;
    I2C_Slave->CR &= 0x3fff; /* clear unused higher bits of CR */
    I2C_Slave->RSR = 0;      /* clear underrun and overrun errors */
    int count = 0;
    int active = 1;

    while (active)
    {
        active = 0;
        // Receving data

        while ((count < BSC_FIFO_SIZE) && (!(I2C_Slave->FR & BSC_FR_RXFE)))
        {
            buffer[count++] = I2C_Slave->DR;
            active = 1;
        }

        if (!active)
        {
            active = I2C_Slave->FR & (BSC_FR_RXBUSY | BSC_FR_TXBUSY);
        }

        if (active)
        {
            // waiting some cycles
            uint32_t r = 200;
            while (r--)
            {
                asm volatile("nop");
            }
        }
    }

    return (count << 16) | (I2C_Slave->FR & 0xFFFF);
}