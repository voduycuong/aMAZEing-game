#include "gpio.h"

typedef struct
{
    uint32_t C;    // Control
    uint32_t S;    // Status
    uint32_t DLEN; // Data Length
    uint32_t A;    // Slave Address
    uint32_t FIFO; // Data FIFO
    uint32_t DIV;  // Clock Divider
    uint32_t DEL;  // Data Delay
    uint32_t CLKT; // Clock Stretch Timeout
} I2C_Master_regs;

typedef enum _i2c_status
{
    I2CS_SUCCESS,
    I2CS_ACK_ERROR,
    I2CS_DATA_LOSS,
    I2CS_CLOCK_TIMEOUT
} i2c_status;

#define I2C_Master0 ((volatile I2C_Master_regs *)(MMIO_BASE + 0x00205000))
#define I2C_Master1 ((volatile I2C_Master_regs *)(MMIO_BASE + 0x00804000))

// Control register
#define C_I2CEN (1 << 15)
#define C_INTR (1 << 10)
#define C_INTT (1 << 9)
#define C_INTD (1 << 8)
#define C_ST (1 << 7)
#define C_CLEAR (1 << 5)
#define C_READ (1 << 0)

// Status register
#define S_CLKT (1 << 9)
#define S_ERR (1 << 8)
#define S_RXF (1 << 7)
#define S_TXE (1 << 6)
#define S_RXD (1 << 5)
#define S_TXD (1 << 4)
#define S_RXR (1 << 3)
#define S_TXW (1 << 2)
#define S_DONE (1 << 1)
#define S_TA (1 << 0)

// I2C Slave: can connect to GPIO pins 10, 11 on RPI4, 18, 19 on RPI3
typedef struct
{
    uint32_t DR;      // Data Register
    uint32_t RSR;     // status and error clear
    uint32_t SLV;     // slave address value
    uint32_t CR;      // Control Register
    uint32_t FR;      // Flag Register
    uint32_t IFLS;    // Interrupt fifo level select register
    uint32_t IMSC;    // Interupt Mask Set Clear Register
    uint32_t RIS;     // Raw Interupt Status Register
    uint32_t MIS;     // Masked Interupt Status Register
    uint32_t ICR;     // Interupt Clear Register
    uint32_t DMACR;   // DMA Control Register
    uint32_t TDR;     // FIFO Test Data
    uint32_t GPUSTAT; // GPU Status Register
    uint32_t HCTRL;   // Host Control Register
    uint32_t DEBUG1;  // Host Control Register
    uint32_t DEBUG2;  // SPI Debug Register
} I2C_SPI_Slave_regs;

#define I2C_Slave ((volatile I2C_SPI_Slave_regs *)(MMIO_BASE + 0x00214000))

/* Slave Control register */
// invert transmit status flags
#define INV_TXF (1 << 13)
// enable host control
#define HOSTCTRLEN (1 << 12)
// enable test FIFO
#define SC_TESTFIFO (1 << 11)
// invert receive status flags
#define SC_INV_RXF (1 << 10)
// enable receive
#define SC_RXE (1 << 9)
// enable transmit
#define SC_TXE (1 << 8)
// abort operation and clear FIFOs
#define SC_BRK (1 << 7)
// send control register as first I2C byte
#define SC_ENCTRL (1 << 6)
// send status register as first I2C byte
#define SC_ENSTAT (1 << 5)
// set SPI polarity high
#define SC_CPOL (1 << 4)
// set SPI phase high
#define SC_CPHA (1 << 3)
// enable I2C mode
#define SC_I2C (1 << 2)
// enable SPI mode
#define SC_SPI (1 << 1)
// enable BSC peripheral
#define SC_EN (1 << 0)

// Flag register
#define BSC_FR_RXBUSY 32
#define BSC_FR_TXFE 16
#define BSC_FR_RXFF 8
#define BSC_FR_TXFF 4
#define BSC_FR_RXFE 2
#define BSC_FR_TXBUSY 1

/* BSC FIFO size */
#define BSC_FIFO_SIZE 512