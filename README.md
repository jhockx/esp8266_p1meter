# esp8266_p1meter

Software for the ESP2866 (Wemos D1 Mini/NodeMcu etc.) that sends P1 smart meter (DSMR) data to a MQTT broker, with the possibility for Over The Air (OTA) firmware updates.

## About this fork
The original project of [fliphess](https://github.com/fliphess/esp8266_p1meter) has issues with DSMR 5.0 meters, which send telegrams every 1 second at a high 115200 baud rate. 
This causes the used SoftwareSerial to struggle to keep up and thus only receives corrupted messages. 

The project of [daniel-jong](https://github.com/daniel-jong/esp8266_p1meter) switches to using the main hardware serial port (RX) for communication with the p1 meter and is tested on the `Landys and Gyr E360` smartmeter (DSMR 5.0).

Then I noticed the project of [WhoSayIn](https://github.com/WhoSayIn/esp8266_dsmr2mqtt), that takes a much more minimalistic approach, which I liked. However, I discovered this project was also designed for the DSMR 4.0 meters.

**With this fork, I want to accomplish the following:**
- Combine the projects mentioned above in a minimalistic setup for the newer DSMR 5.0 smart meters (at the time of writing: 476 vs 681 lines of code, tested on the `ISKRA AM550`).
- Improve the code (also thanks to [RoySpringer](https://github.com/RoySpringer)).
- Separate code in multiple files for readability (the Arduino IDE loads and compiles these in alphabetic order after the project main .ino file).
- Add solar panel meter: read out delivered energy.
- In a previous version of the code, I had trouble getting the CRC to work. This might be the case for you too at some point in the future, so I have built an option in the settings to turn this on and off. Most of the time it worked fine for me without the CRC, but it had occasional "drops" in the data, either back to a lower value or all the way to zero. This is very inconvenient when working with the [Utility Meter](https://www.home-assistant.io/integrations/utility_meter/) of Home Assistant, so I have built extra data quality checks for this. However, since the CRC now seems to be working, I have removed the code. If you do experience this problem and you want to have the custom DQ checks build in your code, then find the method `getValueWithDqCheck` in the master branch at the commit [3c9311975521f6e940884f00f72184ff8944cb24](https://github.com/jhockx/esp8266_p1meter/tree/3c9311975521f6e940884f00f72184ff8944cb24). Open a PR if you want it back in the code.

## Setup
This setup requires:
- An esp8266 (Wemos D1 mini has been tested)
- Small breadboard
- A 10k ohm resistor
- A 4 pin (RJ11) or [6 pin (RJ12) cable](https://www.tinytronics.nl/shop/nl/kabels/adapters/rj12-naar-6-pins-dupont-jumper-adapter). Both cables work great, but a 6 pin cable can also power the ESP8266 on most DSMR5+ meters.

Setting up your Arduino IDE:
- Ensure you have selected the right board (you might need to install your esp8266board in the Arduino IDE).
- I have tested this on the 80 MHz and 160 MHz CPU frequency mode, pick either one.
- Using the Tools->Manage Libraries... install `PubSubClient`.
- In the file `Settings.h` change all values accordingly
- Write to your device via USB the first time, you can do it OTA all times thereafter.

### Circuit diagram
_Note: I have only tested this on the `ISKRA AM550`._

Connect the ESP8266 to an RJ11 cable/connector following the diagram.

| P1 pin   | ESP8266 Pin |
| ----     | ---- |
| 2 - RTS  | 3.3v |
| 3 - GND  | GND  |
| 4 -      |      |
| 5 - RXD (data) | RX (gpio3) |

On most models a 10K resistor should be used between the ESP's 3.3v and the p1's DATA (RXD) pin. Many howto's mention RTS requires 5V (VIN) to activate the P1 port, but for me 3V3 suffices.

![Wiring](https://raw.githubusercontent.com/daniel-jong/esp8266_p1meter/master/assets/esp8266_p1meter_bb.png)

<details><summary>Optional: Powering the ESP8266 using your DSMR5+ meter</summary>
<p>
When using a 6 pin cable you can use the power source provided by the meter.
  
| P1 pin   | ESP8266 Pin |
| ----     | ---- |
| 1 - 5v out | 5v or Vin |
| 2 - RTS  | 3.3v |
| 3 - GND  | GND  |
| 4 -      |      |
| 5 - RXD (data) | RX (gpio3) |
| 6 - GND  | GND  |
  
![Wiring powered by meter](https://raw.githubusercontent.com/daniel-jong/esp8266_p1meter/master/assets/esp8266_p1meter_bb_PoweredByMeter.png)

</p>
</details>

### Data Sent

All metrics are send to their own MQTT topic.
The software sends out to the following MQTT topics:

```
sensors/power/p1meter/consumption_low_tarif
sensors/power/p1meter/consumption_high_tarif
sensors/power/p1meter/delivered_low_tarif
sensors/power/p1meter/delivered_high_tarif
sensors/power/p1meter/gas_meter_m3
sensors/power/p1meter/actual_consumption
sensors/power/p1meter/actual_received
sensors/power/p1meter/instant_power_usage_l1
sensors/power/p1meter/instant_power_usage_l2
sensors/power/p1meter/instant_power_usage_l3
sensors/power/p1meter/instant_power_current_l1
sensors/power/p1meter/instant_power_current_l2
sensors/power/p1meter/instant_power_current_l3
sensors/power/p1meter/instant_voltage_l1
sensors/power/p1meter/instant_voltage_l2
sensors/power/p1meter/instant_voltage_l3
sensors/power/p1meter/actual_tarif_group
sensors/power/p1meter/short_power_outages
sensors/power/p1meter/long_power_outages
sensors/power/p1meter/short_power_drops
sensors/power/p1meter/short_power_peaks
```

### Home Assistant Configuration

Checkout my [Home Assistant configuration repository](https://github.com/jhockx/home-assistant-configuration) for all my Home Assistant configurations, or just checkout [the setup of the MQTT sensors](https://github.com/jhockx/home-assistant-configuration/blob/master/GWL%20monitoring.md#electricitygas-usage-from-the-dsmr50). Feel free to adjust the configuration to your needs.

## Known limitations and issues
In case you power a Wemos D1 mini by the `ISKRA AM550` (and possibly other DSMR 5.0 meters), every so often the device might lose the connection to the MQTT server for a couple of seconds. I believe the device also resets itself, so this probably means that a temporary power loss is causing the loose connection (maybe the meter stops providing current for a second or something). Or it might be the case that your esp8266 device can't process and send the data fast enough (that's why it might be best to set the CPU to 160 MHz). You can check if your device disconnects every so often in the logs of your MQTT broker.

## Thanks to

As [fliphess](https://github.com/fliphess/esp8266_p1meter) thanked a few people, I want to list them here as well:
- https://github.com/jantenhove/P1-Meter-ESP8266
- https://github.com/neographikal/P1-Meter-ESP8266-MQTT
- http://gejanssen.com/howto/Slimme-meter-uitlezen/
- https://github.com/rroethof/p1reader/
- http://romix.macuser.nl/software.html
- http://blog.regout.info/category/slimmeter/
- http://domoticx.com/p1-poort-slimme-meter-hardware/

In addition, I'd like thank and refer to the following projects which served as a source of information:
- [https://github.com/daniel-jong/esp8266_p1meter](https://github.com/daniel-jong/esp8266_p1meter)
- [https://github.com/WhoSayIn/esp8266_dsmr2mqtt](https://github.com/WhoSayIn/esp8266_dsmr2mqtt)

Other sources:
- [DSMR 5.0 documentation](https://www.netbeheernederland.nl/_upload/Files/Slimme_meter_15_a727fce1f1.pdf)
