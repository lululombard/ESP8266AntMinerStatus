#ifndef AntMiner_h
#define AntMiner_h

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "ConfigFile.h";
#include "LcdUtils.h";

class AntMiner {
	private:
		String currency;
		unsigned int errCount;
		unsigned long millisec, coindeskmillisec;
		float btc, totghs, ghs1, ghs2, ghs3;
		int temp1, temp2, temp3, fan1, fan2;
	public:
		AntMiner();
		bool readFromApi();
		bool readCoindeskApi();
		void refreshScreen();
		void errorScreen();
		void doTick();
};

#endif