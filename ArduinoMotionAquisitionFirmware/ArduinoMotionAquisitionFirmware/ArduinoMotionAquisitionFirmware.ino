/*
 Name:		ArduinoMotionAquisitionFirmware.ino
 Created:	6/26/2017 4:01:00 PM
 Author:	34011-36-05

 description:
 this is a code for interfacing the MPU6050 senser from a
 a C# application. 
 */


#include <Wire.h>
#include <Arduino-MPU6050-master\MPU6050.h>

// represents a handle to MPU sensor 
MPU6050 mpu; 

/** The aqcuisition of measurement is triggered by a received command 
 from the PC. Supported commands are 
 s: for stop aqcuisition 
 t: send temperature 
 a: send acceleration
 g: send gyro measurements 


 each data sent will be wrapped in a xml like element so to 
 delimite data of the same kind: 
 exemple a packet of tempreture mesurements well be sent in the 
 form <t> ... </t>
 */

// Globals 
char currCommand = 's';		// Current command that will be executed 
char prevCommand = 's';		// previous command 
char delimiter = ';';		// represents the delimiter that is used for 
							// separating successive data
char isFirstPacket = false; // used to determine if the packet being sent 
							// is the first packet in the current aquisition 
							// (for the same type of aqcuisition )

int8_t nTrials = 0;		// used to count the number of unrecognized commands 
						// received from PC. This may serve as an indicator 
						// of bad communication 
int8_t maxTrials = 5;	// max number of trials: when reached, we send  
						// a closing tag for the current aquisition



// the setup function runs once when you press reset or power the board
void setup() {
	//  initialize serial communication to the given baud rate
	Serial.begin(115200);

	// Intialize MPU6050 
	mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
	// If you want, you can set gyroscope offsets
	// mpu.setGyroOffsetX(155);
	// mpu.setGyroOffsetY(15);
	// mpu.setGyroOffsetZ(15);

	// Calibrate gyroscope. The calibration must be at rest.
	// If you don't want calibrate, comment this line.
	mpu.calibrateGyro();

	// Set threshold sensivty. Default 3.
	// If you don't want use threshold, comment this line or set 0.
	mpu.setThreshold(3);

}

// the loop function runs over and over again until power down or reset
void loop() {

	// is there data available in the serial buffer 
	while (Serial.available() > 0) {
		currCommand = Serial.read();
		switch (currCommand) {
		case 't':
			// send temperature, and set the previous command 
			// variable to the actual command used to decide whether to 
			// append </t> or not for the current transaction 
			sendTemperatureData();
			prevCommand = currCommand;
			break;
		case 'a':
			// send acceleration 
			sendAccelerationData();
			prevCommand = currCommand;
		case 'g':
			// send gyro 
			sendGyroData();
			prevCommand = currCommand;
			break;
		case 's':
			// close the tag of the previous transaction 
			closeCurrentDataTag();
			prevCommand = 's';
		default:
			// unknown command, increment trials and pass to the next
			if (++nTrials > maxTrials) {
				closeCurrentDataTag();
				prevCommand = 's';
			}
			break;
		}

	}
}

// Functions 

// sendTemperatureData: sends temperature data, separated by 'delimiter'
//  and opens/close the appropriate tags
void sendTemperatureData(void) {

	// is it the beginning of a transition 
	// print <t>
	if (currCommand != prevCommand) {

		// close the tag for the previous command
		// if it was a valid command 
		switch (prevCommand) {
		case 'a': // acceleration  
			Serial.print("</a>");
			Serial.print("<t>");
			break;
		case 'g': // gyro 
			Serial.print("</g>");
			Serial.print("<t>");
			break;
		default: // it was a stop or unknown command, print a starting tag for <t>  
			Serial.print("<t>");
			break;
		}
	}

	// Print temperature data, separated by delimiter
	for (int8_t i = 0; i < 10; i++) {
		Serial.print(i);
		Serial.print(delimiter);
	}
}


// send acceleration data 
void sendAccelerationData(void) {
	// is it the beginning of a transition 
	// print <a>
	if (currCommand != prevCommand) {

		// close the tag for the previous command
		// if it was a valid command 
		switch (prevCommand) {
		case 't': // temperature  
			Serial.print("</t>");
			Serial.print("<a>");
			break;
		case 'g': // gyro 
			Serial.print("</g>");
			Serial.print("<a>");
			break;
		default: // it was a stop or unknown command, print a starting tag for <t>  
			Serial.print("<a>");
			break;
		}
	}

	// Print temperature data, separated by delimiter
	for (int8_t i = 0; i < 10; i++) {
		Serial.print(i*1.5);
		Serial.print(delimiter);
		Serial.print(i*1.5);
		Serial.print(delimiter);
		Serial.print(i*1.5);
		Serial.print(delimiter);
	}
}

// send Gyro data 
void sendGyroData(void) {
	// is it the beginning of a transition 
	// print <g>
	if (currCommand != prevCommand) {

		// close the tag for the previous command
		// if it was a valid command 
		switch (prevCommand) {
		case 't': // temperature  
			Serial.print("</t>");
			Serial.print("<g>");
			break;
		case 'a': // acceleration 
			Serial.print("</a>");
			Serial.print("<g>");
			break;
		default: // it was a stop or unknown command, print a starting tag for <t>  
			Serial.print("<g>");
			break;
		}
	}

	// Print temperature data, separated by delimiter
	for (int8_t i = 0; i < 10; i++) {
		Serial.print(i*1.5);
		Serial.print(delimiter);
		Serial.print(i*1.5);
		Serial.print(delimiter);
		Serial.print(i*1.5);
		Serial.print(delimiter);
	}
}


void closeCurrentDataTag(void) {
	switch (prevCommand) {
	case 't':
		Serial.print("</t>");
		return;
	case 'a':
		Serial.print("</a>");
		return;
	case 'g':
		Serial.print("</g>");
		return;
	default:
		return;
	}
}
