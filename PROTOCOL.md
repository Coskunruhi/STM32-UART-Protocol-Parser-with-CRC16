# On-Wire Protocol Specification

## 1. Purpose
This protocol defines how the STM32-based UART message parser (state machine + circular buffer) receives and validates frames using **CRC16/CMS**, and how it responds with a fixed 6-byte reply when a valid message is received.

---

## 2. Serial Settings
- **Baud rate:** 115200  
- **Format:** 8 data bits, no parity, 1 stop bit (8-N-1)  
- **CRC byte order:** Big-endian (CRC_HI first, then CRC_LO)  
- **Flow control:** None  
- **UARTs used in example:** USART2 and/or USART3

---

## 3. Frame Format
Byte 0 : ID (allowed: 0x55, 0x66, 0x77, 0x88, 0x99)
Byte 1 : LEN (total frame length in bytes)
Byte 2..N-3 : DATA (length = LEN - 4)
Byte N-2 : CRC_HI
Byte N-1 : CRC_LO

---

Rules:
- **LEN** = ID + LEN + DATA + CRC_HI + CRC_LO (thus **DATA length = LEN − 4**)
- **CRC16/CMS** is calculated from `ID` through the last **DATA** byte (first `LEN − 2` bytes).
- CRC is sent as **CRC_HI** followed by **CRC_LO**.

---

## 4. CRC16/CMS Parameters
- Polynomial: `0x8005`  
- Init value: `0xFFFF`  
- XorOut: `0x0000`  
- Table-driven implementation, RefIn/RefOut as per code  
- Calculation range: `[ID .. last DATA byte]` (excluding CRC bytes)  
- Wire order: **CRC_HI, CRC_LO**

---

## 5. Valid ID ↔ LEN Mapping
Any frame with an ID–LEN combination outside the table below is rejected.
0x55 → LEN 0xFF
0x66 → LEN 0x7F
0x77 → LEN 0x06
0x88 → LEN 0x07
0x99 → LEN 0x05

---

## 6. Receiver State Machine
1. **WAIT_ID** — Accept only allowed IDs.  
2. **WAIT_LEN** — LEN must match the fixed LEN for the received ID, otherwise reset to WAIT_ID.  
3. **WAIT_DATA** — Collect `LEN − 4` bytes of DATA.  
4. **CRC_HI** — Read the high byte of CRC.  
5. **CRC_CHECK** — Read low byte, calculate CRC, compare:  
   - Match → Build and transmit 6-byte response, reset to WAIT_ID.  
   - No match → Reset to WAIT_ID silently.

---

## 7. Device Response Format
For a valid frame, the device sends: [ ID, OUT1, OUT2, OUT3, CRC_HI, CRC_LO ]
Where OUT values are fixed per ID:
0x66 → 22 05 CB
0x77 → 33 05 F1
0x88 → 44 05 2C
0x99 → 55 05 63
Response CRC is calculated over the first 4 bytes (ID..OUT3) using CRC16/CMS, big-endian on the wire.

---

## 8. Example Transactions
For full request/response examples including long 0x55 and 0x66 frames, see `MessageExampleText` in the repository.

Quick reference:
REQ: 77 06 88 BD 9F CC
RSP: 77 33 05 F1 33 E9

REQ: 88 07 EE 69 01 8C 9B
RSP: 88 44 05 2C 3B A3

REQ: 99 05 04 1B EC
RSP: 99 55 05 63 6F 53

(Last two bytes are CRC_HI, CRC_LO over [ID..OUT3].)

---

## 9. Error Handling
- **Invalid ID:** Ignore and stay in WAIT_ID.  
- **Invalid LEN for ID:** Reset to WAIT_ID.  
- **CRC mismatch:** Reset to WAIT_ID, no response sent.  
- **Byte loss or overflow:** Reset to WAIT_ID.

---

## 10. Testing (HTerm or Serial Monitor)
1. Open serial port at 115200-8N1.  
2. In hex mode, send a sample frame (e.g., `77 06 88 BD 9F CC`).  
3. Verify device sends correct 6-byte response.  
4. Modify one DATA byte and resend → Device should not respond.

---

## 11. Related Files
- `MessageExampleText` — Detailed request/response examples  
- `circular_message.*`, `message_state.*`, `crc16_cms.*`, `main.c` — Source code  
- `23_crc_statemachine.ioc` — STM32CubeMX configuration

---

## 12. License
This protocol and example implementation are released under the MIT License (see `LICENSE` in repository).


