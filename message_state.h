/*
 * message_state.h
 *
 *  Created on: Jul 10, 2025
 *      Author: Erkin Coskun Ruhi
 */

#ifndef INC_MESSAGE_STATE_H_
#define INC_MESSAGE_STATE_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32g4xx_hal.h"

#define MAX_LEN 0xFF      	//max mesaj uzunluğu
#define MSG_OUT_LEN 0x06  	// cikti mesaj uzunluğu

// Mesaj ID'ler
#define MSG_ID_1 0x55
#define MSG_ID_2 0x66
#define MSG_ID_3 0x77
#define MSG_ID_4 0x88
#define MSG_ID_5 0x99

// Mesaj Uzunluklar
#define MSG_LEN_1 0xFF 	// 0x55 mesaj uzunluk
#define MSG_LEN_2 0x7F 	// 0x66 mesaj uzunluk
#define MSG_LEN_3 0x06 	// 0x77 mesaj uzunluk
#define MSG_LEN_4 0x07	// 0x88 mesaj uzunluk
#define MSG_LEN_5 0x05 	// 0x99 mesaj uzunluk

// 0x55 ID'li mesaj cıktıları
#define MSG_OUT_1_1 0x11
#define MSG_OUT_1_2 0x05
#define MSG_OUT_1_3 0x47

// 0x66 ID'li mesaj cıktıları
#define MSG_OUT_2_1 0x22
#define MSG_OUT_2_2 0x05
#define MSG_OUT_2_3 0xCB

// 0x77 ID'li mesaj cıktıları
#define MSG_OUT_3_1 0x33
#define MSG_OUT_3_2 0x05
#define MSG_OUT_3_3 0xF1

// 0x88 ID'li mesaj cıktıları
#define MSG_OUT_4_1 0x44
#define MSG_OUT_4_2 0x05
#define MSG_OUT_4_3 0x2C

// 0x99 ID'li mesaj cıktıları
#define MSG_OUT_5_1 0x55
#define MSG_OUT_5_2 0x05
#define MSG_OUT_5_3 0x63

// Mesaj asamaları
typedef enum{
	STATE_WAIT_ID,
	STATE_WAIT_LEN,
	STATE_WAIT_DATA,
	STATE_CRC_VAL_1,
	STATE_MESSAGE_CRC_CONTROL
}MsgState;

typedef struct {
	MsgState current_state;
	uint8_t msg_input[MAX_LEN];
	uint8_t msg_output[MSG_OUT_LEN];
	size_t index;
} MessageContext;


void MessageState_Init(MessageContext *ctx);
void MessageState_Process(MessageContext *ctx, uint8_t byte, UART_HandleTypeDef *huart);


#endif /* INC_MESSAGE_STATE_H_ */
