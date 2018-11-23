#include <stdio.h>
#include "Opcodes.h"

int main() {
	FILE* file = fopen("out.dat", "wb");
	GPSData data;

	data.lat = 100;
	data.lng = -100;
	data.altitude = 6969;
	
	data.speed = 199999;
	data.heading = 0;
	data.satellites = 1;
	data.epochTime = 69696969;
	
	fputc(GPS_DATA, file);
	fwrite(&data, sizeof(data), 1, file);
	fputc(0xFF, file);
	fputc(0xFF, file);
	fputc(0xFF, file);
	fputc(0xFF, file);


	fclose(file);
}