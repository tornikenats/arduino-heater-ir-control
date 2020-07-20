# Arduino Heater IR Control

Use a thermometer and IR LED to turn on and off an electric heater based on temperature.

## Parts used

1. OLED Screen - SSD1306
2. Thermometer - DH22
3. IR LED - 5mm 940nm IR


## Code structure

Here is how it works: 

Every second, we check the temperature. If the temperature is below our set temperature, we turn on the heater. 

After the temperature exceed our set temperature, we turn off the heater and go into "cooldown mode". When in this mode we don't turn on the heater if we go below our set temperature. Instead, we wait until it goes a set threshold below and then turn it on again. We do this so we aren't repeatedly turning off and on our heater.

Here is an example:

* We set out target temperature to 22*C and our threshold at 1*C
* Room is 20*C
* Heater turns on since we are below 22*C
* Room is now 22.1*C, we turn off heater
* Room goes below 22*C
* Room goes below 21*C, we turn on the heater since we are below target minus threshold (22-1)*C
* Room goes above 22*C, heater turns off
* Repeat