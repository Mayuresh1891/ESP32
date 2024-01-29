#include <SimpleDHT.h>
#include <UIPEthernet.h>  // Include the Ethernet library for ENC28J60
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Mayur_patil"
#define AIO_KEY         "aio_DPJs77CRCueQSlmqU3R8CPwPBily"

EthernetClient client;  // Use EthernetClient for ENC28J60

#define MyMessage "Hello World, I am ESP32"

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature1");
Adafruit_MQTT_Publish Humidity1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity1");
Adafruit_MQTT_Publish MyMessagePublish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/message");

Adafruit_MQTT_Subscribe ReceivedMessageSubscription = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/recmsg");

int pinDHT11 = 0;
SimpleDHT11 dht11(pinDHT11);
byte hum = 0;
byte temp = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {
  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));

  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Initializing Ethernet with ENC28J60..."));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    // You can set a static IP address here if needed.
    // Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println(F("Ethernet connected"));
  Serial.println(F("IP address: "));
  Serial.println(Ethernet.localIP());

  connect();

  Serial.print(F("Subscribing to feed: "));
  Serial.println(mqtt.subscribe(&ReceivedMessageSubscription));
}

void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavailable")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void loop() {
  if (!mqtt.ping(3)) {
    if (!mqtt.connected())
      connect();
  }

  dht11.read(&temp, &hum, NULL);
  Serial.print((int)temp); Serial.print(" *C, ");
  Serial.print((int)hum); Serial.println(" H");
  delay(5000);

  if (!Temperature1.publish(temp)) {
    Serial.println(F("Failed to publish Temperature1"));
  }
  if (!Humidity1.publish(hum)) {
    Serial.println(F("Failed to publish Humidity1"));
  } else {
    Serial.println(F("Temperature1 and Humidity1 Sent!"));
  }

  if (!MyMessagePublish.publish(MyMessage)) {
    Serial.println(F("Failed to publish MyMessage"));
  } else {
    Serial.println(F("MyMessage Sent!"));
  }

  // Check for incoming messages on the subscribed feed
  Serial.println(F("Checking MQTT subscription..."));
  if (mqtt.readSubscription(5000)) {
    Serial.println(F("Received message!"));
    if (ReceivedMessageSubscription.lastread) {
      Serial.print(F("Received message: "));
      Serial.println((char *)ReceivedMessageSubscription.lastread);
    }
  }

  // Delay to prevent excessive checking
  delay(1000);
}
