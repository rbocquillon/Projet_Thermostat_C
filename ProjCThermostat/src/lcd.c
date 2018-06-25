/**
 * @file lcd.c
 * The LCD has a data register and a command register.
 */
#include <stdio.h>

#include "appalloc.h"
#include "lcd.h"

/*
 * ***************************
 * NE PAS MODIFIER CE FICHIER
 * ***************************
 */

struct LCD {
	IPort _portCmd;
	IPort _portData;
	int _iPos;
};

#define COMMAND_CLEAR 0x01
#define COMMAND_HOME 0x02
#define COMMAND_TO_DISPLAY 0x80

/**
 * Send a command to the command register of the LCD component.
 *
 * @param lcd This LCD.
 * @param iCmd The command to send.
 */
static void lcdSendCommand(ILCD lcd, char iCmd) {
	portWrite(lcd->_portCmd, iCmd);
}

/**
 * Send a data to the data register of the LCD component.
 *
 * @param lcd This LCD.
 * @param iData The data to send.
 */
static void lcdSendData(ILCD lcd, char iData) {
	portWrite(lcd->_portData, iData);
}

/**
 * Construct a LCD.
 *
 * @param portCmd The port where to send commands.
 * @param portData The port where to send data.
 */
ILCD createLCD(IPort portCmd, IPort portData) {
	ILCD lcd = (ILCD) appalloc(sizeof(struct LCD), "lcd_alloc");
	lcd->_portCmd = portCmd;
	lcd->_portData = portData;
	lcdSendCommand(lcd, 0x30);
	lcdSendCommand(lcd, 0x30);
	lcdSendCommand(lcd, 0x30);
	lcdSendCommand(lcd, 0x20);
	lcdSendCommand(lcd, 0x20);
	lcdSendCommand(lcd, 0xC0);
	lcdSendCommand(lcd, 0x10);
	lcdSendCommand(lcd, 0x80);
	lcdSendCommand(lcd, 0x00);
	lcdSendCommand(lcd, 0x30);
	lcdSendCommand(lcd, 0x00);
	lcdSendCommand(lcd, 0xE0);
	lcdClear(lcd);
	return lcd;
}

/**
 * Clear the LCD.
 *
 * @param lcd This LCD.
 */
void lcdClear(ILCD lcd) {
	lcd->_iPos = 0;
	lcdSendCommand(lcd, COMMAND_CLEAR);
}

/**
 * Place the cursor at the 0 position (top left of the LCD screen).
 *
 * @param lcd This LCD.
 */
void lcdHome(ILCD lcd) {
	lcd->_iPos = 0;
	lcdSendCommand(lcd, COMMAND_HOME);
}

/**
 * Set the cursor position at the specified position.
 *
 * @param lcd This LCD.
 * @param iAt The cursor position [0-31]
 */
void lcdSetCurs(ILCD lcd, int iAt) {
	if (iAt >= 0 && iAt < 32) {
		lcd->_iPos = iAt;
		lcdSendCommand(lcd, COMMAND_TO_DISPLAY | iAt);
	}
}

/**
 * Get the cursor position.
 *
 * @param lcd This LCD.
 * @return The cursor position [0..32]
 */
int lcdGetCurs(ILCD lcd) {
	return lcd->_iPos;
}

/**
 * Write a character to the current cursor position.
 *
 * @param lcd This LCD.
 * @param c The character to display.
 */
void lcdWrite(ILCD lcd, char c) {
	lcdSendData(lcd, c);
	lcd->_iPos++;
}
