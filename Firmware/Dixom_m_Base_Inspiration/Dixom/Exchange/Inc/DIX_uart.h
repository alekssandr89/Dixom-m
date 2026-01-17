/*
 * DIX_uart.h
 *
 *  Created on: 12 ���. 2022 �.
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_DIX_UART_H_
#define EXCHANGE_INC_DIX_UART_H_

#define MAX_TX_DATA_LENGTH_UART1  255
#define MAX_RX_DATA_LENGTH_UART1  255

#define RX_BUFFER_LENGTH_UART1  1024

void UART_Initialization();
void UART1_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from);
void UART2_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from);
void UART3_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from);
void UART5_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from);

void UART1_Received_DMA();

void RxIdleCallbackUart1();
void TxCpltCallbackUart1();
void RxDmaCallbackUart1();


#endif /* EXCHANGE_INC_DIX_UART_H_ */
