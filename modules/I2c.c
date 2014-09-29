// use these two to set I2C0 clock, default is CCLK/4 (CCLK = cpu clock)
#define PCLKSEL0 (* (unsigned int *) 0x400FC1A8)
#define PCLK_I2C0 0xC000

// Power control register, set PCI2C0 to 1 to enable I2C0
#define PCONP (* (unsigned int *) 0x400FC0C4)
#define PCI2C0 0x80

// #define GPIO0Data (* (unsigned int *) 0x)

// I2C0 Interrupt is at http://www.nxp.com/documents/user_manual/UM10360.pdf#G7.1002159337
// vector offset 0x68, interrupt ID 10
// Interrupt enable register
#define ISER0 (* (unsigned int *) 0xE000E100)
#define ISE_I2C0 0x400

// I2C0 pin configuration http://www.nxp.com/documents/user_manual/UM10360.pdf#page=128&zoom=auto,-206,582

// I2C0 pin configuration register http://www.nxp.com/documents/user_manual/UM10360.pdf#page=128&zoom=auto,-206,582
// set all 4 bits to zero to use standard mode
#define I2CPADCFG (* (unsigned int *) 0x4002C07C)
#define I2CFASTMODE 0xF

// I2C pins
// SDA0 = P0[27]
// SCL0 = P0[28]