#pragma once
#include <stdint.h>

#include "MyFloat.h"

#define MAGIC_BYTE 0x65
#define MAGIC_COUNT 5

#define HERTZ_DATA_ID 1
struct HertzData {//Data that is send once per second
	uint16_t packetCount;
	uint16_t gpsAltitude;//In feet
	
	uint32_t millis;//The number of milliseconds since the program started
	
};

#define SUB_PACKET_DATA_ID 2
struct SubPacketData {
	SubPacketData() = default;

	uint8_t subPacketCount;
	uint8_t reserved;
	int16_t rx;
		
	int16_t ry, rz;// Rotation around each axis
	
	uint16_t millis, reserved1;// The number of millis since the last Hertz packet
	
	float altimeterPressure;// Altitude in feet from sea level
	float temperature;

	MyFloat<uint16_t, 223> pitotSpeed;// Pitot speed in raw units from 0 to 500 ft/s
	MyFloat<int16_t, 515> accelX, accelY, accelZ;// Acceleration in raw units from 0-515 ft/s^2
	
};

//Non-data control codes
#define END_OF_PACKET_ID 10
#define END_OF_STREAM_ID 11


#define SET_CURRENT_ALT 20