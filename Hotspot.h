#ifndef Hotspot_h
#define Hotspot_h

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "ConfigFile.h";
#include "LcdUtils.h";

class Hotspot {
	private:
		DNSServer* dns;
		static bool failure;
	public :
		Hotspot();
		static void connectWifi();
		static bool isFailure();
		void doTick();
};

#endif