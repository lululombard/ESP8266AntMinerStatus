# ESP8266AntMinerStatus
A software to monitor an AntMiner from Bitmain for the ESP8266.
Tested with an AntMiner S9 (January 2018 batch). Should work with any ccminer compatible hardware if AntMiner.cpp complies to your needs.

# Features
* Displays global hasrate of the miner
* Displays the hashrate and temp of each board
* Displays the RPM of both fans
* Displays the current Bitcoin price from CoinDesk (can be disabled)
* Captive web portal to configure everything

# Hardware needed
* ESP8266
* A 20x4 I2C LCD Display (default SDA/SCL pins)
* Wi-Fi router on the same LAN as your AntMiner

# Installation
Use the [ESP8266 core for Arduino](https://github.com/esp8266/Arduino) and flash the sketch with it.

# Configuration
* The ESP8266 broadcasts a Wi-Fi network ("AntMiner Status" by default). Connect to that Wi-Fi, a captive portal should open. If not, open your browser to 192.168.4.1.
* Login with admin/antminer
* Set everything up, **don't forget to change the default password**
* Reboot the ESP
* Done.

# Screenshots
![Config status page](https://i.imgur.com/JDwoEm9.png)
![LCD screen](https://i.imgur.com/RQpFZCU.jpg)

# Dependencies
* [fdebrabander's I2C LCD library](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

# Planned features
* Hideable SSID
* Change the coin displayed on the LCD (DGB instead of BTC for instance)

# Donations
Donations are welcome in the following currencies :
* BTC : 34Pn2i3v4FnwgEMfQq9mAHktkVpNxgcYgj
* BCH : 1JaXNHsCSv4KwU5mY8qCCgnJj7sKuxPxF5
* DGB : DMMnWxmyp4iU1TKCV1eTNpspmNWtRBWeLC
* [PayPal](https://www.paypal.me/lululombardfr)
