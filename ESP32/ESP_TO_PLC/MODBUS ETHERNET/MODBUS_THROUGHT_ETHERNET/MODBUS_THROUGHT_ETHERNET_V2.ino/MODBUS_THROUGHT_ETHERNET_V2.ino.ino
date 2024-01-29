#include <SPI.h>
#include <EthernetENC.h>
#include <ModbusIP_ESP8266.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 0, 106);
IPAddress myDns(192, 168, 0, 102);

EthernetClient client;


// Modbus registers
const int REG = 40000;  // Modbus Hreg offset
IPAddress IP_ModbusSERVER(192, 168, 0, 106);  // Address of Modbus Slave device
//const int number_REG = 0;


int holdingRegsSize = 10;
uint16_t holdingRegs[10];

//uint16_t res[5];

// ModbusIP object
//ModbusIP mb;
ModbusIP mbClient;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial port to connect (needed for native USB port only)
  }

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // Try to configure using IP address instead of DHCP:
    Ethernet.begin(mac, IP_ModbusSERVER, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  // Start Modbus server/client
  //mb.server();
  mbClient.client();


}

void loop() {
  auto link = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (link) {
    case Unknown:
      Serial.println("Unknown");
      break;
    case LinkON:
      Serial.println("ON");
      break;
    case LinkOFF:
      Serial.println("OFF");
      break;
  }

  if (!mbClient.isConnected(IP_ModbusSERVER)) {
    Serial.println("Not connected to Modbus server. Connecting...");
    mbClient.connect(IP_ModbusSERVER);
    delay(5000);  // Wait for 5 seconds before the next connection attempt
    Serial.println("Connection attempt completed");
  }


  // Handle Modbus requests
  //mb.task();
  mbClient.task();
  delay(100);
  
  // Example: Update holding registers with some values
  holdingRegs[0] = analogRead(A0);
  holdingRegs[1] = digitalRead(2);
  holdingRegs[2] = 1234;
  holdingRegs[3] = 5678;
  

  // Print values for debugging
  Serial.println(holdingRegs[0]);
  Serial.println(holdingRegs[1]);
  Serial.println(holdingRegs[2]);
  Serial.println(holdingRegs[3]);


  // Add a delay if needed to avoid flooding the Modbus network
  delay(1000);
}
