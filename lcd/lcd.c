/*
 * High level LCD functions
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
 *
 *------
 * Works with HD44780 and ST7066 LCD controllers.
 *
 * Inspired by "Stdio demo" by Joerg Wunsch <joerg@FreeBSD.ORG>
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
