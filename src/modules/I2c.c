#include <stdlib.h>

#define I2C_READ  0xD1
#define I2C_WRITE 0xD0

#define I2CLH (* (unsigned int *) 0x4005C010) // v
#define I2CLL (* (unsigned int *) 0x4005C014) // v
#define PINMODE_OD0 (* (unsigned int *) 0x4002C068) // v
#define PINMODE_IC1 0x3
#define PINMODE0 (* (unsigned int *) 0x4002C040) // v

// Power control register, set PCI2C1 to 1 to enable I2C1
#define PCONP (* (unsigned int *) 0x400FC0C4) // v
#define PCI2C1 0x80000

#define PCLKSEL1 (* (unsigned int *) 0x400FC1AC) // v
#define PCLK_I2C1 0xC0

#define PINSEL0 (* (unsigned int *) 0x4002C000) // v
#define PINSEL0_SDA1 0x3
#define PINSEL0_SCL1 0xC

#define ISER0 (* (unsigned int *) 0xE000E100) // v
#define ISE_I2C1 0x800

#define I2STAT (* (unsigned int *) 0x4005C004) // v
#define I2C1CONSET (* (unsigned int *) 0x4005C000) // v
#define I2C1CONSET_EN 0x40
#define I2C1CONSET_STA 0x20
#define I2C1CONSET_STO 0x10
#define I2C1CONSET_ACK 0x04
#define I2C1CONCLR (* (unsigned int *) 0x4005C018) // v
#define I2C1CONCLR_SIC 0x10
#define I2C1DAT (* (unsigned int *) 0x4005C008) // v

#define SECONDS    (* (unsigned int *) 0x00)
#define MINUTES    (* (unsigned int *) 0x01)
#define HOURS      (* (unsigned int *) 0x02)
#define DAY        (* (unsigned int *) 0x03)
#define DATE       (* (unsigned int *) 0x04)
#define MONTH      (* (unsigned int *) 0x05)
#define YEAR       (* (unsigned int *) 0x06)
#define CONTROL    (* (unsigned int *) 0x07)

/*** Both master modes ***/
int masterDataCounter = 0;
unsigned char addressByte = 0;

/*** Master transmit mode ***/
unsigned char masterTransmitBuffer[8];
int masterTransmitBufferPointer = 0;

/*** Master receive mode ***/
unsigned char *masterReceiveBuffer;
int masterReceiveBufferPointer = 0;

void initI2C() {
	PCONP       |= PCI2C1;
    PCLKSEL1    |= PCLK_I2C1;
    PINSEL0     |= PINSEL0_SCL1 | PINSEL0_SDA1;
    ISER0       |= ISE_I2C1;
    PINMODE0    |= 0x0A;
    PINMODE_OD0 |= PINMODE_IC1;
    I2C1CONSET  |= 0x40;
    I2CLH        = 0x0A;
    I2CLL        = 0x0A;
}

void beginTransmission (unsigned char bytesAmount, unsigned char* data) {
    addressByte = I2C_WRITE;
    for (int i = 0; i < bytesAmount; i++) {
    	masterTransmitBuffer[i] = data[i];
    }
    masterTransmitBufferPointer = 0;
    masterDataCounter           = bytesAmount;
    I2C1CONSET                  = 0x20;
}

void receive(unsigned char bytesAmount) {
	masterDataCounter   = 0;
	addressByte         = I2C_READ;
	I2C1CONSET          = 0x20;
	memset(masterReceiveBuffer, 0, bytesAmount);
	masterDataCounter   = bytesAmount;
}

void debug(unsigned char state) {
	// the software will go here if the I2C state machine tries to handle a state that I'm not explicitly handling
	__asm("nop");
}

void block(int nops) {
	for (int i = 0; i < nops; i++) {
		__asm("nop");
	}
}

// interrupt
void I2C1_IRQHandler () {
	unsigned char status = I2STAT;
	switch (status) {
		case 0x00: // ✓
			/*** Bus error fault ***/
			I2C1CONSET = 0x14;
			I2C1CONCLR = 0x08;
			break;
		/* Master states; for both transmit and receive modes */
		case 0x08: // ✓
			/*** Start condition has been transmitted ***/
			I2C1DAT    = addressByte;
			I2C1CONSET = 0x04;
			I2C1CONCLR = 0x28;
			break;
		case 0x10: // ✓
			/*** Repeated start condition has been transmitted ***/
			I2C1DAT = addressByte;
			I2C1CONSET = 0x04;
			I2C1CONCLR = 0x28;
			break;
		/* Master transmitter states */
		case 0x18: // ✓
			/*** Slave addres + write has been transmitted and ACK has been received ***/
			I2C1DAT    = masterTransmitBuffer[masterTransmitBufferPointer];
			I2C1CONSET = 0x04;
			I2C1CONCLR = 0x08;
			masterTransmitBufferPointer += 1;
			break;
		case 0x20: // ✓
			/*** Slave address + write has been transmitted, NACK has been received ***/
			I2C1CONSET = 0x14;
			I2C1CONCLR = 0x08;
			break;
		case 0x28: // ✓
			/*** Data has been transmitted, ACK has been received, if transmitted data was last byte then transmit STOP
			 * otherwise transmit next data byte ***/
			if ((--masterDataCounter) == 0) {
				I2C1CONSET = 0x14;
				I2C1CONCLR = 0x08;
			} else {
				I2C1CONSET = 0x04;
				I2C1CONCLR = 0x08;
				I2C1DAT = masterTransmitBuffer[masterTransmitBufferPointer++];
			}
			break;
		case 0x30: // ✓
			/*** Data has been transmitted, NACK received ***/
			I2C1CONSET = 0x14;
			I2C1CONCLR = 0x08;
			break;
		case 0x38: // ✓
			I2C1CONSET = 0x24;
			I2C1CONCLR = 0x08;
			break;
		case 0x40:
			/*** Slave Address + read has been transmitted, ACK was received ***/
			I2C1CONSET = 0x04;
			I2C1CONCLR = 0x08;
			break;
		/* master receiver states */
		case 0x48:
			/*** Slave address + read has been transmitted, NACK was received ***/
			I2C1CONSET = 0x14;
			I2C1CONCLR = 0x08;
			break;
		case 0x50:
		    /*** Data has been received, ACK was returned. Data will be read from I2DAT.
		     * Additional data will be received
		     * If this is the last data byte, than NACK will be returned, otherwise ACK will be returned ***/
			masterReceiveBuffer[masterReceiveBufferPointer] = I2C1DAT;
		    if ((--masterDataCounter) == 0) {
		        I2C1CONCLR = 0x0C;
		    } else {
		        I2C1CONSET = 0x04;
		        I2C1CONCLR = 0x08;
		        masterReceiveBufferPointer++;
		    }
		    break;
		case 0x58:
			/*** Data has been received, NACK was returned. Data will be read from I2DAT. ***/
			masterReceiveBuffer[masterReceiveBufferPointer] = I2C1DAT;
			I2C1CONSET = 0x14;
			I2C1CONCLR = 0x08;
			break;
		default:
			debug(status);
			break;
	}

}
