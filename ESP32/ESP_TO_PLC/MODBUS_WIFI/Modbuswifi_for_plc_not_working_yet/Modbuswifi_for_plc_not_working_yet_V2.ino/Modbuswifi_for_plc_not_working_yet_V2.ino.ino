#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

// Modbus registers
const int REG = 40000;  // Modbus Hreg offset
IPAddress IP_ModbusSERVER(192, 168, 0, 104);  // Address of Modbus Slave device

int holdingRegsSize = 10;
uint16_t holdingRegs[10];

ModbusIP mbClient;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // Wait for the serial port to connect (needed for native USB port only)
  }

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

  delay(5000); // Wait for a moment after connecting to WiFi

  // Start Modbus client
  Serial.println("Initializing Modbus client");
  mbClient.client();
  delay(1000);
}

void loop() {
  if (!mbClient.isConnected(IP_ModbusSERVER)) {
    Serial.println("Not connected to Modbus server. Connecting...");
    delay(5000);
    mbClient.connect(IP_ModbusSERVER);
    delay(5000);  // Wait for 5 seconds before the next connection attempt
    Serial.println("Connection attempt completed");
  }

  // Update Modbus registers with some values
  holdingRegs[0] = analogRead(A0);
  holdingRegs[1] = digitalRead(2);
  holdingRegs[2] = 1234;
  holdingRegs[3] = 5678;

  // Print values for debugging
  Serial.println("Holding Registers:");
  for (int i = 0; i < holdingRegsSize; i++) {
    Serial.print("Reg ");
    Serial.print(i + REG);
    Serial.print(": ");
    Serial.println(holdingRegs[i]);
  }

  // Update Modbus registers
  mbClient.writeHreg(IP_ModbusSERVER, REG, holdingRegs, holdingRegsSize);

  // Delay to avoid flooding the Modbus network
  delay(1000);
}
