// Libraries
#include <ACROBOTIC_SSD1306.h>
#include <DHT.h>
#include <IRremote.h>
#include <Wire.h>

// Constants
#define DHTPIN 7      // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22  (AM2302)

// My heater's remote control codes
#define POWER 0x807F48B7
#define UP_TEMP 0x807FB847
#define DOWN_TEMP 0x807F9867

// Global Variables
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float hum;                // Stores humidity value
float temp;               // Stores temperature value

IRsend irsend;
bool heat_on = false;
float target_temp = 22;
float buffer_below = .5;
bool post_heating = false;
bool refresh = false;

void setup()
{
    // Initialze SSD1306 OLED display
    Wire.begin();
    oled.init();
    oled.clearDisplay();

    // DHT22
    dht.begin();
}

void update_screen(float temp, float hum)
{
    //  display
    oled.setTextXY(0, 0);
    oled.putString("Temp: " + String(temp) + "C");

    oled.setTextXY(1, 0);
    oled.putString("Humi: " + String(hum) + "%");
    oled.setTextXY(2, 0);
    oled.putString("Heat: " + String(heat_on) +
                   ", Post: " + String(post_heating));

    oled.setTextXY(3, 0);
    if (refresh)
    {
        oled.putString(".");
        refresh = false;
    }
    else
    {
        oled.putString(" ");
        refresh = true;
    }
}

void turn_on_heat()
{
    irsend.sendNEC(POWER, 32);
    heat_on = true;
}

void turn_off_heat()
{
    irsend.sendNEC(POWER, 32);
    heat_on = false;
}

void loop()
{
    // do 1 minute delay at the top so we have time to position device
    delay(1000);

    // Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    // Write status to screen
    update_screen(temp, hum);

    // if we are not waiting for buffering and the temp is low
    if (temp < target_temp && !heat_on && !post_heating)
    {
        turn_on_heat();
    }
    // if the temp has reached the target temp
    else if (temp >= target_temp && heat_on)
    {
        turn_off_heat();
        post_heating = true;
    }
    // if we had been heating and are waiting for temp to go down
    else if (temp < target_temp - buffer_below && post_heating)
    {
        turn_on_heat();
        post_heating = false;
    }
}
