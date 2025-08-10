/*
 * circular_message.h
 *
 *  Created on: Jul 10, 2025
 *      Author: Erkin Coskun Ruhi
 */

#ifndef INC_CIRCULAR_MESSAGE_H_
#define INC_CIRCULAR_MESSAGE_H_

#include <stdint.h>

#define CIRC_BUF_SIZE 255

typedef struct{
	uint8_t buffer[CIRC_BUF_SIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
}CircularBuffer;

void CircularBuffer_Init(CircularBuffer *cb);
void CircularBuffer_Write(CircularBuffer *cb, uint8_t data);
int CircularBuffer_IsEmpty(CircularBuffer *cb);
int CircularBuffer_Read(CircularBuffer *cb, uint8_t *data);

void CircularMessage_Init(CircularBuffer *cb);
void CircularMessage_Process(CircularBuffer *cb);

#endif /* INC_CIRCULAR_MESSAGE_H_ */
