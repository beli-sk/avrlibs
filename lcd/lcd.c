/*
 * High level LCD functions
 *
 * Works with HD44780 and ST7066 LCD controllers.
 *
 * Inspired by "Stdio demo" by Joerg Wunsch <joerg@FreeBSD.ORG>
 *
 * Copyright 2013, Michal Belica <devel@beli.sk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include <stdio.h>
//#include <avr/io.h>

#include "hd44780.h"
#include "lcd.h"

#define LCD_COLS 8
#define LCD_LINES 2 // 2 only for now
#if !defined(LCD_COLS) || !defined(LCD_LINES)
#	error LCD_COLS and LCD_LINES need to be defined.
#endif

#if LCD_LINES != 2
#	error Only 2-line display supported at the moment (define LCD_LINES 2).
#endif

static volatile unsigned char count = 0;
static volatile unsigned char scrollcount = 0;

/* 
 * Clear the display.
 */
void lcd_clear(void) {
	count = 0;
	scrollcount = 0;
	hd44780_wait_ready(false);
	hd44780_outcmd(HD44780_CLR);
	hd44780_wait_ready(true);
	hd44780_outcmd(HD44780_HOME);
	hd44780_wait_ready(true);
	hd44780_outcmd(HD44780_DDADDR(0));
}

void lcd_init(void) {

	hd44780_init();


	// Entry mode set: cursor move, no display shift.
	hd44780_outcmd(HD44780_ENTMODE(1, 0));
	hd44780_wait_ready(false);

	// Enable display, disable cursor
	// (FIXME cursor doesn't work well with split-line mode)
	hd44780_outcmd(HD44780_DISPCTL(1, 0, 0));
	hd44780_wait_ready(false);

	lcd_clear();
}

/*
 * lcd_update - shifts the display by 1 position when needed.
 * Should be called periodically with frequency depending on how fast
 * the scrolling should be. When calling from interrupts note that the
 * function waits for LCD to become ready.
 */
void lcd_update(void) {
	if( count > scrollcount + LCD_COLS*2 - 1 ) {
		if( scrollcount ) {
			hd44780_wait_ready(false);
			hd44780_outcmd(HD44780_SHIFT(1, 0));
		}
		scrollcount++;
	}
}

/*
 * Returns true if scrolling is at the end (whole message was displayed).
 */
uint8_t scroll_complete(void) {
	return count <= scrollcount + LCD_COLS*2 - 1;
}

/*
 * Display character
 */
int lcd_putchar( char c, FILE *stream ) {
	static uint8_t nl;

	if (nl_seen && c != '\n') {
		lcd_clear();
		nl = false;
	}
	if( c == '\n' ) {
		nl = true;
	} else if( count < 40-1 ) {
		if( count >= LCD_COLS ) {
			hd44780_wait_ready(false);
			hd44780_outcmd(HD44780_DDADDR(count));
			hd44780_wait_ready(false);
			hd44780_outdata(c);
			hd44780_wait_ready(false);
			hd44780_outcmd(HD44780_DDADDR(40 + count - LCD_COLS));
			hd44780_wait_ready(false);
			hd44780_outdata(c);
		} else {
			hd44780_wait_ready(false);
			hd44780_outdata(c);
			if( count == LCD_COLS-1 ) {
				hd44780_wait_ready(false);
				hd44780_outcmd(HD44780_DDADDR(40));
			}
		}
		count++;
	}

	return 0;
}
