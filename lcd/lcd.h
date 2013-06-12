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

void lcd_init(void);
int	lcd_putchar(char c, FILE *stream);
uint8_t scroll_complete(void);
void lcd_update(void);
void lcd_clear(void);
