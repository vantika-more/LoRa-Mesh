#include <SPI.h>
#include <LoRa.h>

// LoRa pins (same as your working test)
#define LORA_SS    10
#define LORA_RST   9
#define LORA_DIO0  2

// Frequency (use same on receiver)
#define LORA_FREQ  915E6   // use 868E6 only if your module is 868MHz

// Analog input from TMP36 (CN7 PC2)
#define TEMP_PIN   PC2

uint32_t pktId = 0;

float readTMP36_C() {
  int raw = analogRead(TEMP_PIN);     // STM32 usually 12-bit ADC: 0..4095
  float v = (raw * 3.3f) / 4095.0f;   // Convert ADC to voltage

  // TMP36 formula
  float tempC = (v - 0.5f) * 100.0f;
  return tempC;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("LoRa Sender: TMP36 on PC2");

  pinMode(TEMP_PIN, INPUT);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  LoRa.setTxPower(17);
  Serial.println("LoRa init OK. Sending temperature...");
}

void loop() {
  float tC = readTMP36_C();

  String msg = "ID=" + String(pktId++) + ",T=" + String(tC, 2);

  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();

  Serial.print("Sent: ");
  Serial.println(msg);

  delay(2000);
}

