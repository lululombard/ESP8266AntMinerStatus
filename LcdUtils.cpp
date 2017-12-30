#include "LcdUtils.h"

LiquidCrystal_I2C* LcdUtils::lcd = NULL;

void LcdUtils::begin() {
	LcdUtils::lcd = new LiquidCrystal_I2C(0x27, 20, 4);
	lcd->begin();
	lcd->backlight();

	byte persecond[8] = { B00010, B00100, B01000, B10111, B01000, B00110, B00001, B01110 };
	
	byte terrahash[8] = { B01110, B00100, B00100, B00000, B01010, B01110, B01010, B00000 };

	byte fan1[8] = { B01110, B11011, B10101, B11011, B01110, B00010, B11111, B00010 };

	byte fan2[8] = { B01110, B11011, B10101, B11011, B01110, B01000, B11111, B01000 };

	byte euro[8] = { B00011, B00100, B11111, B01000, B11111, B00100, B00011, B00000 };

	byte gbp[8] = { B01110, B10001, B10000, B01000, B11110, B01000, B11111, B00000 };

	byte degc[8] = { B01000, B10100, B01000, B00011, B00100, B00100, B00100, B00011 };

	byte bitcoin[8] = { B01010, B11110, B01001, B01110, B01001, B01001, B11110, B01010 };

	byte bitmain[8] = { B01001, B10010, B10100, B10100, B01110, B11111, B11011, B11001 };



	lcd->createChar(0, persecond);
	lcd->createChar(1, terrahash);
	lcd->createChar(2, fan1);
	lcd->createChar(3, fan2);
	lcd->createChar(4, euro);
	lcd->createChar(5, gbp);
	lcd->createChar(223, degc);
	lcd->createChar(6, bitcoin);
	lcd->createChar(7, bitmain);
	printSplashScreen();
}

void LcdUtils::printSplashScreen() {
	drawTitle();
	lcd->setCursor(7,1);
	lcd->print("v0.0.1");
	lcd->setCursor(0,2);
	lcd->print(" Initializing Wi-Fi ");
}

LiquidCrystal_I2C* LcdUtils::getLcd() {
	return LcdUtils::lcd;
}

void LcdUtils::drawTitle() {
	lcd->setCursor(0,0);
	lcd->print(" ");
	lcd->write(7);
	lcd->print(" AntMiner Status  ");
}
void LcdUtils::clearLine(int line) {
	lcd->setCursor(0, line);
	lcd->print("                    ");
}

void LcdUtils::clearScreen() {
	for (int i = 1; i <= 3; i++) clearLine(i);
}