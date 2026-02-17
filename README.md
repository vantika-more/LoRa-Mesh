# LoRa-Mesh

LoRa Network for air quality monitoring. 

This repository contains Arduino IDE sketches for building a small LoRa-based multi-node network (2–3 nodes) using **STM32 Nucleo boards + SX1276MB1MAS LoRa shields**, intended for resilient sensor data transmission (e.g., temperature / air-quality style telemetry).

---

## Project Overview

In remote monitoring deployments, a single cellular (4G) link can become a single point of failure. This project demonstrates a **backup LoRa network** where a sensor node can transmit data to a nearby receiver node, and the design can be extended to support **multi-hop relay** and **failover** (backup receiver path when the primary receiver is unavailable).

---

## Repository Structure

- `Transmitters/` — sender-side sketches (sensor read + LoRa transmit)  
- `Receivers/` — receiver-side sketches (LoRa receive + print RSSI/SNR, optional ACK/failover) =

> Tip: Keep one sketch per node (Node1 / Node2 / Node3) so you can upload the correct role to each board easily.

---

## Hardware Used

- **STM32 Nucleo boards** (e.g., NUCLEO-L152RE / NUCLEO-F401RE)
- **SX1276MB1MAS LoRa shield** (one per node)
- Sensor example:
  - **TMP36** analog temperature sensor (can be replaced with other analog/digital sensors)

---

## Software Requirements

- Arduino IDE (2.x recommended)
- **Arduino LoRa library** by Sandeep Mistry  
  - Arduino IDE → **Tools → Manage Libraries…** → search: `LoRa` → install **“LoRa” (Sandeep Mistry)**

---

## Wiring Notes (Typical / Example)

### LoRa Shield (SPI + control)
Your LoRa shield must connect correctly to the SPI pins plus:
- `NSS/SS` (chip select)
- `RESET`
- `DIO0` (interrupt)

> Pin names vary by board/core. Use the same pin mapping on all nodes and keep it consistent in code.

### TMP36 (example sensor)
TMP36 (flat face towards you):
- Left: **VCC → 3.3V**
- Middle: **VOUT → analog pin** (example: `PC2`)
- Right: **GND → GND**

---

## How to Run (2 Nodes: Sender → Receiver)

1. Upload a transmitter sketch from `Transmitters/` to **Node 1**
2. Upload a receiver sketch from `Receivers/` to **Node 2**
3. Ensure both nodes use the **same**:
   - Frequency (e.g., `915E6` or `868E6`)
   - Sync word (if used)
   - Pin mapping (SS/RST/DIO0)
4. Open **Serial Monitor** for both boards (115200 baud recommended)
5. Confirm:
   - Sender prints “Sent …”
   - Receiver prints “Received …” + RSSI/SNR (if implemented)

---

## Failover Concept (3 Nodes)

### Goal
- **Normal case:** Node1 → Node2  
- **Failover case (Node2 down):** Node1 → Node3

### Recommended Approach (ACK-based)
To detect a failed primary receiver, implement an **ACK** mechanism:

- Node1 sends DATA to Node2 and waits for `ACK`
- If ACK not received within a timeout, Node1 sends the **same packet** to Node3

This keeps the system resilient and reduces data loss during outages.

> Improvement: Use `packetId` + duplicate filtering at Node3 to avoid logging the same message twice if Node2 recovers mid-stream.

---

## Message Format (Suggested)

A simple, readable payload format:

- `SRC=<id>,DST=<id>,TYPE=DATA,PID=<packetId>,T=<tempC>`

Example:
- `SRC=1,DST=2,TYPE=DATA,PID=40,T=25.30`

---

## Troubleshooting

### Receiver shows nothing
- Confirm both nodes are on the **same frequency** (`915E6` vs `868E6`)
- Check antenna connection
- Verify LoRa pin mapping (SS/RST/DIO0)

### Temperature values look wrong (negative / unrealistic)
- TMP36 wiring is likely wrong (VOUT/GND swapped)
- Confirm sensor powered from **3.3V**
- Print ADC RAW + voltage to debug

### Receiver keeps restarting / prints “starting…” repeatedly
- Remove `while(!Serial)` on Nucleo boards (it may block/reset depending on core/USB)
- Use a short `delay(500)` in `setup()` instead

---

## Future Work

- Multi-hop relay (Node1 → Node2 → Node3)
- Channel access / collision reduction (random backoff, time slots)
- Store-forward buffering when no receiver is available
- Encryption/authentication for payload integrity

---
