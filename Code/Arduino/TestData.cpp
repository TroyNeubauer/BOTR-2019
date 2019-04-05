
#include <stdint.h>
#include <SPI.h>
#include <string.h>
#include <Arduino.h>
#include <limits.h>

#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_BMP280.h>

#include "Opcodes.h"

#undef min
#undef max

#define MY_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MY_MAX(a,b) (((a) < (b)) ? (a) : (b))

#define BUFFER_SIZE 6 * 1024

uint8_t buffer[BUFFER_SIZE];

uint32_t pointer = 0, lastPointer = 0;

//SdFat sd;
//SdFile serialFile;

//SdBaseFile binFile;

bool canWrite(uint32_t bytes) {
	return pointer + bytes < BUFFER_SIZE;
}

//#define SD_CS 40

#define SPI_SPEED SD_SCK_MHZ(20)

//ArduinoOutStream cout(Serial);/

/*bool openSdCard() { a                     
	if (!sd.begin(SD_CS)) {
		if (sd.card()->errorCode()) {
			cout << F("\nSD initialization failed.\n");
			cout << F("\nerrorCode: ") << hex << showbase;
			cout << int(sd.card()->errorCode());
			cout << F(", errorData: ") << int(sd.card()->errorData());
			cout << dec << noshowbase << endl;
			return false;
		}
		if (sd.vol()->fatType() == 0) {
			cout << F("Can't find a valid FAT16/FAT32 partition.\n");
			return false;
		}
		cout << F("begin failed, can't determine error type\n");
		return false;
	}
	cout << F("\nCard successfully initialized.\n");
	cout << endl;
	return true;
}*/

/*bool createSerialFile() {
	if (!serialFile.isOpen()) {
		int rand = random(0, 0xFFFF);
		char name[30];
		itoa(rand, name, HEX);
		strcat(name, "serial.dat");
		if (!serialFile.open(name, O_WRITE | O_CREAT | O_TRUNC)) {
			cout << F("\nUnable to open file") << name << endl;
			return false;
		} else {
			cout << F("\nCreated file: ") << name << endl;
		}
	}
	return true;
}*/

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

Adafruit_BMP280 bmp;
MPU6050 mpu;

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
	if (lastTime == now) return; // Wait until it is the next millisecond
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
	}
	if (now >= nextSubPacket) {
		nextSubPacket += MS_PER_SUB_PACKET;

		//Make up numbers for now...
		float delta = (now - lastTime) / 1000.0f;

		SubPacketData subPacket;
		subPacket.subPacketCount = subPacketCount++;
		subPacket.millis = now - lastPacketTime;
		subPacket.accelerometerSpeed = 0;
		subPacket.pitotSpeed = 0;
		subPacket.altimeterAltitude = static_cast<uint16_t>(bmp.readAltitude(1026.06f) * 3.281f);
		subPacket.accelX.SetInternalValue(mpu.getAccelerationX());
		subPacket.accelY.SetInternalValue(mpu.getAccelerationY());
		subPacket.accelZ.SetInternalValue(mpu.getAccelerationZ());
		
		writeStruct(&subPacket, sizeof(subPacket), SUB_PACKET_DATA_ID);

		lastTime = now;
		lastAltitude = 0;
		lastVerticalSpeed = 0;
	} if (send) {
		digitalWrite(LED_BUILTIN, HIGH);
		buffer[pointer++] = END_OF_PACKET_ID;
		uint8_t checksum = 0;
		for (int i = 0; i < pointer; i++) {
			checksum += buffer[i];
		}
		buffer[pointer++] = checksum;
		checksum = 0;//Reset

		Serial1.write(buffer, pointer);
		//Serial.write(buffer, pointer);
		
		lastPointer = pointer;
		pointer = 0;//Clear buffer
		digitalWrite(LED_BUILTIN, LOW);
	}
}

void setup() {
	randomSeed(analogRead(0));
	launchTime = 5 * 1000;
	pinMode(LED_BUILTIN, OUTPUT);
	Serial1.begin(115200);
	Serial.begin(115200);
	lastTime = millis();

	bmp.begin();
	bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
		Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
		Adafruit_BMP280::SAMPLING_X8,    /* Pressure oversampling */
		Adafruit_BMP280::FILTER_X8,      /* Filtering. */
		Adafruit_BMP280::STANDBY_MS_63); /* Standby time. */

	//while (!openSdCard())
	//	delay(500);
	//while (!createSerialFile())
	//	delay(500);

	mpu.initialize();
	mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
}

void loop() {
	writePacket();
}


/*
 * This program attempts to initialize an SD card and analyze its structure.
 */
/*
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

 // Set USE_SDIO to zero for SPI card access. 
#define USE_SDIO 0
const uint8_t SD_CHIP_SELECT = 40;

const int8_t DISABLE_CHIP_SELECT = -1;

#if USE_SDIO
// Use faster SdioCardEX
SdFatSdioEX sd;
// SdFatSdio sd;
#else // USE_SDIO
SdFat sd;
#endif  // USE_SDIO

// serial output steam
ArduinoOutStream cout(Serial);

// global for card size
uint32_t cardSize;

// global for card erase size
uint32_t eraseSize;
//------------------------------------------------------------------------------
// store error strings in flash
#define sdErrorMsg(msg) sd.errorPrint(F(msg));
//------------------------------------------------------------------------------
uint8_t cidDmp() {
	cid_t cid;
	if (!sd.card()->readCID(&cid)) {
		sdErrorMsg("readCID failed");
		return false;
	}
	cout << F("\nManufacturer ID: ");
	cout << hex << int(cid.mid) << dec << endl;
	cout << F("OEM ID: ") << cid.oid[0] << cid.oid[1] << endl;
	cout << F("Product: ");
	for (uint8_t i = 0; i < 5; i++) {
		cout << cid.pnm[i];
	}
	cout << F("\nVersion: ");
	cout << int(cid.prv_n) << '.' << int(cid.prv_m) << endl;
	cout << F("Serial number: ") << hex << cid.psn << dec << endl;
	cout << F("Manufacturing date: ");
	cout << int(cid.mdt_month) << '/';
	cout << (2000 + cid.mdt_year_low + 10 * cid.mdt_year_high) << endl;
	cout << endl;
	return true;
}
//------------------------------------------------------------------------------
uint8_t csdDmp() {
	csd_t csd;
	uint8_t eraseSingleBlock;
	if (!sd.card()->readCSD(&csd)) {
		sdErrorMsg("readCSD failed");
		return false;
	}
	if (csd.v1.csd_ver == 0) {
		eraseSingleBlock = csd.v1.erase_blk_en;
		eraseSize = (csd.v1.sector_size_high << 1) | csd.v1.sector_size_low;
	}
	else if (csd.v2.csd_ver == 1) {
		eraseSingleBlock = csd.v2.erase_blk_en;
		eraseSize = (csd.v2.sector_size_high << 1) | csd.v2.sector_size_low;
	}
	else {
		cout << F("csd version error\n");
		return false;
	}
	eraseSize++;
	cout << F("cardSize: ") << 0.000512*cardSize;
	cout << F(" MB (MB = 1,000,000 bytes)\n");

	cout << F("flashEraseSize: ") << int(eraseSize) << F(" blocks\n");
	cout << F("eraseSingleBlock: ");
	if (eraseSingleBlock) {
		cout << F("true\n");
	}
	else {
		cout << F("false\n");
	}
	return true;
}
//------------------------------------------------------------------------------
// print partition table
uint8_t partDmp() {
	mbr_t mbr;
	if (!sd.card()->readBlock(0, (uint8_t*)&mbr)) {
		sdErrorMsg("read MBR failed");
		return false;
	}
	for (uint8_t ip = 1; ip < 5; ip++) {
		part_t *pt = &mbr.part[ip - 1];
		if ((pt->boot & 0X7F) != 0 || pt->firstSector > cardSize) {
			cout << F("\nNo MBR. Assuming Super Floppy format.\n");
			return true;
		}
	}
	cout << F("\nSD Partition Table\n");
	cout << F("part,boot,type,start,length\n");
	for (uint8_t ip = 1; ip < 5; ip++) {
		part_t *pt = &mbr.part[ip - 1]; 
		cout << int(ip) << ',' << hex << int(pt->boot) << ',' << int(pt->type);
		cout << dec << ',' << pt->firstSector << ',' << pt->totalSectors << endl;
	}
	return true;
}
//------------------------------------------------------------------------------
void volDmp() {
	cout << F("\nVolume is FAT") << int(sd.vol()->fatType()) << endl;
	cout << F("blocksPerCluster: ") << int(sd.vol()->blocksPerCluster()) << endl;
	cout << F("clusterCount: ") << sd.vol()->clusterCount() << endl;
	cout << F("freeClusters: ");
	uint32_t volFree = sd.vol()->freeClusterCount();
	cout << volFree << endl;
	float fs = 0.000512*volFree*sd.vol()->blocksPerCluster();
	cout << F("freeSpace: ") << fs << F(" MB (MB = 1,000,000 bytes)\n");
	cout << F("fatStartBlock: ") << sd.vol()->fatStartBlock() << endl;
	cout << F("fatCount: ") << int(sd.vol()->fatCount()) << endl;
	cout << F("blocksPerFat: ") << sd.vol()->blocksPerFat() << endl;
	cout << F("rootDirStart: ") << sd.vol()->rootDirStart() << endl;
	cout << F("dataStartBlock: ") << sd.vol()->dataStartBlock() << endl;
	if (sd.vol()->dataStartBlock() % eraseSize) {
		cout << F("Data area is not aligned on flash erase boundaries!\n");
		cout << F("Download and use formatter from www.sdcard.org!\n");
	}
}
//------------------------------------------------------------------------------
void setup() {
	Serial.begin(115200);

	// use uppercase in hex and use 0X base prefix
	cout << uppercase << showbase << endl;

	// F stores strings in flash to save RAM
	cout << F("SdFat version: ") << SD_FAT_VERSION << endl;
#if !USE_SDIO  
	if (DISABLE_CHIP_SELECT < 0) {
		cout << F(
			"\nAssuming the SD is the only SPI device.\n"
			"Edit DISABLE_CHIP_SELECT to disable another device.\n");
	}
	else {
		cout << F("\nDisabling SPI device on pin ");
		cout << int(DISABLE_CHIP_SELECT) << endl;
		pinMode(DISABLE_CHIP_SELECT, OUTPUT);
		digitalWrite(DISABLE_CHIP_SELECT, HIGH);
	}
	cout << F("\nAssuming the SD chip select pin is: ") << int(SD_CHIP_SELECT);
	cout << F("\nEdit SD_CHIP_SELECT to change the SD chip select pin.\n");
#endif  // !USE_SDIO  
}
//------------------------------------------------------------------------------
void loop() {
	// Read any existing Serial data.
	do {
		delay(10);
	} while (Serial.available() && Serial.read() >= 0);

	uint32_t t = millis();
#if USE_SDIO
	if (!sd.cardBegin()) {
		sdErrorMsg("\ncardBegin failed");
		return;
	}
#else  // USE_SDIO
	// Initialize at the highest speed supported by the board that is
	// not over 50 MHz. Try a lower speed if SPI errors occur.
	if (!sd.cardBegin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {
		sdErrorMsg("cardBegin failed");
		return;
	}
#endif  // USE_SDIO 
	t = millis() - t;

	cardSize = sd.card()->cardSize();
	if (cardSize == 0) {
		sdErrorMsg("cardSize failed");
		return;
	}
	cout << F("\ninit time: ") << t << " ms" << endl;
	cout << F("\nCard type: ");
	switch (sd.card()->type()) {
	case SD_CARD_TYPE_SD1:
		cout << F("SD1\n");
		break;

	case SD_CARD_TYPE_SD2:
		cout << F("SD2\n");
		break;

	case SD_CARD_TYPE_SDHC:
		if (cardSize < 70000000) {
			cout << F("SDHC\n");
		}
		else {
			cout << F("SDXC\n");
		}
		break;

	default:
		cout << F("Unknown\n");
	}
	if (!cidDmp()) {
		return;
	}
	if (!csdDmp()) {
		return;
	}
	uint32_t ocr;
	if (!sd.card()->readOCR(&ocr)) {
		sdErrorMsg("\nreadOCR failed");
		return;
	}
	cout << F("OCR: ") << hex << ocr << dec << endl;
	if (!partDmp()) {
		return;
	}
	if (!sd.fsBegin()) {
		sdErrorMsg("\nFile System initialization failed.\n");
		return;
	}
	volDmp();
}
*/