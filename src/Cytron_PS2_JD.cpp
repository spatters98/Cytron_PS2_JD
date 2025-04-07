/**
 * @file Cytron_PS2_JD.cpp
 * @brief Modified original Cytron shield code
 * @author Idris, Cytron Technologies
 * @author S Patterson, UNCC (modifications only)
 * @date Feb 2025
 * @version 0.2
 * 
 * Modifications to original code:
 *   - correct type error
 *   - add code for PostNeoSWSerial library
 *   - remove includes not used
 */

#include "Cytron_PS2_JD.h"

Cytron_PS2Shield::Cytron_PS2Shield(uint8_t rxpin, uint8_t txpin)
{
  _txpin = txpin;
  _rxpin = rxpin;
}

Cytron_PS2Shield::Cytron_PS2Shield()
{
  _txpin = 3;
  _rxpin = 2;
}

void Cytron_PS2Shield::begin(uint32_t baudrate)
{
	if(_rxpin == 0 && _txpin == 1)
	{
		hardwareSerial = true;
		Serial.begin(baudrate);
		while(!Serial);				// wait for serial to start up
	}
	
	else
	{
		hardwareSerial = false;
		pinMode(_rxpin, INPUT);
		pinMode(_txpin, OUTPUT);
#ifdef PostNeoSWSerial_h
		PS2Serial = new PostNeoSWSerial(_rxpin, _txpin);
#else
		PS2Serial = new SoftwareSerial(_rxpin, _txpin);
#endif
		PS2Serial->begin(baudrate);
	}
	pinMode(A1, OUTPUT);
	digitalWrite(A1, HIGH);
}

void Cytron_PS2Shield::write(uint8_t data)
{
	if(hardwareSerial)
	{
		while(Serial.available() > 0) {
			Serial.read();
		}
		Serial.write(data);
		Serial.flush();		// Wait for all data transmitted
	}
	else
	{
		while(PS2Serial->available() > 0) {	
			PS2Serial->read();
		}
		PS2Serial->write(data);
	}
}

uint8_t Cytron_PS2Shield::read(void)
{
	uint8_t rec_data; 
	long waitcount = 0; 
	
	if(hardwareSerial)
	{
		while(true)
		{
			if(Serial.available() > 0)
			{
				rec_data = Serial.read();
				SERIAL_ERR = false;
				return(rec_data);
			}
			waitcount++;
			if(waitcount > 50000)
			{
				SERIAL_ERR = true; 
				return(0xFF); 
			}
		}
	}
	else
	{
		while(true)
		{
			if(PS2Serial->available() > 0)
			{
				rec_data = PS2Serial->read();
				SERIAL_ERR = false; 
				return(rec_data);
			}
			waitcount++; 
			if(waitcount > 50000)
			{
				SERIAL_ERR = true; 
				return (0xFF); 
			}
		}
	}
}

uint8_t Cytron_PS2Shield::readButton(uint8_t key)
{
	if(!hardwareSerial) PS2Serial->listen();
	write(key);
	return read();
}

bool Cytron_PS2Shield::readAllButton()
{
	uint8_t nbyte; 
	uint32_t waitcount;
	
	write(PS2_BUTTON_JOYSTICK);
	
	if(hardwareSerial)
	{		
		nbyte = Serial.readBytes(ps_data.byte, 6);
		
		if(nbyte == 6) return(true); 
		else return (false);
	}
		
	else
	{	
		waitcount = 0; 
		while(PS2Serial->available() < 6)
		{		
			waitcount++; 
			if(waitcount > 50000) {
int nbytes = PS2Serial->available();
Serial.print(nbytes);
Serial.print(" bytes available at fail\n");
for(int i=0;i<nbytes;i++) {
	Serial.print(read());
	Serial.print("  ");
}
Serial.print("\n");
Serial.println(PS2Serial->available());
				return (false); 
			}
		}
		
Serial.println(waitcount);		
		for(int i = 0; i < 6; i++) {
			ps_data.byte[i] = read();
Serial.print(ps_data.byte[i]);
Serial.print("  ");
		}
Serial.print("\n.....\n");
		return(true);	
	}
}

void Cytron_PS2Shield::vibrate(uint8_t motor, uint8_t value)
{
	uint8_t _motor;

	if(motor == 1) _motor = PS2_MOTOR_1; 
	else _motor = PS2_MOTOR_2;
	
	write(_motor);
	write(value); 
}

void Cytron_PS2Shield::reset(uint8_t reset)
{
	if(reset == 1) digitalWrite(A1, LOW);
	else digitalWrite(A1, HIGH);
}
