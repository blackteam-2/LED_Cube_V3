//-----------------------------------------------------------------------------
#include	"Cube_Main.h"
#include	"Cube.h"
#include	"Cube_IO.h"
#include	"Cube_Effect.h"
#include	"FastDigital.h"

/*
This is the functions that drive the LED cube and control the updating and 
refreshing of the dataArray to the Latches
*/

void	SetupCubeIO(void);
void	Setup_Timer1(uint16_t reloadVal);
void	LatchData(byte multiplex, byte data);
volatile	void	LatchLayer(byte layer, bool level);
void	SetData(byte data);
void	SetLayer(byte layer, bool level);
void	SetMuxAddy(byte addy);

#define UPDATE_0_25Hz	62500
#define UPDATE_1Hz		15625
#define UPDATE_4Hz		3906
#define UPDATE_8Hz		1953
#define	UPDATE_16HZ		977
#define	UPDATE_20HZ		781
#define	UPDATE_30HZ		521
#define	UPDATE_40HZ1	390
#define	UPDATE_100HZ	156
#define	UPDATE_40HZ		40

// Holds the current layer that is active
volatile byte curLayer = 1;


//-----------------------------------------------------------------------------
void	InitCube(void) {
	int i;

	// Set the cube data to all off
	SetAllPixelsOff();
	// Setup the IO
	SetupCubeIO();
	// Set each layers power to off
	for(i=0; i<CUBESIZE; i++)
		LatchLayer(i, false);
	// Start the cube update by enabling the timer and ISR
	Setup_Timer1(UPDATE_100HZ);
	// Enable Global Interupts
	sei();
	}
//-----------------------------------------------------------------------------
void	SetupCubeIO(void) {
	// Data Pins
	pinMode(PIN_DATA0, OUTPUT);
	pinMode(PIN_DATA1, OUTPUT);
	pinMode(PIN_DATA2, OUTPUT);
	pinMode(PIN_DATA3, OUTPUT);
	pinMode(PIN_DATA4, OUTPUT);
	pinMode(PIN_DATA5, OUTPUT);
	pinMode(PIN_DATA6, OUTPUT);
	pinMode(PIN_DATA7, OUTPUT);
	// Addy Pins
	pinMode(PIN_ADDY0, OUTPUT);
	pinMode(PIN_ADDY1, OUTPUT);
	pinMode(PIN_ADDY2, OUTPUT);
	pinMode(PIN_CS, OUTPUT);
	}
//-----------------------------------------------------------------------------
void	Setup_Timer1(uint16_t reloadVal){
	//Clear Timer 1
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCCR1C = 0x00;
	if (reloadVal == 0) { return; }
	Serial.println("Start");
	// CRC Mode, OVF_Int, Clk/1024
	TCCR1B |= (1 << WGM12);
	OCR1A = reloadVal;
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << CS12) | (CS10);
	}
//-----------------------------------------------------------------------------
// Cube Update ISR
// This controls the persistence of vision 
ISR(TIMER1_COMPA_vect) {
	byte 	temp;
	int 	a;

	//turn current layer off
	LatchLayer(curLayer, false);
	// Move to the next layer
	if (++curLayer == CUBESIZE) curLayer = 0;
	// set data latches 
	for (a=0; a<CUBESIZE; a++) {
		temp = GetAxisLine(Axis_Y, a, curLayer);
		LatchData(a, temp);
		}
	//turn current layer on
	LatchLayer(curLayer, true);
	}
//-----------------------------------------------------------------------------
void	LatchData(byte multiplex, byte data) {
	SetMuxAddy(multiplex);
	fDigitalWrite(PIN_CS, HIGH);
	SetData(data);
	//delayMicroseconds(20);
	fDigitalWrite(PIN_CS, LOW);
	}
//-----------------------------------------------------------------------------
volatile	void	LatchLayer(byte layer, bool level) {
	fDigitalWrite(PIN_LAYLTCH, HIGH);
	SetLayer(layer, level);
	//delayMicroseconds(20);
	fDigitalWrite(PIN_LAYLTCH, LOW);
	}
//-----------------------------------------------------------------------------
void	SetData(byte data) {
	fDigitalWrite(PIN_DATA0, !(HIGH && (data & 0x01)));
	fDigitalWrite(PIN_DATA1, !(HIGH && (data & 0x02)));
	fDigitalWrite(PIN_DATA2, !(HIGH && (data & 0x04)));
	fDigitalWrite(PIN_DATA3, !(HIGH && (data & 0x08)));
	fDigitalWrite(PIN_DATA4, !(HIGH && (data & 0x10)));
	fDigitalWrite(PIN_DATA5, !(HIGH && (data & 0x20)));
	fDigitalWrite(PIN_DATA6, !(HIGH && (data & 0x40)));
	fDigitalWrite(PIN_DATA7, !(HIGH && (data & 0x80)));
	}
//-----------------------------------------------------------------------------
void	SetLayer(byte layer, bool level) {
	//Se the bus to a known state, ALL OFF
	fDigitalWrite(PIN_DATA0, HIGH);
	fDigitalWrite(PIN_DATA1, HIGH);
	fDigitalWrite(PIN_DATA2, HIGH);
	fDigitalWrite(PIN_DATA3, HIGH);
	fDigitalWrite(PIN_DATA4, HIGH);
	fDigitalWrite(PIN_DATA5, HIGH);
	fDigitalWrite(PIN_DATA6, HIGH);
	fDigitalWrite(PIN_DATA7, HIGH);
	// Set the Specific layer control pin
	switch(layer) {
		case 0:
			fDigitalWrite(PIN_DATA0, level?LOW:HIGH);
			break;
		case 1:
			fDigitalWrite(PIN_DATA1, level?LOW:HIGH);
			break;
		case 2:
			fDigitalWrite(PIN_DATA2, level?LOW:HIGH);
			break;
		case 3:
			fDigitalWrite(PIN_DATA3, level?LOW:HIGH);
			break;
		case 4:
			fDigitalWrite(PIN_DATA4, level?LOW:HIGH);
			break;
		case 5:
			fDigitalWrite(PIN_DATA5, level?LOW:HIGH);
			break;
		case 6:
			fDigitalWrite(PIN_DATA6, level?LOW:HIGH);
			break;
		case 7:
			fDigitalWrite(PIN_DATA7, level?LOW:HIGH);
			break;
		case 8:
			if(level){
				fDigitalWrite(PIN_DATA0, HIGH);
				fDigitalWrite(PIN_DATA1, HIGH);
				fDigitalWrite(PIN_DATA2, HIGH);
				fDigitalWrite(PIN_DATA3, HIGH);
				fDigitalWrite(PIN_DATA4, HIGH);
				fDigitalWrite(PIN_DATA5, HIGH);
				fDigitalWrite(PIN_DATA6, HIGH);
				fDigitalWrite(PIN_DATA7, HIGH);
				}
			/* We should never turn all the layers on at once
			else{
				digitalWrite(PIN_DATA0, LOW);
				digitalWrite(PIN_DATA1, LOW);
				digitalWrite(PIN_DATA2, LOW);
				digitalWrite(PIN_DATA3, LOW);
				digitalWrite(PIN_DATA4, LOW);
				digitalWrite(PIN_DATA5, LOW);
				digitalWrite(PIN_DATA6, LOW);
				digitalWrite(PIN_DATA7, LOW);
				}
				*/
			break;
		}
	}
//-----------------------------------------------------------------------------
void	SetMuxAddy(byte addy) {
	fDigitalWrite(PIN_ADDY0, HIGH && (addy & 0x01));
	fDigitalWrite(PIN_ADDY1, HIGH && (addy & 0x02));
	fDigitalWrite(PIN_ADDY2, HIGH && (addy & 0x04));
	}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------