#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>  // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 2       // what pin we're connected to
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// lowest and highest sensor readings:
const int sensorMin = 0;     //  sensor minimum
const int sensorMax = 1024;  // sensor maximum


const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)
#define CHAT_ID "XXXXXXXXXX"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  // initialize serial communication @ 9600 baud:
  Serial.begin(115200);

  configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
  client.setTrustAnchors(&cert);     // Add root certificate for api.telegram.org
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    Serial.println(WiFi.localIP());
  }
  bot.sendMessage(CHAT_ID, "System has Started!!", "");

  dht.begin();
}
void loop() {

  // read the sensor on analog A0:
  int sensorReading = analogRead(A0);
  // map the sensor range (four options):
  // ex: 'long  int map(long int, long int, long int, long int, long int)'
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

  // range value:
  switch (range) {
    case 0:  // A fire closer than 1.5 feet away.
      Serial.println("** Close  Fire **");
      break;
    case 1:  // A fire between 1-3 feet away.
      Serial.println("**  Distant Fire **");
      break;
    case 2:  // No fire detected.
      Serial.println("No  Fire");
      break;
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");

  bot.sendMessage(CHAT_ID, "test", "");

  delay(1000);  // delay between reads
}