/*
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <Arduino.h>

#include "Opcodes.h"


#undef min
#undef max

#define MY_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MY_MAX(a,b) (((a) < (b)) ? (a) : (b))

#define BUFFER_SIZE 6 * 1024

uint8_t buffer[BUFFER_SIZE];

uint32_t pointer = 0, lastPointer = 0;

bool canWrite(uint32_t bytes) {
	return pointer + bytes < BUFFER_SIZE;
}


void writeStruct(void* data, uint32_t bytes, uint8_t id) {
	if (canWrite(bytes + 1)) {
		buffer[pointer++] = id;
		memcpy((void*)(buffer + pointer), data, bytes);
		pointer += bytes;
	} else {
		Serial.print("Buffer out of space! Pointer: ");
		Serial.print(pointer);
		Serial.print(" Bytes: ");
		Serial.println(bytes);
	}
}

uint32_t launchTime;

uint32_t lastTime = 0, lastSecond = 0, lastPacketTime = 0, nextSubPacket = UINT_MAX;
float lastAltitude = 0.0f, lastVerticalSpeed = 0.0f;
uint16_t packetCount = 0;
uint8_t subPacketCount = 0;

#define SUB_PACKETS_PER_SECOND 10
#define MS_PER_SUB_PACKET (1000 / SUB_PACKETS_PER_SECOND)

float getAltitude() {
	float time = (millis() - launchTime) / 1000.0f;
	if (time < 0) {
		return 0;
	} else if (time < 14.8) {
		return -14.623 * time * (time - 22);
	} else {
		return MY_MAX(2000 - 30 * time, 0);
	}
}

void writePacket() {
	bool send = false;//Logic to make sure we send at 1hz
	uint32_t now = millis();
	if (nextSubPacket == UINT_MAX) {//This is the first time in this loop
		nextSubPacket = now;
		lastSecond = now;
	}
	if (now - lastSecond >= 1000) {//Make sure it's signed to avoid overflow being larger than 1000
		lastSecond += 1000;
		send = true;
	} 
	if (pointer == 0) {//We are starting a new packet
		for (int i = 0; i < MAGIC_COUNT; i++) buffer[pointer++] = MAGIC_BYTE;//Write magic
		subPacketCount = 0;
		lastPacketTime = now;

		HertzData header;
		header.packetCount = packetCount++;
		header.millis = now;
		header.voltage = (float) random(0, 0xFFFF) / 0xFFFF;
		header.cameraBytes = random(1000, 1500);
		header.lat = 0.0f;
		header.lng = 0.0f;
		header.mpuTemperature = random(120, 240) / 2.0f;
		header.gpsAltitude = 9800;

		writeStruct(&header, sizeof(header), HERTZ_DATA_ID);
		//Serial.println("Maing new packet");
	}
	if (now >= nextSubPacket) {
		digitalWrite(LED_BUILTIN, HIGH);
		nextSubPacket += MS_PER_SUB_PACKET;

		//Make up numbers for now...
		float delta = (now - lastTime) / 1000.0f;
		float altitude = getAltitude();
		float verticalSpeed = (altitude - lastAltitude) / delta;
		float verticalAccerlation = (verticalSpeed - lastVerticalSpeed) / delta;

		SubPacketData subPacket;
		subPacket.subPacketCount = subPacketCount++;
		subPacket.millis = now - lastPacketTime;
		subPacket.accelerometerSpeed = verticalSpeed;
		subPacket.pitotSpeed = verticalSpeed;
		subPacket.altimeterAltitude = (uint16_t) altitude;
		subPacket.accelAcceleration = verticalAccerlation;//From ft/s^2 to G
		//subPacket.accelAcceleration = millis() / 1000.0f;

		writeStruct(&subPacket, sizeof(subPacket), SUB_PACKET_DATA_ID);

		lastTime = now;
		lastAltitude = altitude;
		lastVerticalSpeed = verticalSpeed;
		digitalWrite(LED_BUILTIN, LOW);
	}
	if (send) {
		buffer[pointer++] = END_OF_PACKET_ID;
		uint8_t checksum = 0;
		for (int i = 0; i < pointer; i++) {
			checksum += buffer[i];
		}
		buffer[pointer++] = checksum;
		checksum = 0;//Reset

		Serial.write(buffer, pointer);
		
		lastPointer = pointer;
		pointer = 0;//Clear buffer
	}
}

#include <limits.h>
#include <stdint.h>

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

#define	O32_LITTLE_ENDIAN	0x03020100ul
#define	O32_BIG_ENDIAN		0x00010203ul
#define	O32_PDP_ENDIAN		0x01000302ul

static const union { unsigned char bytes[4]; uint32_t value; } o32_host_order =
{ { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)

void setup() {
	launchTime = 5 * 1000;
	pinMode(LED_BUILTIN, OUTPUT);
	randomSeed(analogRead(0));
	Serial.begin(115200);
	Serial1.begin(115200);
	lastTime = millis();
}

void loop() {
	writePacket();
}
*/