/*#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>

NMEAGPS  gps;

//------------------------------------------------------------
//  Define a set of GPS fix information.  It will
//  hold on to the various pieces as they are received from
//  an RMC sentence.  It can be used anywhere in your sketch.

static gps_fix  fix;

//----------------------------------------------------------------
//  This function gets called about once per second, during the GPS
//  quiet time.  It's the best place to do anything that might take
//  a while: print a bunch of things, write to SD, send an SMS, etc.
//
//  By doing the "hard" work during the quiet time, the CPU can get back to
//  reading the GPS chars as they come in, so that no chars are lost.

static void doSomeWork()
{
	// Print all the things!

	trace_all(DEBUG_PORT, gps, fix);

} // doSomeWork

  //------------------------------------
  //  This is the main GPS parsing loop.

void GPSloop()
{
	while (gps.available(gpsPort)) {
		fix = gps.read();
	}

} // GPSloop

  //--------------------------

void setup()
{
	DEBUG_PORT.begin(115200);
	while (!DEBUG_PORT);

	DEBUG_PORT.print(F("NMEA.INO: started\n"));
	DEBUG_PORT.print(F("  fix object size = "));
	DEBUG_PORT.println(sizeof(gps.fix()));
	DEBUG_PORT.print(F("  gps object size = "));
	DEBUG_PORT.println(sizeof(gps));
	DEBUG_PORT.println(F("Looking for GPS device on " GPS_PORT_NAME));


	if (gps.merging == NMEAGPS::NO_MERGING) {
		DEBUG_PORT.print(F("\nWARNING: displaying data from "));
		DEBUG_PORT.print(gps.string_for(LAST_SENTENCE_IN_INTERVAL));
		DEBUG_PORT.print(F(" sentences ONLY, and only if "));
		DEBUG_PORT.print(gps.string_for(LAST_SENTENCE_IN_INTERVAL));
		DEBUG_PORT.println(F(" is enabled.\n"
			"  Other sentences may be parsed, but their data will not be displayed."));
	}

	DEBUG_PORT.print(F("\nGPS quiet time is assumed to begin after a "));
	DEBUG_PORT.print(gps.string_for(LAST_SENTENCE_IN_INTERVAL));
	DEBUG_PORT.println(F(" sentence is received.\n"
		"  You should confirm this with NMEAorder.ino\n"));

	trace_header(DEBUG_PORT);
	DEBUG_PORT.flush();

	gpsPort.begin(9600);
}

//--------------------------

void loop()
{
	GPSloop();
}

*/

//#include <MPU6050/MPU6050.h>
#include "SparkFunMPL3115A2.h"

/*
 MPL3115A2 Barometric Pressure Sensor Library Example Code
 By: Nathan Seidle
 SparkFun Electronics
 Date: September 24th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 Uses the MPL3115A2 library to display the current altitude and temperature

 Hardware Connections (Breakoutboard to Arduino):
 -VCC = 3.3V
 -SDA = A4 (use inline 10k resistor if your board is 5V)
 -SCL = A5 (use inline 10k resistor if your board is 5V)
 -INT pins can be left unconnected for this demo

 During testing, GPS with 9 satellites reported 5393ft, sensor reported 5360ft (delta of 33ft). Very close!
 During testing, GPS with 8 satellites reported 1031ft, sensor reported 1021ft (delta of 10ft).

 Available functions:
 .begin() Gets sensor on the I2C bus.
 .readAltitude() Returns float with meters above sealevel. Ex: 1638.94
 .readAltitudeFt() Returns float with feet above sealevel. Ex: 5376.68
 .readPressure() Returns float with barometric pressure in Pa. Ex: 83351.25
 .readTemp() Returns float with current temperature in Celsius. Ex: 23.37
 .readTempF() Returns float with current temperature in Fahrenheit. Ex: 73.96
 .setModeBarometer() Puts the sensor into Pascal measurement mode.
 .setModeAltimeter() Puts the sensor into altimetery mode.
 .setModeStandy() Puts the sensor into Standby mode. Required when changing CTRL1 register.
 .setModeActive() Start taking measurements!
 .setOversampleRate(byte) Sets the # of samples from 1 to 128. See datasheet.
 .enableEventFlags() Sets the fundamental event flags. Required during setup.

 */
/*
#include <Wire.h>
#include "SparkFunMPL3115A2.h"

 //Create an instance of the object
MPL3115A2 myPressure;

void setup()
{
	Serial.begin(115200);  // Start serial for output
	Serial.println("Start");
	Wire.begin();        // Join i2c bus

	myPressure.begin(); // Get sensor online

	//Configure the sensor
	myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
	//myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

	myPressure.setOversampleRate(4); // Set Oversample to the recommended 128
	myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
	Serial.println("Test");
}

void loop()
{
	//float altitude = myPressure.readAltitude();
	//Serial.print("Altitude(m):");
	//Serial.print(altitude, 2);

	float altitude = myPressure.readAltitudeFt();
	Serial.print(" Altitude(ft):");
	Serial.print(altitude, 2);

	//float pressure = myPressure.readPressure();
	//Serial.print("Pressure(Pa):");
	//Serial.print(pressure, 2);

	//float temperature = myPressure.readTemp();
	//Serial.print(" Temp(c):");
	//Serial.print(temperature, 2);

	float temperature = myPressure.readTempF();
	Serial.print(" Temp(f):");
	Serial.print(temperature, 2);

	Serial.println();
}


/*
#include <Wire.h>

void setup() {
	Serial.begin(115200); //*****  make sure serial monitor baud matches *****

	// Leonardo: wait for serial port to connect
	while (!Serial)
	{
	}

	
}  // end of setup

void loop() {
	Serial.println();
	Serial.println("I2C scanner. Scanning ...");
	byte count = 0;

	Wire.begin();
	for (byte i = 1; i < 120; i++)
	{
		Wire.beginTransmission(i);
		if (Wire.endTransmission() == 0)
		{
			Serial.print("Found address: ");
			Serial.print(i, DEC);
			Serial.print(" (0x");
			Serial.print(i, HEX);
			Serial.println(")");
			count++;
			delay(1);  // maybe unneeded?
		} // end of good response
	} // end of for loop
	Serial.println("Done.");
	Serial.print("Found ");
	Serial.print(count, DEC);
	Serial.println(" device(s).");
	delay(1000);
}*/

const int basePin = 42, pinLength = 5;

void setup() {
	for (int i = basePin; i < pinLength; i++) {
		pinMode(i, OUTPUT);
	}
	Serial.begin(115200);
	Serial.println("INIT!");
}

void loop() {
	for (int i = basePin; i < basePin + pinLength; i++) {
		if (i == basePin) {
			digitalWrite(basePin + pinLength - 1, LOW);
		} else {
			digitalWrite(i - 1, LOW);
		}
		digitalWrite(i, HIGH);
		Serial.println(i);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
	}
}