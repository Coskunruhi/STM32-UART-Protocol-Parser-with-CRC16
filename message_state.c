/*
 * message_state.c
 *
 *  Created on: Jul 10, 2025
 *      Author: Stajyer4
 */

#include "message_state.h"
#include "crc16_cms.h"
#include "main.h"

void MessageState_Init(MessageContext *ctx){
	ctx -> current_state = STATE_WAIT_ID;
	ctx -> index = 0;
	for (size_t i = 0; i < MAX_LEN; i++) ctx -> msg_input[i] = 0;
}

// Mesaj id geçerli mi kontrolü yapılır
static uint8_t is_valid_id(uint8_t id){
	return (id == MSG_ID_1 || id == MSG_ID_2 || id == MSG_ID_3 || id == MSG_ID_4 || id == MSG_ID_5);
}

// Mesaj id ve uzunluğu uyumlu mu kontrolü yapılır
static uint8_t is_valid_len(uint8_t id, uint8_t len){
	switch(id){
	case MSG_ID_1: return len == MSG_LEN_1;
	case MSG_ID_2: return len == MSG_LEN_2;
	case MSG_ID_3: return len == MSG_LEN_3;
	case MSG_ID_4: return len == MSG_LEN_4;
	case MSG_ID_5: return len == MSG_LEN_5;
	default: return 0;
	}
}

// Mesajı ekrana yazdırır
static void print_response(MessageContext *ctx, uint8_t id, UART_HandleTypeDef *huart){
	ctx -> msg_output[0] = id;
	switch(ctx -> msg_input[0]){
	case MSG_ID_1:
		ctx -> msg_output[1] = MSG_OUT_1_1;
		ctx -> msg_output[2] = MSG_OUT_1_2;
		ctx -> msg_output[3] = MSG_OUT_1_3;
		break;

	case MSG_ID_2:
		ctx -> msg_output[1] = MSG_OUT_2_1;
		ctx -> msg_output[2] = MSG_OUT_2_2;
		ctx -> msg_output[3] = MSG_OUT_2_3;
		break;

	case MSG_ID_3:
		ctx -> msg_output[1] = MSG_OUT_3_1;
		ctx -> msg_output[2] = MSG_OUT_3_2;
		ctx -> msg_output[3] = MSG_OUT_3_3;
		break;

	case MSG_ID_4:
		ctx -> msg_output[1] = MSG_OUT_4_1;
		ctx -> msg_output[2] = MSG_OUT_4_2;
		ctx -> msg_output[3] = MSG_OUT_4_3;
		break;

	case MSG_ID_5:
		ctx -> msg_output[1] = MSG_OUT_5_1;
		ctx -> msg_output[2] = MSG_OUT_5_2;
		ctx -> msg_output[3] = MSG_OUT_5_3;
		break;
	}
	uint16_t crc_out = crc16_cms_calc(ctx -> msg_output, 4);
	ctx -> msg_output[4] = crc_out >> 8;
	ctx -> msg_output[5] = crc_out;
	HAL_UART_Transmit(huart, ctx -> msg_output, MSG_OUT_LEN, HAL_MAX_DELAY);
}

// Mesaj kontrol vazdırma
void MessageState_Process(MessageContext *ctx, uint8_t byte, UART_HandleTypeDef *huart){
	switch(ctx -> current_state){
	case STATE_WAIT_ID:
		if(is_valid_id(byte)){
			ctx -> msg_input[0] = byte;
			ctx -> index = 1;
			ctx -> current_state = STATE_WAIT_LEN;
		}
		break;
	case STATE_WAIT_LEN:
		if (is_valid_len(ctx -> msg_input[0], byte)) {
			ctx -> msg_input[1] = byte;
			ctx -> index = 2;
			ctx -> current_state = STATE_WAIT_DATA;
		}else {
			ctx -> current_state = STATE_WAIT_ID;
			ctx -> index = 0;
		}
		break;
	case STATE_WAIT_DATA:
		ctx -> msg_input[ctx -> index++] = byte;
		if (ctx -> index == ctx -> msg_input[1] - 2) {
			ctx -> current_state = STATE_CRC_VAL_1;

		}
		break;
	case STATE_CRC_VAL_1:
		ctx -> msg_input[ctx -> index++] = byte;
		if (ctx -> index == (ctx -> msg_input[1] - 1)) {
			ctx -> current_state =  STATE_MESSAGE_CRC_CONTROL;
		}
		break;
	case STATE_MESSAGE_CRC_CONTROL:
		ctx -> msg_input[ctx -> index] = byte;
		uint16_t crc_calculated = crc16_cms_calc(ctx -> msg_input, ctx -> msg_input[1] - 2);
		uint16_t crcMessage_value = ((uint16_t)ctx -> msg_input[ctx -> index - 1] << 8) + ctx -> msg_input[ctx -> index];
		if (crc_calculated == crcMessage_value) {
			print_response(ctx, ctx -> msg_input[0], huart);
			ctx -> current_state = STATE_WAIT_ID;
			ctx -> index = 0;
		}else {
			ctx -> index = 0;
			ctx -> current_state = STATE_WAIT_ID;
		}
		break;

	}
}


