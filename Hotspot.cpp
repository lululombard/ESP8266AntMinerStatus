#include "Hotspot.h"

DNSServer* dns = NULL;
bool Hotspot::failure = true;

Hotspot::Hotspot() {
	dns = new DNSServer();
	IPAddress apIP(192, 168, 4, 1);
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	dns->setErrorReplyCode(DNSReplyCode::NoError);
	dns->start(53, "*", apIP);
}

void Hotspot::connectWifi() {
	String ssid = ConfigFile::getConfigWifiSsid();
	String pass = ConfigFile::getConfigWifiPass();
	String host = ConfigFile::getConfigWifiHost();
	bool found = false;
	int wifi_stations = WiFi.scanNetworks();
	Serial.println("[INFO] Scanning Wi-Fi networks");
	for (int i = 0; i < wifi_stations; i++) {
		Serial.print("[INFO] Discovered ");
		Serial.println(WiFi.SSID(i));
		if (WiFi.SSID(i).equalsIgnoreCase(host)) {
			host = "AntMiner ";
			host += ConfigFile::getMac();
			host.replace(":", "");
			Serial.print("[WARN] Hostname already taken, will be named ");
			Serial.println(host);
		}
	}
	for (int i = 0; i <= wifi_stations; i++) {
		if (WiFi.SSID(i).equals(ssid)) found = true;
	}
	WiFi.softAP(host.c_str());
	WiFi.hostname(host);
	WiFi.mode(WIFI_AP);
	LcdUtils::clearScreen();
	if (ssid == "") {
		Serial.println("[WARN] No SSID to connect to");
		LcdUtils::getLcd()->setCursor(0,1);
		LcdUtils::getLcd()->print("First config needed!");
		LcdUtils::getLcd()->setCursor(0,2);
		LcdUtils::getLcd()->print(" Please connect to: ");
		LcdUtils::getLcd()->setCursor(0,3);
		LcdUtils::getLcd()->print(host);
	}
	else if (!found) {
		Serial.println("[WARN] SSID not found");
		LcdUtils::getLcd()->setCursor(0,1);
		LcdUtils::getLcd()->print("  Wi-Fi not found!  ");
		LcdUtils::getLcd()->setCursor(0,2);
		LcdUtils::getLcd()->print(" Please connect to: ");
		LcdUtils::getLcd()->setCursor(0,3);
		LcdUtils::getLcd()->print(host);
	}
	else {
		Serial.print("[INFO] Connecting to ");
		Serial.println(ssid);
		LcdUtils::getLcd()->setCursor(0,1);
		LcdUtils::getLcd()->print("Connecting Wi-Fi... ");
		LcdUtils::getLcd()->setCursor(0,2);
		LcdUtils::getLcd()->print(ssid);
		int count = 0;
		WiFi.begin(ssid.c_str(), pass.c_str());
		while (WiFi.status() != WL_CONNECTED) {
			delay(1);
			count++;
			if (count == 20000) {
				Serial.println("[WARN] Incorrect key ");
				LcdUtils::getLcd()->setCursor(0,1);
				LcdUtils::getLcd()->print("Wrong Wi-Fi passcode");
				LcdUtils::getLcd()->setCursor(0,2);
				LcdUtils::getLcd()->print(" Please connect to: ");
				LcdUtils::getLcd()->setCursor(0,3);
				LcdUtils::getLcd()->print(host);
				break;
			}
		}
		if (WiFi.status() == WL_CONNECTED) {
			Hotspot::failure = false;
			Serial.print("[INFO] Connected ! IP address : ");
			Serial.println(WiFi.localIP());
			LcdUtils::clearScreen();
			LcdUtils::getLcd()->setCursor(0,2);
			LcdUtils::getLcd()->print("IP : ");
			LcdUtils::getLcd()->print(WiFi.localIP());
			
			MDNS.begin(host.c_str());
		}
	}
}

bool Hotspot::isFailure() {
	return Hotspot::failure;
}

void Hotspot::doTick() {
	dns->processNextRequest();
}