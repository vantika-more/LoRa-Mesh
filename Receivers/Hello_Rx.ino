#include <SPI.h>
#include <LoRa.h>

#define LORA_SS   10
#define LORA_RST  9
#define LORA_DIO0 2

void setup() {
  Serial.begin(115200);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa failed");
    while (1);
  }
  Serial.println("Receiver ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.println();
  }
}
