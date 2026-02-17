#include <SPI.h>
#include <LoRa.h>

#define LORA_SS    10
#define LORA_RST   9
#define LORA_DIO0  2

#define LORA_FREQ  915E6   // MUST match sender

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("LoRa2 RECEIVER: starting...");

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println(" LoRa init failed!");
    while (1);
  }

  LoRa.receive();  // important
  Serial.println("LoRa2 RECEIVER: listening...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return;

  String msg = "";
  while (LoRa.available()) msg += (char)LoRa.read();

  Serial.print("Received: ");
  Serial.print(msg);
  Serial.print(" | RSSI: ");
  Serial.println(LoRa.packetRssi());
}
