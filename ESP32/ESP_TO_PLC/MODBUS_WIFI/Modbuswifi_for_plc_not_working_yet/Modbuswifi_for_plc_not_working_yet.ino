#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

const int REG = 40000;  // Modbus Hreg offset
IPAddress IP_ModbusSERVER(192, 168, 0, 104);  // Address of Modbus Slave device
const int number_REG = 0;
ModbusIP mbClient;

void setup() {
  Serial.begin(115200);
  WiFi.begin("Sai_links2.4-G", "ppatil@18092001");

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  mbClient.client();
}

uint16_t res[5];

void loop() {
  if (!mbClient.isConnected(IP_ModbusSERVER)) {
    Serial.println("Not connected to Modbus server. Connecting...");
    mbClient.connect(IP_ModbusSERVER);
    delay(5000);  // Wait for 5 seconds before the next connection attempt
    Serial.println("Connection attempt completed");
  }

  mbClient.task();
  delay(100);

  Serial.print("Data read from Modbus registers: ");
  for (int j = 0; j < number_REG; j++) {
    Serial.print(res[j]);
    Serial.print(" ");
  }
  Serial.println("");
}
