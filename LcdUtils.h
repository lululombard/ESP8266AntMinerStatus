#ifndef LcdUtils_h
#define LcdUtils_h

#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

class LcdUtils {
	private:
		static LiquidCrystal_I2C* lcd;
	public:
		static void begin();
		static LiquidCrystal_I2C* getLcd();
		static void drawTitle();
		static void printSplashScreen();
		static void updatePbar(int, int);
		static void clearLine(int);
		static void clearScreen();
};

#endif