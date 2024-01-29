#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Sai_links2.4-G";
const char *password = "ppatil@18092001";
const char *mqttServer = "192.168.0.104";
const int mqttPort = 1883;
const char *clientId = "Tesla_Demo1";
const long interval = 3000;

#define VOLTAGE "230V"
#define CURRENT "5A"
#define TEMP "35c"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  delay(10);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure MQTT client
  mqttClient.setServer(mqttServer, mqttPort);
  Serial.println("MQTT client configured");

  // Initialize millis
  previousMillis = millis();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Reconnected to WiFi");
  }

  mqttClient.loop();

  if (millis() - previousMillis > interval) {
    sendData();
    previousMillis = millis();
  }
}

void sendData() {
  if (mqttClient.connect(clientId)) {
    mqttClient.publish("VOLTAGE", VOLTAGE);
    delay(1000);
    mqttClient.publish("CURRENT", CURRENT);
    delay(1000);
    mqttClient.publish("TEMP", TEMP);
  }
}
