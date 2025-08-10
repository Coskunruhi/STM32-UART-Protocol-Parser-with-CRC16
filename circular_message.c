/*
 * circular_message.c
 *
 *  Created on: Jul 10, 2025
 *      Author: Erkin Coskun Ruhi
 */


#include "circular_message.h"

// Circular Buffer'a başlangıç değerleri atama
void CircularBuffer_Init(CircularBuffer *cb){
	cb -> head = 0;
	cb -> tail = 0;
}

// Circular buffer içine veri yazmak için
void CircularBuffer_Write(CircularBuffer *cb, uint8_t data){
    uint8_t next = (cb -> head + 1) % CIRC_BUF_SIZE;

    if (next != cb -> tail) {
        cb -> buffer[cb -> head] = data;
        cb -> head = next;
    }
}

// Circular Buffer dolu mu boş mu ona bakar
int CircularBuffer_IsEmpty(CircularBuffer *cb){
	return cb -> head == cb -> tail;
}

// Circular Buffer'a girilen değerleri okur
int CircularBuffer_Read(CircularBuffer *cb, uint8_t *data){

	if (CircularBuffer_IsEmpty(cb)) {
		return 0;
	}
	*data = cb -> buffer[cb -> tail];
	cb -> tail = (cb -> tail + 1) % CIRC_BUF_SIZE;
	return 1;
}

