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

  // Handle incoming Modbus requests
  mbClient.task();

  // Check if Modscan is requesting data (function code 0x03: read holding registers)
  if (mbClient.readHreg(IP_ModbusSERVER, REG, holdingRegs, holdingRegsSize)) {
    Serial.println("Modscan requested data. Reading...");

    // Update holdingRegs with the data you want to send to Modscan
    holdingRegs[4] = analogRead(A0);   // Example: replace with actual data
    holdingRegs[5] = digitalRead(2);   // Example: replace with actual data
    holdingRegs[6] = 1234;              // Example: replace with actual data
    holdingRegs[7] = 5678;              // Example: replace with actual data

    // Respond to Modscan with the data
    //mbClient.writeHreg(IP_ModbusSERVER, REG, holdingRegs, holdingRegsSize);
    mbClient.readHreg(IP_ModbusSERVER, REG, holdingRegs, holdingRegsSize);

    // Print values for debugging
  Serial.println("Holding Registers:");
  for (int i = 0; i < holdingRegsSize; i++) {
    Serial.print("Reg ");
    Serial.print(i + REG);
    Serial.print(": ");
    Serial.println(holdingRegs[i]);
  }

  }

  // Other code in your loop...
  delay(1000);
}
