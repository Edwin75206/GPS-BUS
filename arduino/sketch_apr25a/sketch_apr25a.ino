#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// — TinyGPSPlus y Serial2 para ESP32 —
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);   // UART2

// — Pines RX/TX de tu módulo GPS Neo6M —  
//    RX del módulo al pin 16 del ESP32 (TX2)
//    TX del módulo al pin 17 del ESP32 (RX2)
const int GPS_RX_PIN = 17;
const int GPS_TX_PIN = 16;

// — Tus credenciales Wi-Fi y endpoint de tu API —
const char* ssid     = "INFINITUM1CF5";
const char* password = "Hhq6LxrXgq";
const char* endpoint = "http://192.168.1.100:3000/api/locations";

const unsigned long interval = 5000;  // 5 segundos
unsigned long lastSend = 0;

void setup() {
  // USB Serial para debug
  Serial.begin(115200);

  // Serial para GPS
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  delay(100);

  // Conecta a Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK, IP: " + WiFi.localIP().toString());
}

void loop() {
  // Parsea todo lo que llega del GPS
  while (SerialGPS.available()) {
    gps.encode(SerialGPS.read());
  }

  // Espera hasta el siguiente envío
  if (millis() - lastSend < interval) return;
  lastSend = millis();

  // Comprueba que hay fix válido
  if (!gps.location.isValid() || gps.location.age() > 5000) {
    Serial.println("⏳ Sin fix GPS válido");
    return;
  }

  float lat = gps.location.lat();
  float lng = gps.location.lng();
  unsigned long timestamp = millis();

  // Construye JSON
  StaticJsonDocument<200> doc;
  doc["deviceId"]  = "esp32-001";
  doc["lat"]       = lat;
  doc["lng"]       = lng;
  doc["timestamp"] = timestamp;
  String body;
  serializeJson(doc, body);

  // Imprime en Serial
  Serial.println("→ Enviando:");
  Serial.println(body);

  // POST al servidor
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(endpoint);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST(body);
    String resp = http.getString();
    Serial.printf("POST %d, respuesta: %s\n", code, resp.c_str());
    http.end();
  } else {
    Serial.println("❌ WiFi desconectado");
  }
}
