UART Library for AVR
====================

Written by Michal Belica `devel (at) beli (dot) sk`

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

License
-------

UART Library for AVR
Copyright (C) 2013  Michal Belica `devel (at) beli (dot) sk`

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see < http://www.gnu.org/licenses/ >.

