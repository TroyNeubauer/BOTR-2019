#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Opcodes.h"
#include <Arduino.h>

#include <math.h>
#include <cstdlib>
#include <ctime>

int randNum(int min, int max)
{
	return rand() % max + min;
}

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define BUFFER_SIZE 16 * 1024

uint8_t buffer[BUFFER_SIZE];
uint32_t pointer = 0, lastPointer = 0;

bool canWrite(uint32_t bytes) {
	return pointer + bytes < BUFFER_SIZE;
}

void writeStruct(void* data, uint32_t bytes) {
	if (canWrite(bytes)) {
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

uint32_t lastTime = 0, lastSecond = 0;
float lastAltitude = 0.0f, lastVerticalSpeed = 0.0f;
uint16_t packetCount = 0, subPacketCount = 0;

float getAltitude() {
	float time = (millis() - launchTime) / 1000.0f;
	if (time < 0) {
		return 0;
	}
	else if (time < 14.8) {
		return -14.623 * time * (time - 22);
	} else {
		return max(2000 - 30 * time, 0);
	}
}

void writeAccerlemoreterStruct() {

}

void writePacket() {
	bool send = false;//Logic to make sure we send at 1hz
	uint32_t now = millis();
	if ((int32_t) (now - lastSecond) > 1000) {//Make sure it's signed to avoid overflow being larger than 1000
		lastSecond += 1000;
		send = true;
	}
	if (pointer == 0) {//We are starting a new packet
		buffer[pointer++] = PACKET_MAGIC;//Write 9 bytes of magic
		buffer[pointer++] = PACKET_MAGIC;//2
		buffer[pointer++] = PACKET_MAGIC;//3
		buffer[pointer++] = PACKET_MAGIC;//4
		buffer[pointer++] = PACKET_MAGIC;//5
		buffer[pointer++] = PACKET_MAGIC;//6
		buffer[pointer++] = PACKET_MAGIC;//7
		buffer[pointer++] = PACKET_MAGIC;//8
		buffer[pointer++] = PACKET_MAGIC;//9
		subPacketCount = 0;

		PacketHeader header;
		header.packetCount = packetCount++;

		writeStruct(&header, sizeof(header));
	}

	//Sub header
	SubHeader subHeader;
	subHeader.millis = now;
	subHeader.subPacketCount = subPacketCount++;
	buffer[pointer++] = SUB_HEADER_DATA;
	writeStruct(&subHeader, sizeof(subHeader));

	//Make up numbers for now...
	float delta = (now - lastTime) / 1000.0f;
	float altitude = getAltitude();
	float verticalSpeed = (altitude - lastAltitude) / delta;
	float verticalAccerlation = (verticalSpeed - lastVerticalSpeed) / delta;
	accel.ay = verticalAccerlation;

	due.batteryVoltage = 3.3 - (randNum(0, 10) * millis()) / 50000.0f;
	camera.bytesSaved = randNum(506, 1535);
	pitot.airSpeed = verticalSpeed + randNum(-100, 100) / 78.43f;
	altimeter.altitude = altitude + randNum(-100, 100) / 478.43f;
	sd.bytesWritten = camera.bytesSaved;
	radio.bytesSent = lastPointer;

	lastVerticalSpeed = verticalSpeed;
	lastAltitude = altitude;
	lastTime = now;


	if (send) {
		//Send entire buffer
		lastPointer = pointer;
		pointer = 0;//Clear buffer
	}
	
}


void setup() {
	lastTime = millis();
	srand(time(0));
}

void loop() {
	delay(10);
	writePacket();
}