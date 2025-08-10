# STM32 UART Protocol Parser with CRC16/CMS

An interrupt-driven, dual-UART message parser for STM32 (tested on NUCLEO-G431RB) that:
- Uses a state machine to validate incoming frames (ID → LEN → DATA → CRC)
- Buffers data in a lock-free circular buffer for non-blocking reception
- Verifies message integrity using **CRC16/CMS** (poly `0x8005`, init `0xFFFF`, xorOut `0x0000`)
- Sends a fixed 6-byte response on valid CRC

---

## 📹 Demo Video
[![Demo on YouTube](https://img.youtube.com/vi/NewqxMn0cwg/hqdefault.jpg)](https://www.youtube.com/watch?v=NewqxMn0cwg)

---

## ✨ Features
- **Non-blocking UART RX** using HAL `HAL_UART_Receive_IT` and per-UART circular buffers
- **Deterministic parsing** with strict ID ↔ LEN mapping
- **Table-driven CRC16/CMS** calculation for efficiency
- Works with two UART interfaces independently

---

## 📦 Repository Structure
.
├─ circular_message.c / .h      # Circular buffer implementation  
├─ message_state.c / .h         # State machine parser & response builder  
├─ crc16_cms.c / .h             # CRC16/CMS calculation  
├─ main.c                       # UART init, interrupt handling, main loop  
├─ STM32-UART-Protocol-Parser-with-CRC16.ioc   # STM32CubeMX configuration  
├─ PROTOCOL.md                  # On-wire protocol specification  
├─ Message Examples.txt         # Example request/response frames  
├─ LICENSE  
└─ .gitignore  

---

## 🔌 Protocol Overview
**Frame format (big-endian CRC):**
[ ID(1) | LEN(1) | DATA(LEN-4) | CRC_HI(1) | CRC_LO(1) ]

markdown
Kopyala
Düzenle
- Allowed IDs: `0x55, 0x66, 0x77, 0x88, 0x99`
- LEN includes all fields; DATA length = LEN − 4
- CRC16/CMS is computed over `ID..last DATA byte`
- ID ↔ LEN mapping:
0x55 → 0xFF
0x66 → 0x7F
0x77 → 0x06
0x88 → 0x07
0x99 → 0x05

yaml
Kopyala
Düzenle

See [PROTOCOL.md](PROTOCOL.md) for full details.

---

## 🧪 Example
From `Message Examples.txt`:

Request:
77 06 88 BD 9F CC

makefile
Kopyala
Düzenle
Response:
33 05 F1 33 E9

yaml
Kopyala
Düzenle
(Last two bytes are CRC16/CMS over [ID..OUT3])

---

## 🛠️ Build & Run (STM32CubeIDE)
1. Open `STM32-UART-Protocol-Parser-with-CRC16.ioc` in STM32CubeIDE
2. Generate code and ensure `Src/` and `Inc/` contain the provided `.c` and `.h` files
3. Connect NUCLEO-G431RB via USB
4. Flash the program to the board
5. Open HTerm / Serial Monitor @ **115200-8N1** and send example frames in HEX mode

---

## 📜 License
This project is released under the MIT License. See [LICENSE](LICENSE).

---

## 🙌 Credits
Developed by **Erkin Coşkun Ruhi**
