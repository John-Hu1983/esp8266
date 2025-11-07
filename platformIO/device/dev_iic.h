#ifndef _DEV_IIC_H_
#define _DEV_IIC_H_

#define I2C_SDA_PIN 4 // D2引脚
#define I2C_SCL_PIN 5 // D1引脚
#define I2C_DEVICE_ADDRESS 0x3c

void init_i2c_object(u32 freq);
void scan_i2c_devices(void);
bool writeSingleByte(byte devid, byte registerAddress, byte data);
bool writeMultipleBytes(byte devid, byte startRegister, byte *data, size_t length);
byte readSingleByte(byte devid, byte registerAddress);
bool readMultipleBytes(byte devid, byte startRegister, byte *buffer, size_t length);

#endif
