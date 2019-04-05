#pragma once
#include <stdint.h>

#include "MyFloat.h"

#define MAGIC_BYTE 0x65
#define MAGIC_COUNT 5

#define HERTZ_DATA_ID 1
struct HertzData {//Data that is send once per second
	uint16_t packetCount;
	MyFloat<uint8_t, 4> voltage;
	MyFloat<uint8_t, 120> mpuTemperature;
	
	uint32_t millis;//The number of milliseconds since the program started
	
	uint32_t cameraBytes;//The number of bytes read from the camera since the last hertz packet
	
	float lat;//gps latitude
	float lng;//gps longitude
	
	uint16_t gpsAltitude;//In feet
	uint16_t reserved;
};

#define SUB_PACKET_DATA_ID 2
struct SubPacketData {
	SubPacketData() = default;

	uint8_t subPacketCount;
	int8_t pitotAcceleration;
	MyFloat<uint16_t, 500> pitotSpeed;// Pitot speed in raw units from 0 to 500 ft/s
	 
	uint16_t millis;// The number of millis since the last Hertz packet
	MyFloat<uint16_t, 5000> altimeterAltitude;// Altitude in feet from sea level

	MyFloat<int8_t, 90> rx, ry, rz, reserved1;// Rotation around each axis
	
	MyFloat<int16_t, 515> accelX, accelY, accelZ;// Acceleration in raw units from 0-300 ft/s^2
	MyFloat<int16_t, 500> accelerometerSpeed;// Accelerometer Derived speed in raw units from 0 to 500 ft/s
};

//Non-data control codes
#define END_OF_PACKET_ID 10
#define END_OF_STREAM_ID 11
