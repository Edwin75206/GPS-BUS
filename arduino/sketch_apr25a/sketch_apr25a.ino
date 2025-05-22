#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Pines RX/TX para GPS en UART2
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);  // UART2

// — Tus credenciales Wi-Fi y endpoint de tu API —
const char* ssid     = "INFINITUM1CF5";
const char* password = "Hhq6LxrXgq";
const char* endpoint = "http://192.168.1.105:3000/api/locations";

void setup() {
  Serial.begin(115200);
  delay(100);

  // Inicializa la UART2 para el GPS
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 (GPS) iniciado a 9600 baudios");

  // Conecta a Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado, IP: " + WiFi.localIP().toString());
  Serial.println("→ Enviando POST a: " + String(endpoint));

}

void loop() {
  // 1) Lee NMEA durante 1s para alimentar al parser
  unsigned long start = millis();
  while (millis() - start < 1000) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
  }

  // 2) Cuando hay nueva posición válida…
  if (gps.location.isUpdated()) {
    // — Imprime info por Serial —
    Serial.print("LAT: ");  Serial.println(gps.location.lat(), 6);
    Serial.print("LNG: ");  Serial.println(gps.location.lng(), 6);
    Serial.print("Speed (km/h): "); Serial.println(gps.speed.kmph());
    Serial.print("Alt (m): ");       Serial.println(gps.altitude.meters());
    Serial.print("Satélites: ");     Serial.println(gps.satellites.value());
    Serial.print("UTC: ");
    Serial.print(gps.date.year());   Serial.print("/");
    Serial.print(gps.date.month());  Serial.print("/");
    Serial.print(gps.date.day());    Serial.print(" ");
    Serial.print(gps.time.hour());   Serial.print(":");
    Serial.print(gps.time.minute()); Serial.print(":");
    Serial.println(gps.time.second());
    Serial.println();

    // — Construye JSON —
    StaticJsonDocument<200> doc;
    doc["deviceId"] = "esp32-001";
    doc["lat"]      = gps.location.lat();
    doc["lng"]      = gps.location.lng();
    doc["speed"]    = gps.speed.kmph();
    doc["alt"]      = gps.altitude.meters();
    // timestamp en ISO (UTC)
    char buf[25];
    snprintf(buf, sizeof(buf), "%04u-%02u-%02uT%02u:%02u:%02uZ",
             gps.date.year(), gps.date.month(), gps.date.day(),
             gps.time.hour(), gps.time.minute(), gps.time.second());
    doc["timestamp"] = buf;

    String body;
    serializeJson(doc, body);
    Serial.println("→ Enviando POST:");
    Serial.println(body);

    // — Envía al servidor —
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(endpoint);
      http.addHeader("Content-Type", "application/json");
      int statusCode = http.POST(body);
      String resp = http.getString();
      Serial.printf("HTTP %d, respuesta: %s\n\n", statusCode, resp.c_str());
      http.end();
    } else {
      Serial.println("❌ WiFi desconectado\n");
    }
  }
}
