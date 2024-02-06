#include	<Arduino.h>
#include	"Cube_Main.h"
#include	"Cube_IO.h"
#include	"Cube_Effect.h"
#include	"Cube.h"

char	*VerString = "0.00.01";

int 	i, port, data;

volatile	byte	Debounce[2];
volatile 	bool 	BtnFlag[2] = {false, false};

volatile	byte	PinCTemp;
volatile	bool	TestPinPlag;
//-----------------------------------------------------------------------------
ISR(PCINT1_vect){
	// SW401
	if (((PINC&0x04)==0)&&(Debounce[0]==0)) {
		BtnFlag[0] = true;
		Debounce[0] = 20;
		}
	// SW402
	if (((PINC&0x08)==0)&&(Debounce[1]==0)) {
		BtnFlag[1] = true;
		Debounce[1] = 20;
		}
	}
//-----------------------------------------------------------------------------
void	SetupButtons(void) {
	pinMode(PIN_BUTT_LEFT, INPUT_PULLUP);
	pinMode(PIN_BUTT_RIGHT, INPUT_PULLUP);

	// Set the mask for the PCINT pins (PCINT10 + PCINT11)
	PCMSK1 = 0x0C;
	// Set The Pin Change int bank (PCINT1)
	PCICR = 0x02;
	}
//-----------------------------------------------------------------------------
void	DemoLoop(void) {
	
	}
//-----------------------------------------------------------------------------
void setup() {
	Serial.begin(57600);
	while (!Serial);

	//SetupButtons();
	InitCube();

	Serial.print("\r\nLED Cube V2 (");
	Serial.print(VerString);
	Serial.print(")\r\n");
	}
//-----------------------------------------------------------------------------
void loop() {
	//Effect_TopDown(Axis_Z, 3, true, 5, 1000);
	Effect_Rain(120, 100);
	// Check the serial port
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
			case 'D':
				digitalWrite(13, !digitalRead(13));
				break;
			case 'E':
				for(i=0;i<8;i++){
					LatchData(i, 0xFF);
					}	
				Serial.println("LatchData(0xFF)");
				break;
			case 'F':
				for(i=0;i<8;i++){
					LatchData(i, 0xAA);
					}	
				Serial.println("LatchData(0xAA)");
				break;
			case 'G':
				for(i=0;i<8;i++){
					LatchData(i, 0x00);
					}	
				Serial.println("LatchData(0x00)");
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
		/*
	// Check the xxx Btn
	if(BtnFlag[0]) {
		Serial.println("SW401");
		BtnFlag[0] = false;
		}
	// Check the xxx Btn
	if(BtnFlag[1]) {
		Serial.println("SW402");
		BtnFlag[1] = false;
		}
	// 
	if(TestPinPlag) {
		Serial.print("Debounce: ");
		Serial.write(PinCTemp);
		Serial.write("\r\n");
		TestPinPlag = false;
		}
		*/
	}// Loop()
//-----------------------------------------------------------------------------