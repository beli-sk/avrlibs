/*
 * UART library for AVR
 * Copyright (C) 2013  Michal Belica <devel@beli.sk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 


#include "config.h"
#include <util/setbaud.h>
#include <avr/io.h>
#include <stdio.h>

#define CONVERT_CRLF 1

#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
#	define _UBRRH UBRR0H
#	define _UBRRL UBRR0L
#	define _UCSRA UCSR0A
#	define _UCSRB UCSR0B
#	define _UCSRC UCSR0C
#	define _U2X U2X0
#	define _UDRE UDRE0
#	define _UDR UDR0
#	define _UCSZ0 UCSZ00
#	define _UCSZ1 UCSZ01
#	define _RXEN RXEN0
#	define _TXEN TXEN0
#	define _RXC RXC0
#else
#	warning "Unknown part, guessing register names."
#	define _UBRRH UBRRH
#	define _UBRRL UBRRL
#	define _UCSRA UCSRA
#	define _UCSRB UCSRB
#	define _UCSRC UCSRC
#	define _U2X U2X
#	define _UDRE UDRE
#	define _UDR UDR
#	define _UCSZ0 UCSZ0
#	define _UCSZ1 UCSZ1
#	define _RXEN RXEN
#	define _TXEN TXEN
#	define _RXC RXC
#endif

int uart_getchar(FILE *stream);
int uart_putchar(char c, FILE *stream);

FILE uart_io = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

void uart_init(void) {
	_UBRRH = UBRRH_VALUE;
	_UBRRL = UBRRL_VALUE;

#if USE_2X
	_UCSRA |= _BV(_U2X);
#else
	_UCSRA &= ~_BV(_U2X);
#endif
	_UCSRC = _BV(_UCSZ1) | _BV(_UCSZ0);
	_UCSRB = _BV(_RXEN) | _BV(_TXEN);
}

int uart_putchar(char c, FILE *stream) {
#ifdef CONVERT_CRLF
	if( c == '\n' ) {
		uart_putchar('\r', stream);
	}
#endif
	loop_until_bit_is_set(_UCSRA, _UDRE);
	_UDR = c;
	return (int)c;
}

int uart_getchar(FILE *stream) {
	unsigned char c;
	loop_until_bit_is_set(_UCSRA, _RXC);
	c = _UDR;
#ifdef CONVERT_CRLF
	if( c == '\r' ) {
		c = '\n';
	}
#endif
	return (int)c;
}
