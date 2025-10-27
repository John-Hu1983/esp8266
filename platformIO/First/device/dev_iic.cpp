#include "../inc/includes.h"

/*
 * @brief Initialize I2C bus object
 * @param freq I2C bus frequency
 * @return None
 */
void init_i2c_object(u32 freq)
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(freq);
#if LOG_SHOW_I2C
  Serial.println("I2C bus initialization completed");
#endif
  scan_i2c_devices();
}

/*
 * @brief Scan all devices on the I2C bus
 * @param None
 * @return None
 */
void scan_i2c_devices(void)
{
  byte error, address;
  int devicesFound = 0;

#if LOG_SHOW_I2C
  Serial.println("Scanning for I2C devices...");
#endif

  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
#if LOG_SHOW_I2C
      Serial.print("Found I2C device at address: 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
#endif
      devicesFound++;
    }
    else if (error == 4)
    {
#if LOG_SHOW_I2C
      Serial.print("Unknown error at address 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      Serial.println("Unknown error occurred");
#endif
    }
  }

#if LOG_SHOW_I2C
  if (devicesFound == 0)
  {
    Serial.println("No I2C devices found");
  }
  else
  {
    Serial.print("Total of ");
    Serial.print(devicesFound);
    Serial.println(" I2C devices found");
  }
#endif
}

/*
 * @brief Write single byte data to I2C device
 * @param devid I2C device address
 * @param registerAddress Register address to write
 * @param data Data byte to write
 * @return True if write is successful, false otherwise
 */
bool writeSingleByte(byte devid, byte registerAddress, byte data)
{
  Wire.beginTransmission(devid);
  Wire.write(registerAddress); // Send register address
  Wire.write(data);            // Send data
  byte error = Wire.endTransmission();

  if (error == 0)
  {
#if LOG_SHOW_I2C
    Serial.print("Successfully written: register=0x");
    Serial.print(registerAddress, HEX);
    Serial.print(", data=0x");
    Serial.println(data, HEX);
#endif
    return true;
  }
  else
  {
#if LOG_SHOW_I2C
    Serial.print("Write failed, error code: ");
    Serial.println(error);
#endif
    return false;
  }
}

/*
 * @brief Write multiple bytes data to I2C device
 * @param devid I2C device address
 * @param startRegister Register address to start writing
 * @param data Pointer to data byte array to write
 * @param length Number of bytes to write
 * @return True if write is successful, false otherwise
 */
bool writeMultipleBytes(byte devid, byte startRegister, byte *data, size_t length)
{
  Wire.beginTransmission(devid);
  Wire.write(startRegister); // Send starting register address

  for (size_t i = 0; i < length; i++)
  {
    Wire.write(data[i]); // Send data bytes
  }

  byte error = Wire.endTransmission();

  if (error == 0)
  {
#if LOG_SHOW_I2C
    Serial.print("Successfully written ");
    Serial.print(length);
    Serial.print(" bytes of data, starting register=0x");
    Serial.println(startRegister, HEX);
#endif
    return true;
  }
  else
  {
#if LOG_SHOW_I2C
    Serial.print("Multi-byte write failed, error code: ");
    Serial.println(error);
#endif
    return false;
  }
}

/**
 * @brief Read single byte data from I2C device
 * @param devid I2C device address
 * @param registerAddress Register address to read
 * @return Data byte read from the register
 */
byte readSingleByte(byte devid, byte registerAddress)
{
  byte data = 0;

  // First write the register address to read
  Wire.beginTransmission(devid);
  Wire.write(registerAddress);
  Wire.endTransmission(false); // Send repeated start signal

  // Then read one byte of data
  Wire.requestFrom(devid, (byte)1);
  if (Wire.available())
  {
    data = Wire.read();
  }

  return data;
}

/**
 * @brief Read multiple bytes data from I2C device
 * @param devid I2C device address
 * @param startRegister Register address to start reading
 * @param buffer Pointer to buffer to store read data
 * @param length Number of bytes to read
 * @return True if read is successful, false otherwise
 */
bool readMultipleBytes(byte devid, byte startRegister, byte *buffer, size_t length)
{
  // First write the starting register address to read
  Wire.beginTransmission(devid);
  Wire.write(startRegister);
  byte error = Wire.endTransmission(false); // Send repeated start signal

  if (error != 0)
  {
#if LOG_SHOW_I2C
    Serial.print("Address setup before read failed, error code: ");
    Serial.println(error);
#endif
    return false;
  }

  // Then read multiple bytes of data
  int bytesRead = Wire.requestFrom(devid, (byte)length);

  // Fix: Cast bytesRead to size_t for comparison to avoid signedness warning
  if ((size_t)bytesRead == length)
  {
    for (size_t i = 0; i < length; i++)
    {
      if (Wire.available())
      {
        buffer[i] = Wire.read();
      }
    }
    return true;
  }
  else
  {
#if LOG_SHOW_I2C
    Serial.print("Number of bytes read doesn't match, expected: ");
    Serial.print(length);
    Serial.print(", actual: ");
    Serial.println(bytesRead);
#endif
    return false;
  }
}