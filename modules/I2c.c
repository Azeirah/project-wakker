// use these two to set I2C0 clock, default is CCLK/4 (CCLK = cpu clock)
#define PCLKSEL0 (* (unsigned int *) 0x400FC1A8)
#define PCLK_I2C0 0xC000

// Power control register, set PCI2C0 to 1 to enable I2C0
#define PCONP (* (unsigned int *) 0x400FC0C4)
#define PCI2C0 0x80

// Pin function selection register.
#define PINSEL1 (* (unsigned int *) 0x4002C004)
// both must be set to 01
#define PINSEL1_SDA0 0x0C00000
#define PINSEL1_SCL0 0x3000000

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
// I2Cpins
// SDA0 = P0[27]
// SCL0 = P0[28]

// slave address
#define SLA 0x68

#define I2C0CONSET (* (unsigned int *) 0x4001C000)
#define I2C0CONSET_EN 0x40
// bit 5
#define I2C0CONSET_STA 0x20
// bit 4
#define I2C0CONSET_STO 0x10
// bit 2
#define I2C0CONSET_ACK 0x04

void initI2C() {
    // enable power for I2C0
    PCONP |= PCI2C0;
    // set clock to default (is normally default anyway...)
    PCLKSEL0 |= 0xFFFF & PCLK_I2C0;
    // set function of pins P0[27] and p0[28] to I2C0 SDA and SCL
    PINSEL1 |= PINSEL1_SCL0 & (0x1 << 22) | PINSEL1_SDA0 & (0x1 << 24);
    // enable interrupts for I2C0
    ISER0 |= ISE_I2C0;

    I2C0CONSET |= I2C0CONSET_EN;
}

void startCondition() {
    I2C0CONSET |= I2C0CONSET_STA;
}

void stopCondition() {
    I2C0CONSET |= I2C0CONSET_STO;
}

void sendAcknowledge() {
    I2C0CONSET |= I2C0CONSET_ACK;
}
