#include "Opcodes.h"

void setup() {
  Serial.begin(9600);
}

/*
  struct AccelData {
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t temp;
*/

byte value = 0;

void writeBuf(byte* buf, size_t length) {
  for(size_t i = 0; i < length; i++) {
    Serial.write(*buf);
    buf++;
  }
}
void loop() {

  AccelData data;
  data.ax = 0;
  data.ay = 9;
  data.az = 81;
  
  data.gx = 729;
  data.gy = 6561;
  data.gz = 19683;
  data.temp = value++;
  Serial.write((byte*) &data, sizeof(data));
  
  delay(200);
};
