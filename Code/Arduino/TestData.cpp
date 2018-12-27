#include <stdint.h>
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

ArduinoDueData lDueData = {};
CameraData lCameraData = {};
PitotTubeData lPitotTubeData = {};
AccelData lAccelData = {};
SDCardData lSDCardData = {};
RadioData lRadioData = {};


void writePacket() {
	bool send = false;//Logic to make sure we send at 1hz
	uint32_t now = millis();
	if ((int32_t) (now - lastSecond) > 1000) {//Make sure it's signed to avoid overflow being larger than 1000
		lastSecond += 1000;
		send = true;
		digitalWrite(LED_BUILTIN, HIGH);
		//Serial.println("decided to send packet!");
	} else {
		digitalWrite(LED_BUILTIN, LOW);
	}
	if (pointer == 0) {//We are starting a new packet
		for (int i = 0; i < MAGIC_COUNT; i++) {
			buffer[i] = PACKET_MAGIC;//Write magic
		}
		writePayloadSize(0);//0 for now... We will update this accordingly later
		pointer = MAGIC_COUNT + sizeof(uint32_t);
		subPacketCount = 0;

		PacketHeaderData header;
		header.packetCount = packetCount++;
		header.millis = now;

		buffer[pointer++] = PACKET_HEADER_DATA;
		writeStruct(&header, sizeof(header));
	}

	//Sub header
	SubHeaderData subHeader;
	subHeader.millis = now;
	subHeader.subPacketCount = subPacketCount++;
	buffer[pointer++] = SUB_HEADER_DATA;
	writeStruct(&subHeader, sizeof(subHeader));

	//Make up numbers for now...
	float delta = (now - lastTime) / 1000.0f;
	float altitude = getAltitude();
	float verticalSpeed = (altitude - lastAltitude) / delta;
	float verticalAccerlation = (verticalSpeed - lastVerticalSpeed) / delta;

	//Serial.println("Iteration");
	AccelData accel = {};
	accel.ay = verticalAccerlation;
	buffer[pointer++] = ACCEL_DATA;
	writeStruct(&accel, sizeof(accel));
	
	ArduinoDueData due = {};
	due.batteryVoltage = 3.3 - (random(0, 10) * millis()) / 50000.0f;
	buffer[pointer++] = ARDUINO_DUE_DATA;
	writeStruct(&due, sizeof(due));

	CameraData camera = {};
	camera.bytesSaved = random(506, 1535);
	buffer[pointer++] = CAMERA_DATA;
	writeStruct(&camera, sizeof(camera));

	PitotTubeData pitot = {};
	pitot.airSpeed = verticalSpeed + random(-100, 100) / 78.43f;
	buffer[pointer++] = PITOT_TUBE_DATA;
	writeStruct(&pitot, sizeof(pitot));

	AltimeterData altimeter = {};
	altimeter.altitude = altitude + random(-100, 100) / 478.43f;
	buffer[pointer++] = ALTIMETER_DATA;
	writeStruct(&altimeter, sizeof(altimeter));

	SDCardData sd = {};
	sd.bytesWritten = camera.bytesSaved;
	buffer[pointer++] = SD_CARD_DATA;
	writeStruct(&sd, sizeof(sd));

	RadioData radio = {};
	radio.bytesSent = lastPointer;
	buffer[pointer++] = RADIO_DATA;
	writeStruct(&radio, sizeof(radio));

	lastVerticalSpeed = verticalSpeed;
	lastAltitude = altitude;
	lastTime = now;

	if (send) {
		//Send entire buffer
		//Serial.print("Sending packet after");
		//Serial.print(subPacketCount);
		//Serial.println(" sub iterations...");
		//Serial.print(pointer);
		//Serial.println(" bytes");
		writePayloadSize(pointer - sizeof(uint32_t) - MAGIC_COUNT);
		Serial.write((const char *) &buffer, pointer);
		
		lastPointer = pointer;
		pointer = 0;//Clear buffer
	}
}


void setup() {
	lastTime = millis();
	randomSeed(analogRead(0));
	Serial.begin(115200);
}

void loop() {
	if (packetCount <= 100) {
		delay(random(0, 100));
		writePacket();
	}
}
