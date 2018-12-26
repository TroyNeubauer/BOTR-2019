#pragma once
#include <stdint.h>

#define NORMAL_STATUS					0
#define ERROR_STATUS_BAD_DATA			1
#define ERROR_STATUS_WRONG_DEVICE		2
#define ERROR_STATUS_FAILED_TO_CONNECT	3
/*
struct SystemStatus {
	uint8_t generalStatus;
};*/


#define ARDUINO_DUE_DATA	1
struct ArduinoDueData {
	float batteryVoltage;
};
/*
struct ArduinoDueStatus : SystemStatus {
	uint32_t clockSpeed;
	uint8_t lowPowerMode;
};*/

#define CAMERA_DATA			2
struct CameraData {
	uint32_t bytesSaved;//The number of bytes recorded since the last packet
};

#define ACCEL_DATA			3
struct AccelData {
	int16_t ax, ay, az;
	int16_t temp;
};

#define GPS_DATA			4
//GPS data will be a null terminated NMEA string

#define PITOT_TUBE_DATA		5
struct PitotTubeData
{
	uint16_t airSpeed;
};

#define ALTIMETER_DATA		6
struct AltimeterData
{
	float pascals, altitude, temp;
};

#define SD_CARD_DATA		7
struct SDCardData
{
	uint32_t bytesWritten;//the number of bytes written to the sd card since the last packet
};

#define RADIO_DATA			8
struct RadioData
{
	uint32_t bytesSent;//the number of bytes sent since the last packet
};

