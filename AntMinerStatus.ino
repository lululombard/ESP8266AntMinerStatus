#include <ESP8266WiFi.h>

#include "AntMiner.h";
#include "Hotspot.h";
#include "HttpPortal.h";
#include "TickScheduler.h";

AntMiner* antminer;
Hotspot* hotspot;
HttpPortal* http;

void setup (void) {
	TickScheduler::beginTick();
	TickScheduler::beginTask("InitLcd");
	LcdUtils::begin();
	TickScheduler::endTaskAndBeginTask("InitLcd", "InitSerial");
	Serial.begin(74880);
	TickScheduler::endTaskAndBeginTask("InitSerial", "InitConfig");
	ConfigFile::begin();
	TickScheduler::endTaskAndBeginTask("InitConfig", "InitWifi");
	Hotspot::connectWifi();
	TickScheduler::endTaskAndBeginTask("InitWifi", "InitHttp");
	http = new HttpPortal();
	TickScheduler::endTaskAndBeginTask("InitHttp", "InitHotspot");
	hotspot = new Hotspot();
	TickScheduler::endTaskAndBeginTask("InitHotspot", "InitAntMiner");
	antminer = new AntMiner();
	TickScheduler::endTask("InitAntMiner");
	TickScheduler::endTick();
}

void loop (void) {
	TickScheduler::beginTick();
	TickScheduler::beginTask("Hotspot");
	hotspot->doTick();
	TickScheduler::endTaskAndBeginTask("Hotspot", "HTTP");
	http->doTick();
	TickScheduler::endTaskAndBeginTask("HTTP", "AntMiner");
	antminer->doTick();
	TickScheduler::endTask("AntMiner");
	TickScheduler::endTick();
}