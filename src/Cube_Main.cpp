//-----------------------------------------------------------------------------
#include	"Cube_Main.h"
#include	"Cube.h"
#include	"Cube_IO.h"
#include	"Cube_Effect.h"

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
#define	UPDATE_40HZ		40

// Data Array to hold the text info for itterating around the path
volatile byte textPath[TEXTPATHLEN] = {0};

// Holds the current layer that is active
volatile byte curLayer = 1;
volatile byte prevLayer = 0;


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
	Setup_Timer1(UPDATE_20HZ);

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
	// CRC Mode, OVF_Int, Clk/64
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

	//turn previous layer off
	prevLayer = curLayer;
	LatchLayer(prevLayer, true);
	
	if (++curLayer == 7) curLayer = 0;

	//set data latches 
	for (a=0; a<CUBESIZE-1; a++) {
		temp = GetAxisLine(Axis_Y, a, curLayer);
		LatchData(a, temp);
		}
	//turn current layer on
	LatchLayer(curLayer, false);

	if(Debounce[0]>0) Debounce[0]--;
	if(Debounce[1]>0) Debounce[1]--;
	}
//-----------------------------------------------------------------------------
void	LatchData(byte multiplex, byte data) {
	SetMuxAddy(multiplex);
	digitalWrite(PIN_CS, HIGH);
	SetData(data);
	digitalWrite(PIN_CS, LOW);
	}
//-----------------------------------------------------------------------------
volatile	void	LatchLayer(byte layer, bool level) {
	digitalWrite(PIN_LAYLTCH, HIGH);
	SetLayer(layer, level);
	delayMicroseconds(2);
	digitalWrite(PIN_LAYLTCH, LOW);
	}
//-----------------------------------------------------------------------------
void	SetData(byte data) {
	digitalWrite(PIN_DATA0, !(HIGH && (data & 0x01)));
	digitalWrite(PIN_DATA1, !(HIGH && (data & 0x02)));
	digitalWrite(PIN_DATA2, !(HIGH && (data & 0x04)));
	digitalWrite(PIN_DATA3, !(HIGH && (data & 0x08)));
	digitalWrite(PIN_DATA4, !(HIGH && (data & 0x10)));
	digitalWrite(PIN_DATA5, !(HIGH && (data & 0x20)));
	digitalWrite(PIN_DATA6, !(HIGH && (data & 0x40)));
	digitalWrite(PIN_DATA7, !(HIGH && (data & 0x80)));
	}
//-----------------------------------------------------------------------------
void	SetLayer(byte layer, bool level) {
	switch(layer) {
		case 0:
			digitalWrite(PIN_DATA0, level?LOW:HIGH);
			break;
		case 1:
			digitalWrite(PIN_DATA1, level?LOW:HIGH);
			break;
		case 2:
			digitalWrite(PIN_DATA2, level?LOW:HIGH);
			break;
		case 3:
			digitalWrite(PIN_DATA3, level?LOW:HIGH);
			break;
		case 4:
			digitalWrite(PIN_DATA4, level?LOW:HIGH);
			break;
		case 5:
			digitalWrite(PIN_DATA5, level?LOW:HIGH);
			break;
		case 6:
			digitalWrite(PIN_DATA6, level?LOW:HIGH);
			break;
		case 7:
			digitalWrite(PIN_DATA7, level?LOW:HIGH);
			break;
		case 8:
			digitalWrite(PIN_DATA0, LOW);
			digitalWrite(PIN_DATA1, LOW);
			digitalWrite(PIN_DATA2, LOW);
			digitalWrite(PIN_DATA3, LOW);
			digitalWrite(PIN_DATA4, LOW);
			digitalWrite(PIN_DATA5, LOW);
			digitalWrite(PIN_DATA6, LOW);
			digitalWrite(PIN_DATA7, LOW);
			break;
		}
	}
//-----------------------------------------------------------------------------
void	SetMuxAddy(byte addy) {
	digitalWrite(PIN_ADDY0, HIGH && (addy & 0x01));
	digitalWrite(PIN_ADDY1, HIGH && (addy & 0x02));
	digitalWrite(PIN_ADDY2, HIGH && (addy & 0x04));
	}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------