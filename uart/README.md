UART Library for AVR
====================

Written by Michal Belica [devel (at) beli (dot) sk]

Inspired by blog post [Simple Serial Communications With AVR Libc](http://www.appelsiini.net/2011/simple-usart-with-avr-libc) by Mika Tuupola.

Usage example:

```
#define F_CPU 8000000UL
#define BAUD 9600
#include "uart.h"
#include <stdio.h>

int main(void) {
	uart_init();
	stdout = &uart_io;
	stdin = &uart_io;
	puts("Hello world!");
	// ...
}
```
