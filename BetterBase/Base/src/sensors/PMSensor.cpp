#include "../inc/BaseConfig.h"

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
#include "PMSensor.h"

uint8_t SDS011::readParticleByPin(int pin)
{
    if (serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized serial mode!");
        return 0;
    }
    PRINT_DEBUG_LN("Pin is" + String(pin));
    return pulseIn(pin, HIGH, 1500000) / 1000 - 2;
}

uint8_t SDS011::get10() {
    return readParticleByPin(PINOUT_NOVA_PM_SENSOR_PWM_10);
}

uint8_t SDS011::get25() {
    return readParticleByPin(PINOUT_NOVA_PM_SENSOR_PWM_25);
}

#define SLEEPCMD SDS011::constructCMD(0x06, 0x01, 0x00)

#define WAKECMD SDS011::constructCMD(0x06, 0x01, 0x01)

#define QUERY_MODE SDS011::constructCMD(0x02, 0x00, 0x00)

#define REPORTING_MODE SDS011::constructCMD(0x02, 0x01, 0x00)

const byte *SDS011::constructCMD(
	byte command,
	byte data2,
	byte data3)
{

	return constructCMD(command, data2, data3, 0xFFFF);
}

const byte *SDS011::constructCMD(byte command, byte data2, byte data3, int deviceID)
{

	static byte cmd[19] = {
		0xAA, // head
		0xB4, // command id
		0xFF, // data byte 1
		0xFF, // data byte 2 (set mode)
		0x00, // data byte 3 (manual)
		0x00, // data byte 4
		0x00, // data byte 5
		0x00, // data byte 6
		0x00, // data byte 7
		0x00, // data byte 8
		0x00, // data byte 9
		0x00, // data byte 10
		0x00, // data byte 11
		0x00, // data byte 12
		0x00, // data byte 13
		0xFF, // data byte 14 (device id byte 1)
		0xFF, // data byte 15 (device id byte 2)
		0xFF, // checksum
		0xAB  // tail
	};

	cmd[2] = command;
	cmd[3] = data2;
	cmd[4] = data3;
	cmd[15] = deviceID >> 8;
	cmd[16] = deviceID & 0xFF;
	cmd[17] = (command + data2 + data3 + cmd[15] + cmd[16]) & 0xFF;
	return cmd;
}

void SDS011::sendCMD(const byte *cmd)
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	DEBUG_PRINT("Sending:\t\t");
	for (byte i = 0; i < 19; i++)
	{
		sds_data.write(cmd[i]);
		DEBUG_PRINT_HEX(cmd[i]);
		DEBUG_PRINT(" ");
	}
}

void SDS011::waitAndAnswer()
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	DEBUG_PRINTLN("\nSDS Answers:");
	sds_data.flush();
	while (sds_data.available() <= 0)
	{
		yield();
	}
	while (sds_data.available() > 0)
	{
		DEBUG_PRINT_HEX(sds_data.read());
		DEBUG_PRINT(" ");
	}
	DEBUG_PRINTLN();
}

int SDS011::process(int db1)
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	byte buffer;
	int value;
	int len = 0;
	int data2 = 0;
	int data3 = 0;
	int data4 = 0;
	int deviceHigh = 0;
	int deviceLow = 0;
	int checksum_is = 0;
	int checksum_ok = 0;
	int error = 1;
	int checksum_should = 0;
	DEBUG_PRINT("\nSDS Answers:\t\t");
	sds_data.flush();
	long now = millis();
	while (sds_data.available() < 10)
	{
		yield();
		if (millis() - now > 500)
		{
			Serial.println("Fucked up");
			return 0;
		}
		
	}
	while ((sds_data.available() > 0) && (sds_data.available() >= (10 - len)))
	{
		buffer = sds_data.read();
		value = int(buffer);
		switch (len)
		{
		case (0): // HEADER
			if (value != 0xAA)
			{
				//checksum_is += 0xF1;
				//checksum_ok = 0;
				len = -1;
				DEBUG_PRINT("X");
			};
			break;
		case (1): // COMMAND ID
			if (value != 0xC5)
			{
				len = -1;
				DEBUG_PRINT("XX");
			};
			break;
		case (2): // COMMAND
			if (value != db1)
			{
				len = -1;
				DEBUG_PRINT("XXX");
			}
			else
			{
				checksum_is += value;
			}
			break;
		case (3): // DATA 2
			data2 = value;
			checksum_is += value;
			break;
		case (4): // DATA 3
			data3 = value;
			checksum_is += value;
			break;
		case (5): // DATA 4
			data4 = value;
			checksum_is += value;
			break;
		case (6): // DEVICE ID
			deviceHigh = value;
			checksum_is += value;
			break;
		case (7): // DEVICE ID
			deviceLow = value;
			checksum_is += value;
			break;
		case (8):
			checksum_should = value;
			if (value == (checksum_is & 0xFF))
			{
				checksum_ok = 1;
			}
			else
			{
				len = -1;
			};
			break;
		case (9):
			if (value != 171)
			{
				len = -1;
			};
			break;
		}
		DEBUG_PRINT_HEX(value);
		DEBUG_PRINT(" ");
		len++;

		if (len == 10 && (checksum_ok == 1 || checksum_is == checksum_should))
		{
			len = 0;
			error = 0;
			break;
		}

		if (len == 10 && checksum_is != checksum_should)
		{
			error = 1;
			checksum_ok = 0;
			break;
		}
		yield();
	}
	DEBUG_PRINTLN("\n--------------------------");
	DEBUG_PRINT("Response was ");
	if (error > 0)
	{
		DEBUG_PRINTLN("faulty");
		DEBUG_PRINT("Checksum Expected:\t");
		DEBUG_PRINTLN_HEX((checksum_is & 0xFF));
		DEBUG_PRINT("Checksum Is:\t\t");
		DEBUG_PRINTLN_HEX((checksum_should & 0xFF));
	}
	else
	{
		DEBUG_PRINTLN("ok");
	}

	DEBUG_PRINT("Data: ");
	DEBUG_PRINT_HEX(data2);
	DEBUG_PRINT(" ");
	DEBUG_PRINT_HEX(data3);
	DEBUG_PRINT(" ");
	DEBUG_PRINTLN_HEX(data4);

	DEBUG_PRINT("Device ID is ");
	DEBUG_PRINT_HEX(deviceHigh);
	DEBUG_PRINTLN_HEX(deviceLow);
	DEBUG_PRINTLN("--------------------------");
	return error;
}

void SDS011::printMode()
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	DEBUG_PRINTLN("QUERY MODE");
	sendCMD(QUERY_MODE);
	process(QUERY_MODE[2]);
}

void SDS011::autoMode()
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	DEBUG_PRINTLN("SET AUTO MODE");
	sendCMD(REPORTING_MODE);
	process(REPORTING_MODE[2]);
}
// --------------------------------------------------------
// SDS011:read
// --------------------------------------------------------
int SDS011::read(float *p25, float *p10)
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	byte buffer;
	int value;
	int len = 0;
	int pm10_serial = 0;
	int pm25_serial = 0;
	int checksum_is;
	int checksum_ok = 0;
	int error = 1;
	while ((sds_data.available() > 0) && (sds_data.available() >= (10 - len)))
	{
		buffer = sds_data.read();
		DEBUG_PRINT_HEX(buffer);
		DEBUG_PRINT(" ");
		value = int(buffer);
		switch (len)
		{
		case (0):
			if (value != 170)
			{
				len = -1;
			};
			break;
		case (1):
			if (value != 192)
			{
				len = -1;
			};
			break;
		case (2):
			pm25_serial = value;
			checksum_is = value;
			break;
		case (3):
			pm25_serial += (value << 8);
			checksum_is += value;
			break;
		case (4):
			pm10_serial = value;
			checksum_is += value;
			break;
		case (5):
			pm10_serial += (value << 8);
			checksum_is += value;
			break;
		case (6):
			checksum_is += value;
			break;
		case (7):
			checksum_is += value;
			break;
		case (8):
			if (value == (checksum_is % 256))
			{
				checksum_ok = 1;
			}
			else
			{
				len = -1;
			};
			break;
		case (9):
			if (value != 171)
			{
				len = -1;
			};
			break;
		}
		len++;
		if (len == 10 && checksum_ok == 1)
		{
			*p10 = (float)pm10_serial / 10.0;
			*p25 = (float)pm25_serial / 10.0;
			len = 0;
			checksum_ok = 0;
			pm10_serial = 0.0;
			pm25_serial = 0.0;
			checksum_is = 0;
			error = 0;
			DEBUG_PRINTLN("");
		}
		yield();
	}
	return error;
}

// --------------------------------------------------------
// SDS011:sleep
// --------------------------------------------------------
void SDS011::sleep()
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	DEBUG_PRINTLN("SET SLEEP");
	sendCMD(SLEEPCMD);
	sds_data.flush();
	while (sds_data.available())
	{
		sds_data.read();
	}
}

// --------------------------------------------------------
// SDS011:wakeup
// --------------------------------------------------------
void SDS011::wakeup()
{
    if (!serialMode) {
        PRINT_DEBUG_LN("SDS011: Wrong method used! You initialized PWM mode!");
        return;
    }
	DEBUG_PRINTLN("WAKE UP");
	sendCMD(WAKECMD);
	while (process(WAKECMD[2]))
	{
		delay(50);
		sendCMD(WAKECMD);
	}
}

#endif