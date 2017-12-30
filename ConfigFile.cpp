#include "ConfigFile.h"

bool ConfigFile::mustrestart = 0;
bool ConfigFile::mount = 0;
String ConfigFile::configWifiSsid = "";
String ConfigFile::configWifiPass = "";
String ConfigFile::configWifiHost = "";
String ConfigFile::configWifiDhcp = "";
String ConfigFile::configWifiIpStaticIp = "";
String ConfigFile::configWifiIpNetmask = "";
String ConfigFile::configWifiIpGateway = "";
String ConfigFile::configAntMinerIp = "";
String ConfigFile::configCoindeskCurrency = "";
String ConfigFile::configPassAdmin = "";

void ConfigFile::begin() {
	LcdUtils::printSplashScreen();
	mount = SPIFFS.begin();
	if (!ConfigFile::mount) {
		Serial.println("[WARN] SPIFFS needs to be formated");
		if (!SPIFFS.format()) {
			Serial.println("[GRAVE] Could not format SPIFFS");
			return;
		}
		mount = SPIFFS.begin();
	}
	if (!mount) Serial.println("[GRAVE] Could not mount SPIFFS");
	loadConfig();
}

void ConfigFile::checkForDefaultConfig() {
	createDefaultValue("/config/wifi/ssid.txt", "");
	createDefaultValue("/config/wifi/pass.txt", "");
	createDefaultValue("/config/wifi/host.txt", "AntMiner Status");
	createDefaultValue("/config/wifi/dhcp.txt", "1");
	createDefaultValue("/config/wifi/ip_staticip.txt", "192.168.1.100");
	createDefaultValue("/config/wifi/ip_netmask.txt", "255.255.255.0");
	createDefaultValue("/config/wifi/ip_gateway.txt", "192.168.1.1");
	createDefaultValue("/config/antminer/ip.txt", "127.0.0.1");
	createDefaultValue("/config/coindesk/currency.txt", "USD");
	createDefaultValue("/pass/admin.txt", "antminer");
}

void ConfigFile::resetConfig() {
	setMustRestart();
	SPIFFS.remove("/config/wifi/ssid.txt");
	SPIFFS.remove("/config/wifi/pass.txt");
	SPIFFS.remove("/config/wifi/host.txt");
	SPIFFS.remove("/config/wifi/dhcp.txt");
	SPIFFS.remove("/config/wifi/ip_staticip.txt");
	SPIFFS.remove("/config/wifi/ip_netmask.txt");
	SPIFFS.remove("/config/antminer/ip.txt");
	SPIFFS.remove("/config/coindesk/currency.txt");
	SPIFFS.remove("/config/wifi/ip_gateway.txt");
	loadConfig();
}

void ConfigFile::loadConfig() {
	checkForDefaultConfig();
	int elem = 0;
	Serial.println("[INFO] Loading config from SPIFFS");
	configWifiSsid = ConfigFile::readFileContents("/config/wifi/ssid.txt");
	Serial.print("[INFO] Config node added : configWifiSsid=");
	Serial.println(ConfigFile::getConfigWifiSsid());
	configWifiPass = ConfigFile::readFileContents("/config/wifi/pass.txt");
	Serial.print("[INFO] Config node added : configWifiPass=");
	Serial.println(ConfigFile::getConfigWifiPass());
	configWifiHost = ConfigFile::readFileContents("/config/wifi/host.txt");
	Serial.print("[INFO] Config node added : configWifiHost=");
	Serial.println(ConfigFile::getConfigWifiHost());
	configWifiDhcp = ConfigFile::readFileContents("/config/wifi/dhcp.txt");
	Serial.print("[INFO] Config node added : configWifiDhcp=");
	Serial.println(ConfigFile::getConfigWifiDhcp());
	configWifiIpStaticIp = ConfigFile::readFileContents("/config/wifi/ip_staticip.txt");
	Serial.print("[INFO] Config node added : configWifiIpStaticIp=");
	Serial.println(ConfigFile::getConfigWifiIpStaticIp());
	configWifiIpNetmask = ConfigFile::readFileContents("/config/wifi/ip_netmask.txt");
	Serial.print("[INFO] Config node added : configWifiIpNetmask=");
	Serial.println(ConfigFile::getConfigWifiIpNetmask());
	configWifiIpGateway = ConfigFile::readFileContents("/config/wifi/ip_gateway.txt");
	Serial.print("[INFO] Config node added : configWifiIpGateway=");
	Serial.println(ConfigFile::getConfigWifiIpGateway());
	configAntMinerIp = ConfigFile::readFileContents("/config/antminer/ip.txt");
	Serial.print("[INFO] Config node added : configAntMinerIp=");
	Serial.println(ConfigFile::getConfigAntMinerIp());
	configCoindeskCurrency = ConfigFile::readFileContents("/config/coindesk/currency.txt");
	Serial.print("[INFO] Config node added : configCoindeskCurrency=");
	Serial.println(ConfigFile::getConfigCoindeskCurrency());
	configPassAdmin = ConfigFile::readFileContents("/pass/admin.txt");
	Serial.print("[INFO] Config node added : configPassAdmin=");
	//Serial.println(ConfigFile::getConfigPassAdmin());
	Serial.println("******");
	Serial.println("[INFO] Config loaded");
}

String ConfigFile::readFileContents(char* filename) {
	if (!ConfigFile::mount) return "";
	if (!SPIFFS.exists(filename)) return "";
	File f = SPIFFS.open(filename, "r");
	if (f) {
		String result = f.readStringUntil('\n');
		result.replace("\r", "");
		result.replace("\n", "");
		result.trim();
		String toreturn = result;
		f.close();
		return toreturn;
	}
	return "";
}

void ConfigFile::createDefaultValue(char* filename, char* value) {
	if (SPIFFS.exists(filename)) return;
	writeToFile(filename, value);
}

void ConfigFile::writeToFile(char* filename, char* value) {
	writeToFile(filename, String(value));
}

void ConfigFile::writeToFile(char* filename, String value) {
	if (!ConfigFile::mount) return;
	File f = SPIFFS.open(filename, "w");
	if (f) {
		f.println(value);
		f.close();
	}
}

bool ConfigFile::getMustRestart() {
	return ConfigFile::mustrestart;
}

void ConfigFile::setMustRestart() {
	ConfigFile::mustrestart = true;
}

String ConfigFile::getMac() {
	uint8_t macAddr[6];
	WiFi.macAddress(macAddr);
	String out = "";
	out += String(macAddr[0], HEX);
	out += ":";
	out += String(macAddr[1], HEX);
	out += ":";
	out += String(macAddr[2], HEX);
	out += ":";
	out += String(macAddr[3], HEX);
	out += ":";
	out += String(macAddr[4], HEX);
	out += ":";
	out += String(macAddr[5], HEX);
	out.toUpperCase();
	return out;
}

String ConfigFile::getConfigWifiSsid() {
	return configWifiSsid;
}
String ConfigFile::getConfigWifiPass() {
	return configWifiPass;
}
String ConfigFile::getConfigWifiHost() {
	return configWifiHost;
}
String ConfigFile::getConfigWifiDhcp() {
	return configWifiDhcp;
}
String ConfigFile::getConfigWifiIpStaticIp() {
	return configWifiIpStaticIp;
}
String ConfigFile::getConfigWifiIpNetmask() {
	return configWifiIpNetmask;
}
String ConfigFile::getConfigWifiIpGateway() {
	return configWifiIpGateway;
}
String ConfigFile::getConfigAntMinerIp() {
	return configAntMinerIp;
}
String ConfigFile::getConfigCoindeskCurrency() {
	return configCoindeskCurrency;
}
String ConfigFile::getConfigPassAdmin() {
	return configPassAdmin;
}

void ConfigFile::setConfigWifiSsid(String newconf) {
	if (!newconf.equals(getConfigWifiSsid())) {
		writeToFile("/config/wifi/ssid.txt", newconf);
		configWifiSsid = newconf;
		setMustRestart();
	}
}
void ConfigFile::setConfigWifiPass(String newconf) {
	if (!newconf.equals(getConfigWifiPass())) {
		writeToFile("/config/wifi/pass.txt", newconf);
		configWifiPass = newconf;
		setMustRestart();
	}
}
void ConfigFile::setConfigWifiHost(String newconf) {
	if (!newconf.equals(getConfigWifiHost())) {
		writeToFile("/config/wifi/host.txt", newconf);
		configWifiHost = newconf;
		setMustRestart();
	}
}
void ConfigFile::setConfigWifiDhcp(String newconf) {
	if (!newconf.equals(getConfigWifiDhcp())) {
		writeToFile("/config/wifi/dhcp.txt", newconf);
		configWifiDhcp = newconf;
		setMustRestart();
	}
}
void ConfigFile::setConfigWifiIpStaticIp(String newconf) {
	if (!newconf.equals(getConfigWifiIpStaticIp())) {
		writeToFile("/config/wifi/ip_staticip.txt", newconf);
		configWifiIpStaticIp = newconf;
		setMustRestart();
	}
}
void ConfigFile::setConfigWifiIpNetmask(String newconf) {
	if (!newconf.equals(getConfigWifiIpNetmask())) {
		writeToFile("/config/wifi/ip_netmask.txt", newconf);
		configWifiIpNetmask = newconf;
		setMustRestart();
	}
}
void ConfigFile::setConfigWifiIpGateway(String newconf) {
	if (!newconf.equals(getConfigWifiIpGateway())) {
		writeToFile("/config/wifi/ip_gateway.txt", newconf);
		configWifiIpGateway = newconf;
		setMustRestart();
	}
}
void ConfigFile::setConfigAntMinerIp(String newconf) {
	if (!newconf.equals(getConfigAntMinerIp())) {
		writeToFile("/config/antminer/ip.txt", newconf);
		configAntMinerIp = newconf;
	}
}
void ConfigFile::setConfigCoindeskCurrency(String newconf) {
	if (!newconf.equals(getConfigCoindeskCurrency())) {
		writeToFile("/config/coindesk/currency.txt", newconf);
		configCoindeskCurrency = newconf;
	}
}
void ConfigFile::setConfigPassAdmin(String newconf) {
	if (!newconf.equals(getConfigPassAdmin())) {
		writeToFile("/pass/admin.txt", newconf);
		configPassAdmin = newconf;
	}
}