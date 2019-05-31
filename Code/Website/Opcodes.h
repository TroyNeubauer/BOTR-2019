#pragma once
#include <stdint.h>

#define PACKET_MAGIC 0x65
#define MAGIC_COUNT 5

//24 bytes
#define HERTZ_DATA 1
struct HertzData {//Data that is send once per second
	uint16_t packetCount;
	uint16_t voltage;//Mapped between 3.3 and 0
	uint32_t millis;//The number of milliseconds since the program started
	uint32_t cameraBytes;//The number of bytes read from the camera since the last hertz packet
	float lat;//gps latitude
	float lng;//gps longitude
	uint16_t mpuTemperature;//In Fahrenheit mapped between between 0 degrees and 120 degrees
	uint16_t gpsAltitude;//In feet
} __attribute__((packed, aligned(1)));

//21 bytes
#define SUB_PACKET_DATA 2
struct SubPacketData {
	uint8_t subPacketCount;
	int8_t pitotAcceleration;
	uint16_t millis;//The number of millis since the last Hertz packet
	int16_t rvx, rvy, rvz;	//Rate of rotation around each axis
	int16_t ax, ay, az;//Acceleration
	uint16_t altimeterAltitude;//Altitude in feet from sea level
	uint16_t pitotSpeed;
	uint8_t accelerometerSpeed;//Accelerometer Derived speed in raw units from 0 to 500 ft/s
} __attribute__((packed, aligned(1)));
//3 DONE Sensors must be included to measure current altitude.
//4 DONE Accelerometer must be included to measure speed and acceleration.
//5 Telemetry must include a DONE(timestamp with sufficient resolution), DONE(current altitude),
//DONE(accelerometer derived speed and acceleration), DONE(pitot tube derived speed and acceleration).
