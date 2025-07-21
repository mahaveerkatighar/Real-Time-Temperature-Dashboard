#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

// Wi-Fi credentials
#define WIFI_SSID "BrainBox"
#define WIFI_PASSWORD "wifi_pass"

// Firebase credentials
#define API_KEY "AIzaSyCtUi3T0EfzYVipWZ524CDduOxQ6zOZxsksk"
#define DATABASE_URL "https://temperature-detection-101bc-default-rtdb.firebaseio.com"
#define USER_EMAIL "your_@gmail.com"
#define USER_PASSWORD "xxxxxxxxxxx"

// DS18B20 setup
#define ONE_WIRE_BUS 4  // D2 (GPIO4)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// NTP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // IST (UTC + 5:30)

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Wi-Fi Connected!");

  // Firebase setup
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // DS18B20 sensor
  sensors.begin();

  // NTP time
  timeClient.begin();
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  setTime(timeClient.getEpochTime()); // Initialize TimeLib
}

void loop() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);

  timeClient.update();  // Refresh NTP time
  setTime(timeClient.getEpochTime());

  Serial.println("\n🌡️ Temperature Readings:");
  Serial.print("Celsius: ");
  Serial.println(temperatureC);
  Serial.print("Fahrenheit: ");
  Serial.println(temperatureF);

  // Construct timestamp path like "2025-07-20/12:35:12"
  String path = "/temperature/history/";
  path += String(year()) + "-";
  path += (month() < 10 ? "0" : "") + String(month()) + "-";
  path += (day() < 10 ? "0" : "") + String(day()) + "/";
  path += (hour() < 10 ? "0" : "") + String(hour()) + ":";
  path += (minute() < 10 ? "0" : "") + String(minute()) + ":";
  path += (second() < 10 ? "0" : "") + String(second());

  // Upload to Firebase
  if (Firebase.ready()) {
    if (Firebase.RTDB.setFloat(&fbdo, path + "/Celsius", temperatureC)) {
      Serial.println("✅ Celsius uploaded");
    } else {
      Serial.print("❌ Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, path + "/Fahrenheit", temperatureF)) {
      Serial.println("✅ Fahrenheit uploaded");
    } else {
      Serial.print("❌ Error: ");
      Serial.println(fbdo.errorReason());
    }
  }

  delay(5000);
}
