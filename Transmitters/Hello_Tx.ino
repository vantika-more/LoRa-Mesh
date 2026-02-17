#include <SPI.h>
#include <LoRa.h>

// ---- CHANGE THESE IF YOUR SHIELD USES DIFFERENT PINS ----
#define LORA_SS    10
#define LORA_RST   9
#define LORA_DIO0  2

// Frequency (keep SAME on receiver)
#define LORA_FREQ  915E6   // Australia often uses 915 MHz

uint32_t counter = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("LoRa Sender starting...");

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("❌ LoRa begin failed (check wiring/pins/frequency)");
    while (1);
  }

  LoRa.setTxPower(17);     // 2 to 20 (depends on module)
  // LoRa.setSpreadingFactor(7); // optional
  // LoRa.setSignalBandwidth(125E3); // optional
  // LoRa.setCodingRate4(5); // optional

  Serial.println("✅ LoRa Sender ready");
}

void loop() {
  String payload = "HELLO " + String(counter++);

  LoRa.beginPacket();
  LoRa.print(payload);
  int ok = LoRa.endPacket();   // returns 1 if success in many cores

  Serial.print("Sent: ");
  Serial.print(payload);
  Serial.print(" | endPacket(): ");
  Serial.println(ok);

  delay(1000);
}
