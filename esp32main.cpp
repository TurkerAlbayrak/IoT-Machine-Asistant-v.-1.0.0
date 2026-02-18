#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";

const char* request_topic = "factory/request";
const char* data_topic = "factory/data";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastRequest = 0;
const long interval = 1000;

void callback(char* topic, byte* payload, unsigned int length) {

  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (!error) {
    Serial.println("=== SENKRON VERI GELDI ===");

    Serial.print("Temp: ");
    Serial.println((float)doc["temperature"]);

    Serial.print("Humidity: ");
    Serial.println((float)doc["humidity"]);

    Serial.print("Gas: ");
    Serial.println((int)doc["gas"]);

    Serial.print("Vib X: ");
    Serial.println((float)doc["vibration"]["x"]);

    Serial.println("=========================");
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32ClientSync")) {
      client.subscribe(data_topic);
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  unsigned long now = millis();

  if (now - lastRequest > interval) {
    lastRequest = now;
    client.publish(request_topic, "send");
  }
}
