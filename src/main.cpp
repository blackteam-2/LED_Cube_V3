#include	<Arduino.h>
#include	"Cube_Main.h"

int port, data;

void setup() {
	InitCube();
	Serial.begin(57600);
	}

void loop() {
	if(Serial.available()>0) {
		switch (Serial.read()){
			case 'A':
				SetData(0xFF);
				Serial.println("SetData(0xFF)");
				break;
			case 'B':
				SetData(0x00);
				Serial.println("SetData(0x00)");
				break;
			case 'C':
				SetData(0xAA);
				Serial.println("SetData(0xAA)");
				break;
			case 'P':
				delay(2);
				port = Serial.read();
				data = Serial.read();
				if((port>='0')&&(port<='7')) {
					port -= '0';
					if((data=='0')||(data=='1')) {
						if(data=='0') LatchLayer(port, false);
						else LatchLayer(port, true);
						Serial.print("Layer ");
						Serial.write(port);
						Serial.print(" Power set: ");
						Serial.write(data);
						Serial.print("\r\n");
						}
					else {
						Serial.print("Data is not valid: ");
						Serial.write(data);
						Serial.print("\r\n");
						}
					}
				else {
					Serial.print("Port is not valid: ");
					Serial.write(port);
					Serial.print("\r\n");
					}
				break;
			}
		}
	}

