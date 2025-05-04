/*
 * uart.c
 *
 *  Created on: Nov 19, 2024
 *      Author: dbfir
 */

#include "uart.h"
#include <stdio.h>

UART_HandleTypeDef *myHuart1;   //uart의 핸들
UART_HandleTypeDef *myHuart2;   //uart의 핸들

//Ring buffer cicular buffer

uint8_t rxChar1; 							 //수신문자
uint8_t rxChar2;
#define rxBufferMax 255				 //수신버퍼의 최대크기
int rxBufferWrite1,rxBufferWrite2; 					   //수신버퍼 쓰기 포인터
int rxBufferRead1,rxBufferRead2;						   //수신버퍼 읽기 포인터
uint8_t rxBuffer1[rxBufferMax],rxBuffer2[rxBufferMax]; //수신버퍼

int _write(int file, char* p, int len){
	HAL_UART_Transmit(myHuart2, (uint8_t *)p, len, 10);
	return len;
}

// uart장치의 초기화 함수 구현
void initUart1(UART_HandleTypeDef *inHuart) {
	myHuart1 = inHuart;
	//수신인터럽트 설정
	HAL_UART_Receive_IT(myHuart1, &rxChar1, 1);
	rxBufferRead1 = rxBufferWrite1 = 0;
}

void initUart2(UART_HandleTypeDef *inHuart) {
	myHuart2 = inHuart;
	//수신인터럽트 설정
	HAL_UART_Receive_IT(myHuart2, &rxChar2, 1);
	rxBufferRead2 = rxBufferWrite2 = 0;
}

void clearRxWifiBuffer1() {
	rxBufferRead2 = rxBufferWrite2 = 0;
   memset(rxBuffer1, 0, rxBufferMax);
}

//문자수신 처리 함수 구현
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == myHuart1->Instance){
		// 수신딘 문자를 버퍼에 저장하고 쓰기포인터의 값을 1증가
		rxBuffer1[rxBufferWrite1++] = rxChar1;
		// 쓰기포인터의 값이 최대치에 도달하면 다시 0으로 초기화
		rxBufferWrite1 %= rxBufferMax;
		// 다음 문자 수신 인터럽트를 위하여 재 설정
		HAL_UART_Receive_IT(myHuart1, &rxChar1, 1);
	}
	if (huart->Instance == myHuart2->Instance) {
		// 수신딘 문자를 버퍼에 저장하고 쓰기포인터의 값을 1증가
		rxBuffer2[rxBufferWrite2++] = rxChar2;
		// 쓰기포인터의 값이 최대치에 도달하면 다시 0으로 초기화
		rxBufferWrite2 %= rxBufferMax;
		// 다음 문자 수신 인터럽트를 위하여 재 설정
		HAL_UART_Receive_IT(myHuart2, &rxChar2, 1);
	}

}

// 버퍼에서 문자 꺼내오기
uint8_t getUart1(){
	uint8_t result;
	// 수신된 문자 없음
	if(rxBufferWrite1 == rxBufferRead1) return 0;
	//읽기 포인터가 가르키는 위치의 버퍼문자를 꺼내고, 읽기포인터를 1 증가
	result = rxBuffer1[rxBufferRead1++];
	//읽기포인터가 최대치에 도달하면 다시 0으로 초기화
	rxBufferRead1 %= rxBufferMax;
	return result;
}

uint8_t getUart2(){
	uint8_t result;
	// 수신된 문자 없음
	if(rxBufferWrite2 == rxBufferRead2) return 0;
	//읽기 포인터가 가르키는 위치의 버퍼문자를 꺼내고, 읽기포인터를 1 증가
	result = rxBuffer2[rxBufferRead2++];
	//읽기포인터가 최대치에 도달하면 다시 0으로 초기화
	rxBufferRead2 %= rxBufferMax;
	return result;
}
