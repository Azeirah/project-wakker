// use these two to set I2C1 clock, default is CCLK/4 (CCLK = cpu clock)
// #define PCLKSEL0 (* (unsigned int *) 0x400FC1A8)
// #define PCLK_I2C1 0xC000

#define I2C_READ 1
#define I2C_WRITE ~1

// Power control register, set PCI2C1 to 1 to enable I2C1
#define PCONP (* (unsigned int *) 0x400FC0C4)
#define PCI2C1 0x80000

#define PCLKSEL1 0x400FC1AC
#define PCLK_I2C1 0xC0

// Pin function selection register.
#define PINSEL1 (* (unsigned int *) 0x4002C004)
// both must be set to 11
#define PINSEL1_SDA1 0x0C0
#define PINSEL1_SCL1 0x300

// I2C1 Interrupt is at http://www.nxp.com/documents/user_manual/UM10360.pdf#G7.1002159337
// vector offset 0x6C, interrupt ID 11
// Interrupt enable register
#define ISER0 (* (unsigned int *) 0xE000E100)
#define ISE_I2C1 0x800

// slave address
#define SLA 0x68

#define I2C1CONSET (* (unsigned int *) 0x4005C000)
#define I2C1CONSET_EN 0x40
// bit 5, start condition
#define I2C1CONSET_STA 0x20
// bit 4, stop condition
#define I2C1CONSET_STO 0x10
// bit 2, acknowledge signal
#define I2C1CONSET_ACK 0x04

#define I2C1CONCLR 0x400C018
#define I2C1CONCLR_SIC 0x10

// data register
#define I2C1DAT 0x4005C008

void initI2C() {
    // enable power for I2C1
    PCONP |= PCI2C1;
    // set clock to default (is normally default anyway...)
    PCLKSEL0 |= PCLK_I2C1;
    // set function of pins P0[27] and p0[28] to I2C1 SDA and SCL
    PINSEL1 |= PINSEL1_SCL1 | PINSEL1_SDA1;
    // enable interrupts for I2C1
    ISER0 |= ISE_I2C1;

    I2C1CONSET |= I2C1CONSET_EN;
}

void startCondition() {
    I2C1CONSET |= I2C1CONSET_STA;
}

void stopCondition() {
    I2C1CONSET |= I2C1CONSET_STO;
}

void sendAcknowledge() {
    I2C1CONSET |= I2C1CONSET_ACK;
}

void writeByte (unsigned char byte) {
    I2C1DAT |= byte;
}

void writeData (unsigned char data) {
    // start command
    writeByte((SLA << 1) | I2C_WRITE);
    I2CCONSET |= I2C1CONSET_STA;
    writeByte(data);
    // initialize the master data counter to match the length of the message being sent?
}

unsigned char readData () {
    writeByte((SLA << 1) | I2C_WRITE);
    startCondition();
    // initialize the master data counter to match the length of the message being sent?
    return I2C1DAT;
}

void clearI2CInterrupt () {
    I2C1CONCLR |= I2C1CONCLR_SIC;
}

