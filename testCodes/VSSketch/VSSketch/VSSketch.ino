/*
 Name:		VSSketch.ino
 Created:	6/15/2017 4:01:29 PM
 Author:	34011-36-05
*/
#include "Wire.h"
#include <Arduino-MPU6050-master\MPU6050.h>

// the setup function runs once when you press reset or power the board

MPU6050 mpu;

void setup() {
	

	Serial.begin(115200);
	// Initialize MPU 
	Serial.println("Initialization MPU6050 from VS ");
	Serial.print("size of float = ");
	Serial.println(sizeof(float));
	while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
	{
		Serial.print("Cound not find a valid MPU6050"); 
	}
	mpu.calibrateGyro(); 
	mpu.setThreshold(0); 
	
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	Vector rawGyro = mpu.readRawGyro();
	Vector normGyro = mpu.readNormalizeGyro();
	Serial.print(rawGyro.XAxis);
	Serial.print('\t');
	Serial.print(rawGyro.YAxis);
	Serial.print('\t');
	Serial.print(rawGyro.ZAxis);
	
	Serial.print(normGyro.XAxis);
	Serial.print('\t');
	Serial.print(normGyro.YAxis);
	Serial.print('\t');
	Serial.print(normGyro.ZAxis);

	delay(1000);
}



void checkSettings() {
	Serial.println();

	Serial.print(" * Sleep Mode:        ");
	Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

	Serial.print(" * Clock Source:      ");
	switch (mpu.getClockSource())
	{
	case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
	case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
	case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
	case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
	case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
	case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
	case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
	}

	Serial.print(" * Gyroscope:         ");
	switch (mpu.getScale())
	{
	case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
	case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
	case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
	case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
	}

	Serial.print(" * Gyroscope offsets: ");
	Serial.print(mpu.getGyroOffsetX());
	Serial.print(" / ");
	Serial.print(mpu.getGyroOffsetY());
	Serial.print(" / ");
	Serial.println(mpu.getGyroOffsetZ());

	Serial.println();
}
