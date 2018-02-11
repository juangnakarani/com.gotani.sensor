//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"mgu", "sen", "sel", "rab", "kam", "jum", "sab"};

//define button
const int buttonPin = 3;
const int ledPin =  13;
int buttonState = 0;
int backlightDelayed = 100;
bool backlightButton = false;
void setup()
{

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(57600);
  dht.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  lcd.init();                      // initialize the lcd
  //  lcd.backlight();
}


void loop()
{
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    backlightButton = true;
    lcd.backlight();
  }

  if (backlightButton == true) {
    backlightDelayed--;
    if (backlightDelayed == 0) {
      lcd.noBacklight();
      backlightButton = false;
      backlightDelayed = 100;
    }
  }
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.print("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);


  DateTime now = rtc.now();

  char buffer[20] = "";

  //  sprintf(buffer, "%s,%s/%s/%s %02d:%02d:%02d", daysOfTheWeek[now.dayOfTheWeek()], now.day(),now.month(),now.year(), now.hour(), now.minute(), now.second());
  sprintf(buffer, "%s,%02d/%02d/%02d %02d:%02d", daysOfTheWeek[now.dayOfTheWeek()], now.day(), now.month(), now.year(), now.hour(), now.minute());
  //  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(buffer);

  lcd.setCursor(0, 1);
  lcd.print("lembab:");
  lcd.setCursor(7, 1);
  lcd.print(h);
  lcd.setCursor(0, 2);
  lcd.print("suhu  :");
  lcd.setCursor(7, 2);
  lcd.print(t);

  lcd.setCursor(0, 3);
  lcd.print("status:normal");
  //  delay(1000);
}
