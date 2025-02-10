/*
 * Serial.h
 *
 * Created: 1/03/1015 5:41:43 PM
 *  Author: Robert
 *  Modified: Tristan Feb 2025
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

//include librariesserial0_write_byte
#include <avr/io.h>
#include <stdbool.h>
#include <stdarg.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>

//macros
# define USART_BAUDRATE 38400
# define USART_BAUDRATE0 115200
# define BAUD_PRESCALE  ((((F_CPU/16)+(USART_BAUDRATE/2))/(USART_BAUDRATE))-1)
# define BAUD_PRESCALE0 ((((F_CPU/16)+(USART_BAUDRATE0/2))/(USART_BAUDRATE0))-1)

//function prototypes

//Initialize serial 0 UART 
void serial0_init(void);
//Use to test if new data is available on UART0
bool serial0_available();
//Sends text via UART0
void serial0_print_string(char * string_pointer);
//Send single byte via UART0
void serial0_write_byte(uint8_t data_byte);
//Send 0-6 bytes via UART0
void serial0_write_bytes(uint8_t numBytes, ...);
//Reads 1-6 data bytes available from UART0
void serial0_get_data(uint8_t *data, uint8_t size);
// ISR executed whenever a new byte is available in the serial buffer
ISR(USART0_RX_vect);

//Initialize serial 1 UART 
void serial1_init(void);
//Use to test if new data is available on UART1
bool serial1_available();
//Sends text via UART1
void serial1_print_string(char * string_pointer);
//Send single byte via UART1
void serial1_write_byte(uint8_t data_byte);
//Send 0-6 bytes via UART1
void serial1_write_bytes(uint8_t numBytes, ...);
//Reads 1-6 data bytes available from UART1
void serial1_get_data(uint8_t *data, uint8_t size);
// ISR executed whenever a new byte is available in the serial buffer
ISR(USART1_RX_vect);

//Initialize serial 2 UART 
void serial2_init(void);
//Use to test if new data is available on UART2
bool serial2_available();
//Sends text via UART2
void serial2_print_string(char * string_pointer);
//Send single byte via UART2
void serial2_write_byte(uint8_t data_byte);
//Send 0-6 bytes via UART2
void serial2_write_bytes(uint8_t numBytes, ...);
//Reads 1-6 data bytes available from UART2
void serial2_get_data(uint8_t *data, uint8_t size);
// ISR executed whenever a new byte is available in the serial buffer
ISR(USART2_RX_vect);  

//Initialize serial 3 UART 
void serial3_init(void);
//Use to test if new data is available on UART3
bool serial3_available();
//Sends text via UART3
void serial3_print_string(char * string_pointer);
//Send single byte via UART3
void serial3_write_byte(uint8_t data_byte);
//Send 0-6 bytes via UART3
void serial3_write_bytes(uint8_t numBytes, ...);
//Reads 1-6 data bytes available from UART3
void serial3_get_data(uint8_t *data, uint8_t size);
// ISR executed whenever a new byte is available in the serial buffer
ISR(USART3_RX_vect);

#endif /* SERIAL_H_ */
