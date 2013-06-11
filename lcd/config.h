/* HD44780 LCD port connections */
#define HD44780_RS D, 5
#define HD44780_RW D, 6
#define HD44780_E  D, 7
/* The data bits have to be not only in ascending order but also consecutive. */
#define HD44780_D4 C, 0

/* Whether to read the busy flag, or fall back to
 *    worst-time delays. */
#define USE_BUSY_BIT 1

#define LCD_COLS 8
#define LCD_LINES 2
