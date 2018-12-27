#pragma once
#include <stdint.h>

#define PACKET_MAGIC 0x65
#define MAGIC_COUNT 13

#define PACKET_HEADER_DATA	0
struct PacketHeaderData
{
	uint16_t packetCount;
	uint32_t millis;
} __attribute__((packed));

#define SUB_HEADER_DATA		1
struct SubHeaderData
{
	uint16_t subPacketCount;
	uint32_t millis;
} __attribute__((packed));

#define ARDUINO_DUE_DATA	2
struct ArduinoDueData 
{
	float batteryVoltage;
} __attribute__((packed));
/*
struct ArduinoDueStatus : SystemStatus {
	uint32_t clockSpeed;
	uint8_t lowPowerMode;
} __attribute__((packed));*/

#define CAMERA_DATA			3
struct CameraData 
{
	uint32_t bytesSaved;//The number of bytes recorded since the last packet
} __attribute__((packed));

#define ACCEL_DATA			4
struct AccelData 
{
	int16_t rvx, rvy, rvz;	//Rate of rotation around each axis
	int16_t ax, ay, az;		//Acceleration
	float vx, vy, vz;		//Velocity
	int16_t temperature;
} __attribute__((packed));

#define GPS_DATA			5
//GPS data will be a null terminated NMEA string

#define PITOT_TUBE_DATA		6
struct PitotTubeData
{
	float acceleration;
	uint16_t airSpeed;
} __attribute__((packed));

#define ALTIMETER_DATA		7
struct AltimeterData
{
	float pascals, altitude, temp;
} __attribute__((packed));

#define SD_CARD_DATA		8
struct SDCardData
{
	uint32_t bytesWritten;//the number of bytes written to the sd card since the last packet
} __attribute__((packed));

#define RADIO_DATA			9
struct RadioData
{
	uint32_t bytesSent;//the number of bytes sent since the last packet
} __attribute__((packed));
