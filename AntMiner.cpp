#include "AntMiner.h"

AntMiner::AntMiner() {
	millisec = 0;
	coindeskmillisec = 0;
	errCount = 0;

	currency = "USD";
	btc = 0;

	totghs = 0; 
	ghs1 = 0; 
	ghs2 = 0; 
	ghs3 = 0;
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	fan1 = 0;
	fan2 = 0;
}

bool AntMiner::readCoindeskApi() {
	if (ConfigFile::getConfigCoindeskCurrency().length() == 0) return true;
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("[WARN] Could not get version (Wi-Fi error)");
		return false;
	}
	HTTPClient http;
	String url = "http://api.coindesk.com/v1/bpi/currentprice/";
	url += ConfigFile::getConfigCoindeskCurrency();
	url += "EUR.json";
	http.begin(url);
	int httpCode = http.GET();
	if (httpCode > 0) {
		if (httpCode == HTTP_CODE_OK) {
			String reply = http.getString();
			DynamicJsonBuffer jsonBuffer;
			JsonObject& json = jsonBuffer.parseObject(reply);
			if (json.success()) {
				JsonObject& bpi = json["bpi"][ConfigFile::getConfigCoindeskCurrency()];
				btc = bpi["rate_float"];
				currency = ConfigFile::getConfigCoindeskCurrency();
				return true;
			}
			else {
				Serial.print("[WARN] Wrong Coindesk JSON : ");
				Serial.println(reply);
			}
		}
		else {
			Serial.print("[WARN] Got wrong HTTP code while checking updates from Coindesk, error ");
			Serial.println(httpCode);
		}
	}
	else {
		Serial.print("[WARN] Could not get price from Coindesk, error ");
		Serial.println(http.errorToString(httpCode).c_str());
	}
	return false;
}

bool AntMiner::readFromApi() {
	WiFiClient client;
	if (client.connect(strdup(ConfigFile::getConfigAntMinerIp().c_str()), 4028)) {
		client.print("{\"command\": \"stats\"}");
		String reply = client.readStringUntil('\r');
		reply.replace("}{", "},{"); // Trickery for the broken JSON produced by bmminer
		DynamicJsonBuffer jsonBuffer;
		JsonObject& json = jsonBuffer.parseObject(reply);
		if (json.success()) {
			JsonObject& stats = json["STATS"][1];
			String temptotghs = stats["GHS 5s"].asString();
			String tempghs1 = stats["chain_rate6"].asString();
			String tempghs2 = stats["chain_rate7"].asString();
			String tempghs3 = stats["chain_rate8"].asString();
			totghs = temptotghs.toFloat();
			ghs1 = tempghs1.toFloat();
			ghs2 = tempghs2.toFloat();
			ghs3 = tempghs3.toFloat();
			temp1 = stats["temp6"];
			temp2 = stats["temp7"];
			temp3 = stats["temp8"];
			fan1 = stats["fan3"];
			fan2 = stats["fan6"];
			return true;
		}
		else {
			Serial.print("[WARN] Wrong JSON : ");
			Serial.println(reply);
		}
	}
	else {
		Serial.println("[WARN] Failed to read from the AntMiner");
	}
	errCount++;
	return false;
}

void AntMiner::refreshScreen() {
	LcdUtils::getLcd()->setCursor(0, 0);
	LcdUtils::getLcd()->write(7);
	LcdUtils::getLcd()->print(" ");
	LcdUtils::getLcd()->print("AntMiner");
	LcdUtils::getLcd()->print("  ");
	LcdUtils::getLcd()->print(totghs/1000);
	LcdUtils::getLcd()->write(1);
	LcdUtils::getLcd()->write(0);

	LcdUtils::getLcd()->setCursor(0, 1);
	LcdUtils::getLcd()->print(ghs1/1000);
	LcdUtils::getLcd()->write(1);
	LcdUtils::getLcd()->write(0);
	LcdUtils::getLcd()->print(" ");
	LcdUtils::getLcd()->print(temp1);
	LcdUtils::getLcd()->write(223);
	LcdUtils::getLcd()->print(" ");
	LcdUtils::getLcd()->write(2);
	LcdUtils::getLcd()->print(fan1);
	LcdUtils::getLcd()->print(" RPM");

	LcdUtils::getLcd()->setCursor(0, 2);
	LcdUtils::getLcd()->print(ghs2/1000);
	LcdUtils::getLcd()->write(1);
	LcdUtils::getLcd()->write(0);
	LcdUtils::getLcd()->print(" ");
	LcdUtils::getLcd()->print(temp2);
	LcdUtils::getLcd()->write(223);
	LcdUtils::getLcd()->print(" ");
	LcdUtils::getLcd()->write(3);
	LcdUtils::getLcd()->print(fan2);
	LcdUtils::getLcd()->print(" RPM");

	LcdUtils::getLcd()->setCursor(0, 3);
	LcdUtils::getLcd()->print(ghs3/1000);
	LcdUtils::getLcd()->write(1);
	LcdUtils::getLcd()->write(0);
	LcdUtils::getLcd()->print(" ");
	LcdUtils::getLcd()->print(temp3);
	LcdUtils::getLcd()->write(223);
	LcdUtils::getLcd()->print(" ");
	if (ConfigFile::getConfigCoindeskCurrency().length() > 0) {
		LcdUtils::getLcd()->write(6);
		LcdUtils::getLcd()->print(round(btc));
		if (currency == "EUR") {
			LcdUtils::getLcd()->print(" ");
			LcdUtils::getLcd()->write(4);
		}
		else if (currency == "USD" || currency == "CAD" || currency == "AUD") {
			LcdUtils::getLcd()->print(" ");
			LcdUtils::getLcd()->print("$");
		}
		else if (currency == "GBP") {
			LcdUtils::getLcd()->print(" ");
			LcdUtils::getLcd()->write(5);
		}
		else {
			LcdUtils::getLcd()->print(ConfigFile::getConfigCoindeskCurrency());
		}
	}
}

void AntMiner::errorScreen() {
	LcdUtils::drawTitle();
	LcdUtils::getLcd()->setCursor(0, 1);
	LcdUtils::getLcd()->print("A JSON error occured");
	LcdUtils::getLcd()->setCursor(0, 2);
	LcdUtils::getLcd()->print("Check settings, WiFi");
	LcdUtils::getLcd()->setCursor(0, 3);
	LcdUtils::getLcd()->print("and the AntMiner.   ");
}

void AntMiner::doTick() {
	if (millis() - millisec > 2500 || millis() < millisec) {
		millisec = millis();
		if (readFromApi()) {
			refreshScreen();
		}
		else if (errCount >= 10) {
			errorScreen();
		}
	}
	else if (millis() - coindeskmillisec > 30000 || millis() < coindeskmillisec) {
		coindeskmillisec = millis();
		readCoindeskApi();
	}
}