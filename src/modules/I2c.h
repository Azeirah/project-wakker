/* This file was automatically generated.  Do not edit! */
void I2C1_IRQHandler();
void block(int nops);
void debug(unsigned char state);
void receive(unsigned char bytesAmount);
void beginTransmission(unsigned char bytesAmount,unsigned char *data);
void initI2C();
extern int masterReceiveBufferPointer;
extern unsigned char *masterReceiveBuffer;
extern int masterTransmitBufferPointer;
extern unsigned char masterTransmitBuffer[8];
extern unsigned char addressByte;
extern int masterDataCounter;
