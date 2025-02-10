//Example ATmega2560 Project
//File: serial.h
//Author: Robert Howie
//Created: 2 March 2015
//Modified: Tristan Davies Feb 2025
//V1.0 Basic serial setup for printing to the serial terminal

//Derived from: http://www.github.com/abcminiuser/avr-tutorials/blob/master/USART/Output/USART.pdf?raw=true by Dean Camera
//See http://www.fourwalledcubicle.com/AVRArticles.php for more

#include "serial.h"

volatile bool serial0DataReady = false;
volatile bool serial1DataReady = false;
volatile bool serial2DataReady = false;
volatile bool serial3DataReady = false;

volatile uint8_t serial0DataByte1 = 0;
volatile uint8_t serial0DataByte2 = 0;
volatile uint8_t serial0DataByte3 = 0;
volatile uint8_t serial0DataByte4 = 0;
volatile uint8_t serial0DataByte5 = 0;
volatile uint8_t serial0DataByte6 = 0;

volatile uint8_t serial1DataByte1 = 0;
volatile uint8_t serial1DataByte2 = 0;
volatile uint8_t serial1DataByte3 = 0;
volatile uint8_t serial1DataByte4 = 0;
volatile uint8_t serial1DataByte5 = 0;
volatile uint8_t serial1DataByte6 = 0;

volatile uint8_t serial2DataByte1 = 0;
volatile uint8_t serial2DataByte2 = 0;
volatile uint8_t serial2DataByte3 = 0;
volatile uint8_t serial2DataByte4 = 0;
volatile uint8_t serial2DataByte5 = 0;
volatile uint8_t serial2DataByte6 = 0;

volatile uint8_t serial3DataByte1 = 0;
volatile uint8_t serial3DataByte2 = 0;
volatile uint8_t serial3DataByte3 = 0;
volatile uint8_t serial3DataByte4 = 0;
volatile uint8_t serial3DataByte5 = 0;
volatile uint8_t serial3DataByte6 = 0;

void serial0_init(void)
{
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //turn on the transmission and reception circuitry
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01); //use 8- bit character sizes
	UBRR0 = BAUD_PRESCALE0; //load baud rate prescaler into register
	UCSR0B |= (1 << RXCIE0); // Enable the USART Receive Complete interrupt (USART_RXC)
} //end serial0_init

bool serial0_available()
{
	return serial0DataReady; //new data available
} //end serial0_available

void serial0_print_string(char * string_pointer) //function to print a string via serial
{
	while(*string_pointer) //while not null character (end of string)
	{
		while((UCSR0A&(1<<UDRE0))==0){} //wait until data register is ready
		UDR0 = *string_pointer; //send what's at the string pointer to serial data register
		string_pointer++; //increment string pointer to go to next letter in string
	}
} //end serial0_print_string

void serial0_write_byte(uint8_t data_byte)
{
	while((UCSR0A&(1<<UDRE0))==0){} //wait until data register is ready
	UDR0 = data_byte;
} //end serial0_write_byte

void serial0_write_bytes(uint8_t numBytes, ...) //Function to write 1 to 6 bytes to serial
{
	va_list databytes; //retrieve variadic arguments
	va_start(databytes, numBytes); 
	serial0_write_byte(0xFF); //send start delimiter
	serial0_write_byte(numBytes); //send start delimiter
	for(uint8_t i=0; i<numBytes; i++) //increment through data being written
	{
		serial0_write_byte(va_arg(databytes,int));
	}
	serial0_write_byte(0xFE); //send end delimiter
	va_end(databytes);
} //end serial0_write_bytes

void serial0_get_data(uint8_t *data, uint8_t size)
{
	cli();
	uint8_t tempData[6] = {serial0DataByte1,serial0DataByte2,serial0DataByte3,serial0DataByte4,serial0DataByte5,serial0DataByte6}; //local copy of data
	sei();
	for(uint8_t i = 0; i<size; i++) //increment data to be returned
	{
		data[i] = tempData[i];  //pass data through pointer
	}
	serial0DataReady = false; //new data no longer available
} //end serial0_get_data

ISR(USART0_RX_vect)  // ISR executed whenever a new byte is available in the serial buffer
{
	static uint8_t recvByte1=0, recvByte2=0, recvByte3=0, recvByte4=0, recvByte5=0, recvByte6=0;		// data bytes received
	static uint8_t serial_fsm_state=0;									// used in the serial receive ISR
	static uint8_t numBytes = 6;
	uint8_t	serial_byte_in = UDR0; //move serial byte into variable
	
	if(serial_byte_in == 0xFE) //check for ned delimiter
	{
		if(serial_fsm_state == numBytes)
		{
			//Assign local data to be available externally
			serial0DataByte1 = recvByte1;
			serial0DataByte2 = recvByte2;
			serial0DataByte3 = recvByte3;
			serial0DataByte4 = recvByte4;
			serial0DataByte5 = recvByte5;
			serial0DataByte6 = recvByte6;
			// now that the stop byte has been received, set a flag that new data is available
			serial0DataReady=true;
		}
		serial_fsm_state = 0; //set to expect start byte
	}
	switch(serial_fsm_state) //switch by the current state
	{
		case 0:
		//do nothing, if check after switch case will find start byte and set serial_fsm_state to 1
		break;
		case 1: //waiting for number of bytes parameter
		numBytes = serial_byte_in + 2;
		serial_fsm_state++;
		break;
		case 2: //waiting for first parameter
		recvByte1 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 3: //waiting for second parameter
		recvByte2 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 4: //waiting for third parameter
		recvByte3 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 5: //waiting for fourth parameter
		recvByte4 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 6: //waiting for fifth parameter
		recvByte5 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 7: //waiting for sixth parameter
		recvByte6 = serial_byte_in;
		serial_fsm_state++;
		break;
	}
	if(serial_byte_in == 0xFF) //if start byte is received, we go back to expecting the first data byte
	{
		serial_fsm_state=1;
	}
} //end ISR

void serial1_init(void)
{
	UCSR1B = (1<<RXEN1)|(1<<TXEN1); //turn on the transmission and reception circuitry
	UCSR1C = (1<<UCSZ10)|(1<<UCSZ11); //use 8- bit character sizes
	UBRR1 = BAUD_PRESCALE; //load baud rate prescaler into register
	UCSR1B |= (1 << RXCIE1); // Enable the USART Receive Complete interrupt (USART_RXC)
} //end serial1_init

bool serial1_available()
{
	return serial1DataReady; //new data available
} //end serial1_available

void serial1_print_string(char * string_pointer) //function to print a string via serial
{
	while(*string_pointer) //while not null character (end of string)
	{
		while((UCSR1A&(1<<UDRE1))==0){} //wait until data register is ready
		UDR1 = *string_pointer; //send what's at the string pointer to serial data register
		string_pointer++; //increment string pointer to go to next letter in string
	}
} //end serial1_print_string

void serial1_write_byte(uint8_t data_byte)
{
	while((UCSR1A&(1<<UDRE1))==0){} //wait until data register is ready
	UDR1 = data_byte;
} //end serial1_write_byte

void serial1_write_bytes(uint8_t numBytes, ...) //Function to write 1 to 6 bytes to serial
{
	va_list databytes; //retrieve variadic arguments
	va_start(databytes, numBytes); 
	serial1_write_byte(0xFF); //send start delimiter
	serial1_write_byte(numBytes); //send start delimiter
	for(uint8_t i=0; i<numBytes; i++) //increment through data being written
	{
		serial1_write_byte(va_arg(databytes,int));
	}
	serial1_write_byte(0xFE); //send end delimiter
	va_end(databytes);
} //end serial1_write_bytes

void serial1_get_data(uint8_t *data, uint8_t size)
{
	cli();
	uint8_t tempData[6] = {serial1DataByte1,serial1DataByte2,serial1DataByte3,serial1DataByte4,serial1DataByte5,serial1DataByte6}; //local copy of data
	sei();
	for(uint8_t i = 0; i<size; i++) //increment data to be returned
	{
		data[i] = tempData[i];  //pass data through pointer
	}
	serial1DataReady = false; //new data no longer available
} //end serial1_get_data

ISR(USART1_RX_vect)  // ISR executed whenever a new byte is available in the serial buffer
{
	static uint8_t recvByte1=0, recvByte2=0, recvByte3=0, recvByte4=0, recvByte5=0, recvByte6=0;		// data bytes received
	static uint8_t serial_fsm_state=0;									// used in the serial receive ISR
	static uint8_t numBytes = 6;
	uint8_t	serial_byte_in = UDR1; //move serial byte into variable
	
	if(serial_byte_in == 0xFE) //check for ned delimiter
	{
		if(serial_fsm_state == numBytes)
		{
			//Assign local data to be available externally
			serial1DataByte1 = recvByte1;
			serial1DataByte2 = recvByte2;
			serial1DataByte3 = recvByte3;
			serial1DataByte4 = recvByte4;
			serial1DataByte5 = recvByte5;
			serial1DataByte6 = recvByte6;
			// now that the stop byte has been received, set a flag that new data is available
			serial1DataReady=true;
		}
		serial_fsm_state = 0; //set to expect start byte
	}
	switch(serial_fsm_state) //switch by the current state
	{
		case 0:
		//do nothing, if check after switch case will find start byte and set serial_fsm_state to 1
		break;
		case 1: //waiting for number of bytes parameter
		numBytes = serial_byte_in + 2;
		serial_fsm_state++;
		break;
		case 2: //waiting for first parameter
		recvByte1 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 3: //waiting for second parameter
		recvByte2 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 4: //waiting for third parameter
		recvByte3 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 5: //waiting for fourth parameter
		recvByte4 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 6: //waiting for fifth parameter
		recvByte5 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 7: //waiting for sixth parameter
		recvByte6 = serial_byte_in;
		serial_fsm_state++;
		break;
	}
	if(serial_byte_in == 0xFF) //if start byte is received, we go back to expecting the first data byte
	{
		serial_fsm_state=1;
	}
} //end ISR

void serial2_init(void)
{
	UCSR2B = (1<<RXEN2)|(1<<TXEN2); //turn on the transmission and reception circuitry
	UCSR2C = (1<<UCSZ20)|(1<<UCSZ21); //use 8- bit character sizes
	UBRR2 = BAUD_PRESCALE; //load baud rate prescaler into register
	UCSR2B |= (1 << RXCIE2); // Enable the USART Receive Complete interrupt (USART_RXC)
} //end serial2_init

bool serial2_available()
{
	return serial2DataReady; //new data available
} //end serial2_available

void serial2_print_string(char * string_pointer) //function to print a string via serial
{
	while(*string_pointer) //while not null character (end of string)
	{
		while((UCSR2A&(1<<UDRE2))==0){} //wait until data register is ready
		UDR2 = *string_pointer; //send what's at the string pointer to serial data register
		string_pointer++; //increment string pointer to go to next letter in string
	}
} //end serial2_print_string

void serial2_write_byte(uint8_t data_byte)
{
	while((UCSR2A&(1<<UDRE2))==0){} //wait until data register is ready
	UDR2 = data_byte;
} //end serial2_write_byte

void serial2_write_bytes(uint8_t numBytes, ...) //Function to write 1 to 6 bytes to serial
{
	va_list databytes; //retrieve variadic arguments
	va_start(databytes, numBytes); 
	serial2_write_byte(0xFF); //send start delimiter
	serial2_write_byte(numBytes); //send start delimiter
	for(uint8_t i=0; i<numBytes; i++) //increment through data being written
	{
		serial2_write_byte(va_arg(databytes,int));
	}
	serial2_write_byte(0xFE); //send end delimiter
	va_end(databytes);
} //end serial2_write_bytes

void serial2_get_data(uint8_t *data, uint8_t size)
{
	cli();
	uint8_t tempData[6] = {serial2DataByte1,serial2DataByte2,serial2DataByte3,serial2DataByte4,serial2DataByte5,serial2DataByte6}; //local copy of data
	sei();
	for(uint8_t i = 0; i<size; i++) //increment data to be returned
	{
		data[i] = tempData[i];  //pass data through pointer
	}
	serial2DataReady = false; //new data no longer available
} //end serial2_get_data

ISR(USART2_RX_vect)  // ISR executed whenever a new byte is available in the serial buffer
{
	static uint8_t recvByte1=0, recvByte2=0, recvByte3=0, recvByte4=0, recvByte5=0, recvByte6=0;		// data bytes received
	static uint8_t serial_fsm_state=0;									// used in the serial receive ISR
	static uint8_t numBytes = 6;
	uint8_t	serial_byte_in = UDR2; //move serial byte into variable
	if(serial_byte_in == 0xFE) //check for end delimiter
	{
		if(serial_fsm_state == numBytes)
		{
			//Assign local data to be available externally
			serial2DataByte1 = recvByte1;
			serial2DataByte2 = recvByte2;
			serial2DataByte3 = recvByte3;
			serial2DataByte4 = recvByte4;
			serial2DataByte5 = recvByte5;
			serial2DataByte6 = recvByte6;
			// now that the stop byte has been received, set a flag that new data is available
			serial2DataReady=true;
		}
		serial_fsm_state = 0; //set to expect start byte
	}
	switch(serial_fsm_state) //switch by the current state
	{
		case 0:
		//do nothing, if check after switch case will find start byte and set serial_fsm_state to 1
		break;
		case 1: //waiting for number of bytes parameter
		numBytes = serial_byte_in + 2;
		serial_fsm_state++;
		break;
		case 2: //waiting for first parameter
		recvByte1 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 3: //waiting for second parameter
		recvByte2 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 4: //waiting for third parameter
		recvByte3 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 5: //waiting for fourth parameter
		recvByte4 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 6: //waiting for fifth parameter
		recvByte5 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 7: //waiting for sixth parameter
		recvByte6 = serial_byte_in;
		serial_fsm_state++;
		break;
	}
	if(serial_byte_in == 0xFF) //if start byte is received, we go back to expecting the first data byte
	{
		serial_fsm_state=1;
	}
} //end ISR

void serial3_init(void)
{
	UCSR3B = (1<<RXEN3)|(1<<TXEN3); //turn on the transmission and reception circuitry
	UCSR3C = (1<<UCSZ30)|(1<<UCSZ31); //use 8- bit character sizes
	UBRR3 = BAUD_PRESCALE; //load baud rate prescaler into register
	UCSR3B |= (1 << RXCIE3); // Enable the USART Receive Complete interrupt (USART_RXC)
} //end serial3_init

bool serial3_available()
{
	return serial3DataReady; //new data available
} //end serial3_available

void serial3_print_string(char * string_pointer) //function to print a string via serial
{
	while(*string_pointer) //while not null character (end of string)
	{
		while((UCSR3A&(1<<UDRE3))==0){} //wait until data register is ready
		UDR3 = *string_pointer; //send what's at the string pointer to serial data register
		string_pointer++; //increment string pointer to go to next letter in string
	}
} //end serial3_print_string

void serial3_write_byte(uint8_t data_byte)
{
	while((UCSR3A&(1<<UDRE3))==0){} //wait until data register is ready
	UDR3 = data_byte;
} //end serial3_write_byte

void serial3_write_bytes(uint8_t numBytes, ...) //Function to write 1 to 6 bytes to serial
{
	va_list databytes; //retrieve variadic arguments
	va_start(databytes, numBytes); 
	serial3_write_byte(0xFF); //send start delimiter
	serial3_write_byte(numBytes); //send start delimiter
	for(uint8_t i=0; i<numBytes; i++) //increment through data being written
	{
		serial3_write_byte(va_arg(databytes,int));
	}
	serial3_write_byte(0xFE); //send end delimiter
	va_end(databytes);
} //end serial3_write_bytes

void serial3_get_data(uint8_t *data, uint8_t size)
{
	cli();
	uint8_t tempData[6] = {serial3DataByte1,serial3DataByte2,serial3DataByte3,serial3DataByte4,serial3DataByte5,serial3DataByte6}; //local copy of data
	sei();
	for(uint8_t i = 0; i<size; i++) //increment data to be returned
	{
		data[i] = tempData[i];  //pass data through pointer
	}
	serial3DataReady = false; //new data no longer available
} //end serial3_get_data

ISR(USART3_RX_vect)  // ISR executed whenever a new byte is available in the serial buffer
{
	static uint8_t recvByte1=0, recvByte2=0, recvByte3=0, recvByte4=0, recvByte5=0, recvByte6=0;		// data bytes received
	static uint8_t serial_fsm_state=0;									// used in the serial receive ISR
	static uint8_t numBytes = 6;
	uint8_t	serial_byte_in = UDR3; //move serial byte into variable
	
	if(serial_byte_in == 0xFE) //check for ned delimiter
	{
		if(serial_fsm_state == numBytes)
		{
			//Assign local data to be available externally
			serial3DataByte1 = recvByte1;
			serial3DataByte2 = recvByte2;
			serial3DataByte3 = recvByte3;
			serial3DataByte4 = recvByte4;
			serial3DataByte5 = recvByte5;
			serial3DataByte6 = recvByte6;
			// now that the stop byte has been received, set a flag that new data is available
			serial2DataReady=true;
		}
		serial_fsm_state = 0; //set to expect start byte
	}
	switch(serial_fsm_state) //switch by the current state
	{
		case 0:
		//do nothing, if check after switch case will find start byte and set serial_fsm_state to 1
		break;
		case 1: //waiting for number of bytes parameter
		numBytes = serial_byte_in + 2;
		serial_fsm_state++;
		break;
		case 2: //waiting for first parameter
		recvByte1 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 3: //waiting for second parameter
		recvByte2 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 4: //waiting for third parameter
		recvByte3 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 5: //waiting for fourth parameter
		recvByte4 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 6: //waiting for fifth parameter
		recvByte5 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 7: //waiting for sixth parameter
		recvByte6 = serial_byte_in;
		serial_fsm_state++;
		break;
	}
	if(serial_byte_in == 0xFF) //if start byte is received, we go back to expecting the first data byte
	{
		serial_fsm_state=1;
	}
} //end ISR
