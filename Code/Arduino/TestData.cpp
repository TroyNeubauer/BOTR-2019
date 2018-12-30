#include <stdint.h>
#include <limits.h>
#include <string.h>
#include "Opcodes.h"
#include <Arduino.h>


#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define BUFFER_SIZE 16 * 1024

uint8_t buffer[BUFFER_SIZE];

uint32_t pointer = 0, lastPointer = 0;

bool canWrite(uint32_t bytes) {
	return pointer + bytes < BUFFER_SIZE;
}

void writePayloadSize(uint32_t value) {//Little endian
	buffer[MAGIC_COUNT + 0] = (value >> 0) & 0xFF;
	buffer[MAGIC_COUNT + 1] = (value >> 8) & 0xFF;
	buffer[MAGIC_COUNT + 2] = (value >> 16) & 0xFF;
	buffer[MAGIC_COUNT + 3] = (value >> 24) & 0xFF;
}

void writeStruct(void* data, uint32_t bytes) {
	if (canWrite(bytes)) {
		memcpy((void*)(buffer + pointer), data, bytes);
		pointer += bytes;
	} else {
		//Serial.print("Buffer out of space! Pointer: ");
		//Serial.print(pointer);
		//Serial.print(" Bytes: ");
		//Serial.println(bytes);
	}
}

uint32_t launchTime;

uint32_t lastTime = 0, lastSecond = 0, lastPacketTime = 0, nextSubPacket = UINT32_MAX;
float lastAltitude = 0.0f, lastVerticalSpeed = 0.0f;
uint16_t packetCount = 0;
uint8_t subPacketCount = 0;

#define SUB_PACKETS_PER_SECOND 10
#define MS_PER_SUB_PACKET (1000 / SUB_PACKETS_PER_SECOND)

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

void writePacket() {
	bool send = false;//Logic to make sure we send at 1hz
	uint32_t now = millis();
	if (nextSubPacket == UINT32_MAX) {//This is the first time in this loop
		nextSubPacket = now - 1;//-1 so that we create the first one this time
		lastSecond = now;
	}
	if ((int32_t) (now - lastSecond) > 1000) {//Make sure it's signed to avoid overflow being larger than 1000
		lastSecond += 1000;
		send = true;
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN, LOW);
		//Serial.println("decided to send packet!");
	} 
	if (pointer == 0) {//We are starting a new packet
		for (int i = 0; i < MAGIC_COUNT; i++) {
			buffer[i] = PACKET_MAGIC;//Write magic
		}
		writePayloadSize(0);//0 for now... We will update this accordingly later
		pointer = MAGIC_COUNT + sizeof(uint32_t);
		subPacketCount = 0;
		lastPacketTime = now;

		HertzData header;
		header.packetCount = packetCount++;
		header.millis = now;
		header.voltage = random(40000, UINT16_MAX);
		header.cameraBytes = random(1000, 1500);
		header.lat = 0.0f;
		header.lng = 0.0f;
		header.mpuTemperature = random(30000, 40000);
		header.gpsAltitude = 9800;

		writeStruct(&header, sizeof(header));

	}
	if (now > nextSubPacket || (send && (subPacketCount < SUB_PACKETS_PER_SECOND))) {
		nextSubPacket += MS_PER_SUB_PACKET;

		//Make up numbers for now...
		float delta = (now - lastTime) / 1000.0f;
		float altitude = getAltitude();
		float verticalSpeed = (altitude - lastAltitude) / delta;
		float verticalAccerlation = (verticalSpeed - lastVerticalSpeed) / delta;

		SubPacketData subPacket = {};
		subPacket.subPacketCount = subPacketCount++;
		subPacket.millis = now - lastPacketTime;

		subPacket.accelerometerSpeed = verticalSpeed;
		subPacket.pitotSpeed = abs(verticalSpeed + random(-100, 100) / 708.43f);
		subPacket.altimeterAltitude = altitude + random(-100, 100) / 1378.43f;
		subPacket.ay = verticalAccerlation;

		writeStruct(&subPacket, sizeof(subPacket));

		lastVerticalSpeed = verticalSpeed;
		lastAltitude = altitude;
		lastTime = now;
	}
	if (send) {
		//Send entire buffer
		//Serial.print("Sending packet after");
		//Serial.print(subPacketCount);
		//Serial.println(" sub iterations...");
		//Serial.print(pointer);
		//Serial.println(" bytes");
		writePayloadSize(pointer - sizeof(uint32_t) - MAGIC_COUNT);
		
		uint32_t start = millis();
		Serial1.write((const char *) &buffer, pointer);
		Serial1.flush();
		uint32_t delta = millis() - start;
		Serial.print("BIG: ");
		Serial.println(delta);
		Serial.print("Data size ");
		Serial.println(pointer);
		Serial.print("points: ");
		Serial.println(subPacketCount);
		
		lastPointer = pointer;
		pointer = 0;//Clear buffer
	}
}


void setup() {
	launchTime = 5 * 1000;
	randomSeed(analogRead(0));
	Serial.begin(115200);
	Serial1.begin(115200);
	lastTime = millis();
}

void loop() {
	writePacket();
	delay(random(0, MS_PER_SUB_PACKET / 3));
}
