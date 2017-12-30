#include "HttpPortal.h"

ESP8266WebServer server(80);

HttpPortal::HttpPortal() {
	server.on("/logo.svg", handleLogo);
	server.on("/style.css", handleStyle);
	server.onNotFound(handleDefault);
	const char* headerkeys[] = {"Cookie"} ;
	size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
	server.collectHeaders(headerkeys, headerkeyssize);
	server.begin();
	if (!Hotspot::isFailure()) LcdUtils::clearScreen();
}

void HttpPortal::doTick() {
	server.handleClient();
}

void HttpPortal::handleDefault() {
	unsigned long millistart = millis();
	int level = getLvl();
	if (server.method() == HTTP_POST) {
		if (server.hasArg("logout")) {
			String header = "HTTP/1.1 301 OK\r\nSet-Cookie: lvl=0;\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
			server.sendContent(header);
			return;
		}
		if (server.hasArg("reset")) ConfigFile::resetConfig();
		if (server.hasArg("reboot")) {
			String out = "";
			out += getHeader(0);
			out += "\t<div id=\"login\" class=\"box\">\n";
			out += "\t\t<h1>Rebooting...</h1>\n";
			out += "\t\t<p>The monitoring device is restarting... Please check the LCD to know when it will be back up (less than 60 sec)</p>\n";
			out += "\t</div>\n";
			out += getFooter(0);
			server.send(200, "text/html", out);
			for (int i = 0; i <= 1000; i++) {
				server.handleClient();
				delay(1);
			}
			ESP.restart();
			return;
		}
		if (server.hasArg("user") && server.hasArg("pass")) {
			level = -1;
			if (server.arg("user").equalsIgnoreCase("admin") && server.arg("pass") == ConfigFile::getConfigPassAdmin()) level = 1;
			String header = "HTTP/1.1 301 OK\r\nSet-Cookie: lvl=%lvl%;\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
			header.replace("%lvl%", String(level));
			server.sendContent(header);
			return;
		}
		for (uint8_t i = 0; i < server.args(); i++) {
			if (server.argName(i).equals("wifissid")) ConfigFile::setConfigWifiSsid(server.arg(i));
			if (server.argName(i).equals("wifipass")) ConfigFile::setConfigWifiPass(server.arg(i));
			if (server.argName(i).equals("wifihost")) ConfigFile::setConfigWifiHost(server.arg(i));
			if (server.argName(i).equals("wifidhcp")) ConfigFile::setConfigWifiDhcp(server.arg(i));
			if (server.argName(i).equals("wifiipstaticip")) ConfigFile::setConfigWifiIpStaticIp(server.arg(i));
			if (server.argName(i).equals("wifiipnetmask")) ConfigFile::setConfigWifiIpNetmask(server.arg(i));
			if (server.argName(i).equals("wifiipgateway")) ConfigFile::setConfigWifiIpGateway(server.arg(i));
			if (server.argName(i).equals("antminerip")) ConfigFile::setConfigAntMinerIp(server.arg(i));
			if (server.argName(i).equals("coindeskcurrency")) ConfigFile::setConfigCoindeskCurrency(server.arg(i));
			if (server.argName(i).equals("passadmin")) ConfigFile::setConfigPassAdmin(server.arg(i));
		}
		String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}

	String header = "HTTP/1.1 200 OK\r\nCache-Control: no-cache\r\nContent-Type: text/html\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
	server.sendContent(header);
	server.sendContent(getHeader(level));
	server.sendContent(getLogin(level));
	server.sendContent(getButtons(level));
	server.sendContent(getAdminConfigP1(level));
	server.sendContent(getAdminConfigP2(level));
	server.sendContent(getAdminConfigP3(level));
	server.sendContent(getFooter(level));
}

String HttpPortal::getHeader(int level) {
	String out = "";
	out += "<!DOCTYPE html>\n";
	out += "<html>\n";
	out += "<head>\n";
	out += "\t<meta charset=\"UTF-8\"/>\n";
	out += "\t<title>AntMiner Status</title>\n";
	out += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
	out += "\t<link href=\"/style.css\" rel=\"stylesheet\" type=\"text/css\"/>\n";
	out += "</head>\n";
	out += "<body>\n";
	out += "\t<div id=\"header\">\n";
	out += "\t\t<ul id=\"menu\">\n";
	out += "\t\t\t<li><span class=\"nohover\"><b><img height=\"12px\" src=\"/logo.svg\" alt=\"AntMiner Status Logo\"/> AntMiner Status</b></span></li>\n";
	out += "\t\t\t<li><span class=\"nohover\">Mining Monitor</span></li>\n";
	out += "\t\t</ul>\n";
	out += "\t</div>\n";
	out += "\t<div id=\"content\">\n";
	return out;
}

String HttpPortal::getLogin(int level) {
	String out = "";
	if (level < 1) {
		out += "\t<form method=\"POST\">\n";
		out += "\t\t<div id=\"reboot\" class=\"box\">\n";
		out += "\t\t\t<h1>Login</h1>\n";
		if (level < 0) out += "\t\t\t<p class=\"error\">Please check your credentials</p>\n";
		out += "\t\t\t<input name=\"user\" type=\"text\" placeholder=\"Login\"/>\n";
		out += "\t\t\t<input name=\"pass\" type=\"password\" placeholder=\"Password\"/>\n";
		out += "\t\t\t<input type=\"submit\" value=\"Login\"/>\n";
		out += "\t\t</div>\n";
		out += "\t</form>\n";
	}
	return out;
}

String HttpPortal::getButtons(int level) {
	String out = "";
	if (level >= 1) {
		out += "\t<form class=\"redbutton\" method=\"POST\">\n";
		out += "\t\t<input name=\"logout\" value=\"true\" type=\"hidden\"/>\n";
		out += "\t\t<input type=\"submit\" value=\"Logout\"/>\n";
		out += "\t</form>\n";
		out += "\t<form class=\"redbutton\" method=\"POST\">\n";
		out += "\t\t<input name=\"reset\" value=\"true\" type=\"hidden\"/>\n";
		out += "\t\t<input type=\"submit\" value=\"Reset settings\"/>\n";
		out += "\t</form>\n";
		out += "\t<form class=\"redbutton\" method=\"POST\">\n";
		out += "\t\t<input name=\"reboot\" value=\"true\" type=\"hidden\"/>\n";
		out += "\t\t<input type=\"submit\" value=\"Reboot\"/>\n";
		out += "\t</form>\n";
		if (ConfigFile::getMustRestart()) {
			out += "\t<div id=\"reboot\" class=\"box error\">\n";
			out += "\t\t<h1>Warning</h1>\n";
			out += "\t\t<p>You need to reboot this device to apply changes</p>\n";
			out += "\t</div>\n";
		}
	}
	return out;
}

String HttpPortal::getAdminConfigP1(int level) {
	String out = "";
	if (level >= 1) {
		out += "\t<form method=\"POST\">\n";
		out += "\t\t<div class=\"box\">\n";
		out += "\t\t\t<h1>Configuration</h1>\n";
		out += "\t\t\t<p><label for=\"wifissid\">SSID</label> <input required placeholder=\"SSID to connect to (same network as AntMiner)\" type=\"text\" name=\"wifissid\" value=\"" + ConfigFile::getConfigWifiSsid() + "\"/></p>\n";
		out += "\t\t\t<p><label for=\"wifipass\">Wi-Fi passphrase</label> <input placeholder=\"Wi-Fi passphrase (leave empty if none)\" type=\"text\" name=\"wifipass\" value=\"" + ConfigFile::getConfigWifiPass() + "\"/></p>\n";
		out += "\t\t\t<p><label for=\"wifihost\">Hostname</label> <input placeholder=\"The hostname of this device and its hotspot\" type=\"text\" name=\"wifihost\" value=\"" + ConfigFile::getConfigWifiHost() + "\"/></p>\n";
		out += "\t\t\t<p><label for=\"wifidhcp\">DHCP</label> <select name=\"wifidhcp\" id=\"wifidhcp\" onchange=\"updateDhcpSelector();\"><option value=\"1\"";
		if (ConfigFile::getConfigWifiDhcp()) out += " selected=\"selected\"";
		out += ">Yes</option><option value=\"0\"";
		if (!ConfigFile::getConfigWifiDhcp()) out += " selected=\"selected\"";
		out += ">No</option></select></p>\n";
		out += "\t\t\t<p id=\"wifiipstaticip\"";
		if (ConfigFile::getConfigWifiDhcp()) out += " class=\"hide\"";
		out += "><label for=\"wifiipstaticip\">Static IP</label> <input placeholder=\"IP address\" type=\"text\" name=\"wifiipstaticip\" value=\"" + ConfigFile::getConfigWifiIpStaticIp() + "\"/></p>\n";
		out += "\t\t\t<p id=\"wifiipnetmask\"";
		if (ConfigFile::getConfigWifiDhcp()) out += " class=\"hide\"";
		out += "><label for=\"wifiipnetmask\">IP mask</label> <input placeholder=\"Mask (ex 255.255.255.0)\" type=\"text\" name=\"wifiipnetmask\" value=\"" + ConfigFile::getConfigWifiIpNetmask() + "\"/></p>\n";
		out += "\t\t\t<p id=\"wifiipgateway\"";
		if (ConfigFile::getConfigWifiDhcp()) out += " class=\"hide\"";
		out += "><label for=\"wifiipgateway\">Gateway</label> <input placeholder=\"Gateway IP\" type=\"text\" name=\"wifiipgateway\" value=\"" + ConfigFile::getConfigWifiIpGateway() + "\"/></p>\n";
	}
	return out;
}

String HttpPortal::getAdminConfigP2(int level) {
	String out = "";
	if (level >= 1) {
		out += "\t\t\t<script>\n";
		out += "\t\t\t\tfunction updateDhcpSelector() {\n";
		out += "\t\t\t\t\tvar d = document.getElementById(\"wifidhcp\");\n";
		out += "\t\t\t\t\tvar value = (d.options[d.selectedIndex].value == \"1\" ? true : false);\n";
		out += "\t\t\t\t\tshow(\"wifiipstaticip\", !value);\n";
		out += "\t\t\t\t\tshow(\"wifiipnetmask\", !value);\n";
		out += "\t\t\t\t\tshow(\"wifiipgateway\", !value);\n";
		out += "\t\t\t\t}\n";
		out += "\t\t\t\tfunction show(id, value) {\n";
		out += "\t\t\t\t\tvar d = document.getElementById(id);\n";
		out += "\t\t\t\t\tif (value) d.className=\"\";\n";
		out += "\t\t\t\t\telse d.className=\"hide\";\n";
		out += "\t\t\t\t}\n";
		out += "\t\t\t</script>\n";
	}
	return out;
}

String HttpPortal::getAdminConfigP3(int level) {
	String out = "";
	if (level >= 1) {
		out += "\t\t\t<p><label for=\"antminerip\">AntMiner IP</label> <input type=\"text\" name=\"antminerip\" value=\"" + ConfigFile::getConfigAntMinerIp() + "\"/></p>\n";
		out += "\t\t\t<p><label for=\"coindeskcurrency\">Currency (must be supported by coinbase, empty to disable)</label> <input type=\"text\" name=\"coindeskcurrency\" value=\"" + ConfigFile::getConfigCoindeskCurrency() + "\"/></p>\n";
		out += "\t\t\t<p><label for=\"passadmin\">Admin password</label> <input type=\"password\" name=\"passadmin\" value=\"" + ConfigFile::getConfigPassAdmin() + "\"/></p>\n";
		out += "\t\t\t<p><input type=\"submit\" value=\"Save settings\"/></p>\n";
		out += "\t\t</div>\n";
	}
	return out;
}

String HttpPortal::getFooter(int level) {
	String out = "";
	if (level >= 1) out += "\t</form>\n";
	out += "\t</div>\n";
	out += "\t<div id=\"footer\">\n";
	out += "\t<p><a href=\"https://github.com/lululombard/ESP8266AntMinerStatus\">GitHub</a></p>\n";
	out += "\t</div>\n";
	out += "</body>\n";
	out += "</html>\n";
	return out;
}

int HttpPortal::getLvl() {
	if (server.hasHeader("Cookie")) {
		String cookie = server.header("Cookie");
		if (cookie.indexOf("lvl=-1") != -1) return -1;
		if (cookie.indexOf("lvl=0") != -1) return 0;
		if (cookie.indexOf("lvl=1") != -1) return 1;
		return 0;
	}
	return 0;
}

void HttpPortal::handleLogo() {
	String out = "";
	out += "<svg version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\" width=\"77.000000pt\" height=\"82.000000pt\" viewBox=\"0 0 77.000000 82.000000\" preserveAspectRatio=\"xMidYMid meet\">";
	out += "<g transform=\"translate(0.000000,82.000000) scale(0.100000,-0.100000)\" fill=\"#000000\" stroke=\"none\">";
	out += "<path d=\"M240 780 c-11 -11 -20 -33 -20 -49 0 -16 -12 -54 -26 -85 -47 -103 -43 -257 8 -359 l19 -38 -41 -46 c-23 -27 -46 -67 -55 -96 -27 -91 -31 -87 104 -87 l119 0 6 35 c9 47 47 75 103 75 51 0 90 -31 99 -80 6 -29 9 -30 56 -30 l50 0 -7 43 c-20 118 -107 212 -219 233 l-38 7 4 76 c7 96 31 161 63 161 50 0 73 72 35 110 -45 45 -112 9 -109 -58 1 -20 -5 -51 -13 -67 -17 -32 -36 -123 -41 -189 -2 -35 -6 -41 -31 -47 -39 -11 -50 -1 -72 69 -24 77 -22 180 6 255 17 46 26 57 43 57 38 0 67 28 67 64 0 60 -68 88 -110 46z\"/>";
	out += "</g>";
	out += "</svg>";
	server.sendHeader("Cache-Control", "max-age=86400");
	server.send(200, "image/svg+xml", out);
}

void HttpPortal::handleStyle() {
	String out = "";
	out += "* {\n";
	out += "\tmargin: 0;\n";
	out += "\tpadding: 0;\n";
	out += "\tborder-radius: 0;\n";
	out += "\t-webkit-appearance: none;\n";
	out += "}\n";
	out += "body {\n";
	out += "\tbackground: #444;\n";
	out += "\tcolor: #fff;\n";
	out += "\tfont-family: \"Arial\";\n";
	out += "}\n";
	out += "a, a:visited, a:active, a:focus {\n";
	out += "\tcolor: #fff;\n";
	out += "}\n";
	out += "#menu {\n";
	out += "\tlist-style-type: none;\n";
	out += "\tmargin: 0;\n";
	out += "\tpadding: 0;\n";
	out += "\toverflow: hidden;\n";
	out += "\tbackground: #333;\n";
	out += "\tz-index: 1\n";
	out += "}\n";
	out += "#menu li {\n";
	out += "\tfloat: left;\n";
	out += "}\n";
	out += "#menu li span {\n";
	out += "\tdisplay: inline-block;\n";
	out += "\tcolor: #f2f2f2;\n";
	out += "\ttext-align: center;\n";
	out += "\tpadding: 14px 16px;\n";
	out += "\ttext-decoration: none;\n";
	out += "\ttransition: 0.3s;\n";
	out += "\tfont-size: 17px;\n";
	out += "\tcursor: default;\n";
	out += "}\n";
	out += "#menu li span:not(.nohover):hover {\n";
	out += "\tbackground-color: #555;\n";
	out += "\tcursor: pointer;\n";
	out += "}\n";
	out += "#content {\n";
	out += "\twidth: 80%;\n";
	out += "\tmargin: auto;\n";
	out += "}\n";
	out += ".box {\n";
	out += "\tposition: relative;\n";
	out += "\tz-index: 1;\n";
	out += "\tbackground: #333;\n";
	out += "\tmargin: 0 auto 100px;\n";
	out += "\tpadding: 5px;\n";
	out += "\ttext-align: center;\n";
	out += "\tbox-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);\n";
	out += "}\n";
	out += ".box h1 {\n";
	out += "\tmargin-bottom: 10px;\n";
	out += "}\n";
	out += "input, select {\n";
	out += "\toutline: 0;\n";
	out += "\tbackground: #555;\n";
	out += "\tcolor: #fff;\n";
	out += "\twidth: 50%;\n";
	out += "\tborder: 0;\n";
	out += "\tmargin: 0 0 15px;\n";
	out += "\tpadding: 15px;\n";
	out += "\tbox-sizing: border-box;\n";
	out += "\tfont-size: 14px;\n";
	out += "}\n";
	out += "label {\n";
	out += "\ttext-align: center;\n";
	out += "\twidth: 40%;\n";
	out += "\tdisplay: inline-block;\n";
	out += "}\n";
	out += "input[type=submit] {\n";
	out += "\twidth: 270px;\n";
	out += "\ttext-transform: uppercase;\n";
	out += "\toutline: 0;\n";
	out += "\tbackground: #4CAF50;\n";
	out += "\tborder: 0;\n";
	out += "\tfont-size: 14px;\n";
	out += "\t-webkit-transition: all 0.3 ease;\n";
	out += "\ttransition: all 0.3 ease;\n";
	out += "\tcursor: pointer;\n";
	out += "\tbox-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);\n";
	out += "}\n";
	out += "input[type=submit]:hover, input[type=submit]:active, input[type=submit]:focus {\n";
	out += "\tbackground: #43A047;\n";
	out += "}\n";
	out += ".redbutton input[type=submit] {\n";
	out += "\tbackground: #AF4C4C;\n";
	out += "}\n";
	out += ".redbutton input[type=submit]:hover, .redbutton input[type=submit]:active, .redbutton input[type=submit]:focus {\n";
	out += "\tbackground: #A04343;\n";
	out += "}\n";
	out += "#login, #reboot {\n";
	out += "\tmax-width: 360px;\n";
	out += "\tpadding: 45px;\n";
	out += "\tmargin: 50px auto;\n";
	out += "}\n";
	out += "#login input, #reboot input {\n";
	out += "\twidth: 100%;\n";
	out += "}\n";
	out += ".redbutton input {\n";
	out += "\twidth: 270px;\n";
	out += "\tdisplay: block;\n";
	out += "\tmargin: 25px auto;\n";
	out += "}\n";
	out += ".error {\n";
	out += "\tcolor: #a00;\n";
	out += "}\n";
	out += ".hide {\n";
	out += "\tdisplay: none;\n";
	out += "}\n";
	out += ".box .error {\n";
	out += "\tmargin: 10px;\n";
	out += "\tcolor: #a00;\n";
	out += "\tfont-size: 12px;\n";
	out += "}\n";
	out += "#footer {\n";
	out += "\tbackground: #333;\n";
	out += "\ttext-align: center;\n";
	out += "\tposition: fixed;\n";
	out += "\tbottom: 0;\n";
	out += "\twidth: 100%;\n";
	out += "\tfont-size: 8;\n";
	out += "\tline-height: 16px;\n";
	out += "\tz-index: 1\n";
	out += "}\n";
	out += "@media screen and (max-width:680px) {\n";
	out += "\t#menu li:not(:first-child) {\n";
	out += "\t\tdisplay: none;\n";
	out += "\t}\n";
	out += "\t#content {\n";
	out += "\t\twidth: 95%;\n";
	out += "\t}\n";
	out += "\t .box input, .box select, label {\n";
	out += "\t\twidth: 100%;\n";
	out += "\t}\n";
	out += "}\n";
	server.sendHeader("Cache-Control", "max-age=86400");
	server.send(200, "text/css", out);
}