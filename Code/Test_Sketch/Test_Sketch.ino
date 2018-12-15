#include "Opcodes.h"

void setup() {
  Serial.begin(115200);
}

/*
  struct AccelData {
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t temp;
*/

uint16_t value = 0;

void writeBuf(byte* buf, size_t length) {
  for(size_t i = 0; i < length; i++) {
    Serial.write(*buf);
    buf++;
  }
}

void writeEnding() {
  for(int i = 0; i < END_BYTE_COUNT; i++) {
    Serial.write(0x4C);
  }
}

const int PACKETS_PER_SECOND = 100;
const int MICROS_PER_PACKET = 1000000 / PACKETS_PER_SECOND;

void loop() {
  long start = micros();
  AccelData data;
  data.ax = 0x4C;
  data.ay = 9;
  data.az = 81;
  
  data.gx = 729;
  data.gy = 6561;
  data.gz = 19683;
  data.temp = value++;
  Serial.write(ACCEL_DATA);
  Serial.write((byte*) &data, sizeof(data));
  writeEnding();

  long microsTaken = micros() - start;
  long sleepTime = MICROS_PER_PACKET - microsTaken;
  delayMicroseconds(sleepTime);
  
  
  
  
  
/*  
  Serial.print("Per Packet ");
  Serial.print(MS_PER_PACKET);
  Serial.print("Ms taken ");
  Serial.print(msTaken);
  Serial.print("sleeping for ");
  Serial.println(sleepTime);*/
};
