#include "FastDigital.h"
//-----------------------------------------------------------------------------
void	fDigitalWrite(int pin, int level) {
	switch (pin) {
		// Digital
		case 0: // PD0 - D0
			if(level) PORTD |= 0x01;
			else PORTD &= ~0x01;
			break;
		case 1: // PD1 - D1
			if(level) PORTD |= 0x02;
			else PORTD &= ~0x02;
			break;
		case 2: // PD2 - D2
			if(level) PORTD |= 0x04;
			else PORTD &= ~0x04;
			break;
		case 3: // PD3 - D3
			if(level) PORTD |= 0x08;
			else PORTD &= ~0x08;
			break;
		case 4: // PD4 - D4
			if(level) PORTD |= 0x10;
			else PORTD &= ~0x10;
			break;
		case 5: // PD5 - D5
			if(level) PORTD |= 0x20;
			else PORTD &= ~0x20;
			break;
		case 6: // PD6 - D6
			if(level) PORTD |= 0x40;
			else PORTD &= ~0x40;
			break;
		case 7: // PD7 - D7
			if(level) PORTD |= 0x80;
			else PORTD &= ~0x80;
			break;
		case 8: // PB0 - D8
			if(level) PORTB |= 0x01;
			else PORTB &= ~0x01;
			break;
		case 9: // PB1 - D9
			if(level) PORTB |= 0x02;
			else PORTB &= ~0x02;
			break;
		case 10: // PB2 - D10
			if(level) PORTB |= 0x04;
			else PORTB &= ~0x04;
			break;
		case 11: // PB3 - D11
			if(level) PORTB |= 0x08;
			else PORTB &= ~0x08;
			break;
		case 12: // PB4 - D12
			if(level) PORTB |= 0x10;
			else PORTB &= ~0x10;
			break;
		case 13: // PB5 - D13
			if(level) PORTB |= 0x20;
			else PORTB &= ~0x20;
			break;
		// Analog
		case 14: // PC0 - A0
			if(level) PORTC |= 0x01;
			else PORTC &= ~0x01;
			break;
		case 15: // PC1 - A1
			if(level) PORTC |= 0x02;
			else PORTC &= ~0x02;
			break;
		case 16: // PC2 - A2
			if(level) PORTC |= 0x04;
			else PORTC &= ~0x04;
			break;
		case 17: // PC3 - A3
			if(level) PORTC |= 0x08;
			else PORTC &= ~0x08;
			break;
		case 18: // PC4 - A4
			if(level) PORTC |= 0x10;
			else PORTC &= ~0x10;
			break;
		case 19: // PC5 - A5
			if(level) PORTC |= 0x20;
			else PORTC &= ~0x20;
			break;
		}
	}
//-----------------------------------------------------------------------------
int		fDigitalRead(int pin) {
	switch (pin) {
		// Digital
		case 0: // PD0 - D0
			return PIND&0x01;
		case 1: // PD1 - D1
			return PIND&0x02;
		case 2: // PD2 - D2
			return PIND&0x04;
		case 3: // PD3 - D3
			return PIND&0x08;
		case 4: // PD4 - D4
			return PIND&0x10;
		case 5: // PD5 - D5
			return PIND&0x20;
		case 6: // PD6 - D6
			return PIND&0x40;
		case 7: // PD7 - D7
			return PIND&0x80;
		case 8: // PB0 - D8
			return PINB&0x01;
		case 9: // PB1 - D9
			return PINB&0x02;
		case 10: // PB2 - D10
			return PINB&0x04;
		case 11: // PB3 - D11
			return PINB&0x08;
		case 12: // PB4 - D12
			return PINB&0x10;
		case 13: // PB5 - D13
			return PINB&0x20;
		// Analog
		case 14: // PC0 - A0
			return PORTC&0x01;
		case 15: // PC1 - A1
			return PORTC&0x02;
		case 16: // PC2 - A2
			return PORTC&0x04;
		case 17: // PC3 - A3
			return PORTC&0x08;
		case 18: // PC4 - A4
			return PORTC&0x10;
		case 19: // PC5 - A5
			return PORTC&0x20;
		}
	return -1;
	}
	//-----------------------------------------------------------------------------