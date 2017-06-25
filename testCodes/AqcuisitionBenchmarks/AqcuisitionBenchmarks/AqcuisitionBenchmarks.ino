/*
 Name:		AqcuisitionBenchmarks.ino
 Created:	6/22/2017 4:00:35 PM
 Author:	34011-36-05
*/

// the setup function runs once when you press reset or power the board

float a = 12.5, b = 1250.14, c = -152.02;
int8_t i = 10;

void setup() {
	Serial.begin(115200);
	Serial.println("Measuring aqcuisition speed");

}

// the loop function runs over and over again until power down or reset
void loop() {

	
	while (i-- > 0) {
		unsigned long StartTime = micros();
		Serial.print(a);
		Serial.print("; ");
		Serial.print(b);
		Serial.print("; ");
		Serial.print(c);
		Serial.print("     Span: ");
		Serial.println(micros() - StartTime);
		_delay_ms(1);
	}
	_delay_ms(1000);
	i = 10;



}
