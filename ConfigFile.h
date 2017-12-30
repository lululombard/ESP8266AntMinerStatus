#ifndef ConfigFile_h
#define ConfigFile_h

#include <ESP8266WiFi.h>
#include "LcdUtils.h";
#include "FS.h"

class ConfigFile {
	private:
		static bool mount;
		static bool mustrestart;
		static String configWifiSsid;
		static String configWifiPass;
		static String configWifiHost;
		static String configWifiDhcp;
		static String configWifiIpStaticIp;
		static String configWifiIpNetmask;
		static String configWifiIpGateway;
		static String configAntMinerIp;
		static String configCoindeskCurrency;
		static String configPassAdmin;
	public:
		static void begin();
		static void checkForDefaultConfig();
		static void resetConfig();
		static void loadConfig();
		static String readFileContents(char*);
		static void createDefaultValue(char*, char*);
		static void writeToFile(char*, char*);
		static void writeToFile(char*, String);
		static bool getMustRestart();
		static void setMustRestart();
		static String getMac();
		static String getConfigWifiSsid();
		static String getConfigWifiPass();
		static String getConfigWifiHost();
		static String getConfigWifiDhcp();
		static String getConfigWifiIpStaticIp();
		static String getConfigWifiIpNetmask();
		static String getConfigWifiIpGateway();
		static String getConfigAntMinerIp();
		static String getConfigCoindeskCurrency();
		static String getConfigPassAdmin();
		static void setConfigWifiSsid(String);
		static void setConfigWifiPass(String);
		static void setConfigWifiHost(String);
		static void setConfigWifiDhcp(String);
		static void setConfigWifiIpStaticIp(String);
		static void setConfigWifiIpNetmask(String);
		static void setConfigWifiIpGateway(String);
		static void setConfigAntMinerIp(String);
		static void setConfigCoindeskCurrency(String);
		static void setConfigPassAdmin(String);
};

#endif