#ifndef HttpPortal_h
#define HttpPortal_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ConfigFile.h"
#include "LcdUtils.h";
#include "Hotspot.h";

class HttpPortal {
	private:
		static char* assetsStatics[];
	public:
		HttpPortal();
		void doTick();
		static void handleDefault();
		static String getHeader(int);
		static String getLogin(int);
		static String getButtons(int);
		static String getAdminConfigP1(int);
		static String getAdminConfigP2(int);
		static String getAdminConfigP3(int);
		static String getFooter(int);
		static int getLvl();
		static void handleLogo();
		static void handleStyle();
};

#endif