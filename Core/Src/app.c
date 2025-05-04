/*
 * app.c
 *
 *  Created on: Mar 11, 2025
 *      Author: wjdek
 */
#include "app.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

uint32_t timeoutWifiRx;
uint32_t timeoutReceiveWifi;

typedef struct{
	char * ssid;
	char * password;
}wSystem_t;

wSystem_t wSystem;

char* receiveWifi() {
    /*static char lineBuffer[2048];
    signed char ch = getWifiChar();
    int pos = 0;
    if(ch != -1) {
        timeoutReceiveWifi = 100;
        while(ch != '\r') {
            if(ch == '\n') pos = 0;
            else if(ch != -1) {
                lineBuffer[pos++] = ch;
                timeoutReceiveWifi = 100;
            }
            if(timeoutReceiveWifi == 0) {
                clearRxWifiBuffer();
                return NULL;
            }
            ch = getWifiChar();
        }
        lineBuffer[pos++] = 0;
    }
    return lineBuffer;*/
    static char lineBuffer[2048];
    int pos = 0;
    signed char ch = getUart1();

    if (ch == -1) { // 데이터가 없으면 NULL 반환
        return NULL;
    }
    timeoutReceiveWifi = 100; // 타임아웃 초기화
    while (ch != '\r') { // '\r'까지 읽기
        if (ch == '\n') {
            pos = 0; // 새로운 줄의 시작
        } else if (ch != -1) { // 유효한 문자만 버퍼에 저장
            if (pos >= sizeof(lineBuffer) - 1) { // 버퍼 오버플로우 방지
                clearRxWifiBuffer1(); // Rx 버퍼 정리
                return NULL; // 에러 반환
            }
            lineBuffer[pos++] = ch;
        }
        if (timeoutReceiveWifi == 0) { // 타임아웃 발생 시
            clearRxWifiBuffer1(); // Rx 버퍼 정리
            return NULL; // 타임아웃 반환
        }
        ch = getUart1(); // 다음 문자 읽기
    }
    lineBuffer[pos] = '\0'; // 문자열 종료
    return lineBuffer; // 읽은 데이터 반환
}

bool waitForResponse(char *cmpStr) {
    char *rxData;
    timeoutWifiRx = 3000;
    rxData = receiveWifi();
    while (strncmp(rxData, cmpStr, strlen(cmpStr)) != 0) {
        rxData = receiveWifi();
        HAL_Delay(1);
        if (timeoutWifiRx == 0) {
            return false; // Timeout occurred
        }
    }
    return true;
}

void sendWifi(char *str) {
	HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 20);
}


void sendWifiCommand(char *txData) {
    sendWifi(txData);
}
char* receiveWifiPacket() {
    char *rxData;
    char *result;
    rxData = receiveWifi();
    if(strncmp(rxData, "+IPD", 4) == 0) {
        result = strchr(rxData, ':');   // ;의 위치를 찾는다
        result++;
    }
    else {
        return 0;
    }
    return result;
}

void processAPmode() {
    static bool isFirst = true;
    char str[255];
    const int delayTime = 5;
    if(isFirst) {
        isFirst = false;
        int status;
        printf("\nAP mode\n");
        sendWifiCommand("AT+RST\r\n");
        osDelay(delayTime);
        status = waitForResponse("ready");
        if(status == 1) printf("\nR:ready\n");
        sendWifiCommand("ATE0\r\n");
        status = waitForResponse("ATE0");
        osDelay(delayTime);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=ATE0\n");
        sendWifiCommand("AT\r\n");
        osDelay(delayTime);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=AT\n");
        clearRxWifiBuffer1();
        sendWifiCommand("AT+CWINIT=1\r\n");
        osDelay(delayTime);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=CWINIT\n");
        if(status == 0) printf("\nError cwinit\n");
        clearRxWifiBuffer1();
        sendWifiCommand("AT\r\n");
        osDelay(delayTime);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=AT\n");
        clearRxWifiBuffer1();
        sendWifiCommand("AT+CWMODE=2\r\n");
        osDelay(delayTime);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=CWMODE\n");
        if(status == 0) printf("\nError cwmode\n");
        clearRxWifiBuffer1();
        sprintf(str, "AT+CWSAP=\"%s\",\"%s\",11,3\r\n", CONFIG_AP_SSID, CONFIG_AP_PASSWORD); // 3=WPA2_PSK
        sendWifiCommand(str);
        osDelay(delayTime);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=CWSAP\n");
        if(status == 0) printf("\nError cwsap\n");
        clearRxWifiBuffer1();
        sendWifiCommand("AT+CIPMUX=1\r\n"); //다중연결
        osDelay(delayTime);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=CIPMUX\n");
        if(status == 0) printf("\nError cipmux\n");
        clearRxWifiBuffer1();
        osDelay(50);    // CIPMUX의 값을 변경 후 시간 필요함
        sprintf(str, "AT+CIPSERVER=1,%d\r\n", DEFAULT_AP_MODE_PORT);
        sendWifiCommand(str);
        osDelay(delayTime*10);
        status = waitForResponse("OK");
        if(status == 1) printf("\nR:OK=CIPSERVER\n");
        if(status == 0) printf("\nError cipserver\n");
        clearRxWifiBuffer1();//*/
    }
    else {
        static uint8_t complete = 0;
        // +IPD,0,7:message
        char *rxMessage = receiveWifiPacket();
        if(rxMessage) printf("%s\n",rxMessage);
//        HAL_UART_Transmit(&huart1, (uint8_t *)rxMessage, strlen(rxMessage), strlen(rxMessage));
//
//
//        if(strncmp(rxMessage, "OPEN", 4) == 0) {
//            // send "STANDBY"
//            char txMessage[100];
//            sprintf(txMessage, "STANDYBY");
//            sprintf(str, "AT+CIPSEND=0,%d", strlen(txMessage));
//            sendWifiCommand(str);
//            while(getUart1() != '>');
//            sendWifiCommand(txMessage);
//            uint8_t status = waitForResponse("SEND OK");
//        }
//        if(strncmp(rxMessage, "SSID", 4) == 0) {
//            sprintf((char *)wSystem.ssid, "%s", &rxMessage[5]);
//            complete |= 0x02;
//        }
//        if(strncmp(rxMessage, "PASSWORD", 8) == 0) {
//            sprintf((char *)wSystem.password, "%s", &rxMessage[9]);
//            complete |= 0x04;
//        }
//        if(complete == 0x07) {
//            // 시스템 리부팅
//            HAL_NVIC_SystemReset();
//        }
    	// 디버그 중계코드
//    	char ch = getUart1();
//    	if(ch != 0) HAL_UART_Transmit(&huart2, &ch, 1, 1);
//    	ch = getUart2();
//    	if(ch != 0) HAL_UART_Transmit(&huart1, &ch, 1, 1);//*/
    }
}

void defaultApp(){
	while(1){
		osDelay(1);
	}
}

void msCallback(){
	if(timeoutWifiRx>0){
		timeoutWifiRx--;
	}
	if(timeoutReceiveWifi>0){
		timeoutReceiveWifi--;
	}
}

void Esp32ApmodeWifi(){
	//장치 초기화
	initUart1(&huart1);
	initUart2(&huart2);
	while(1){
		processAPmode();
//  	char ch = getUart1();
//  	if(ch != 0) HAL_UART_Transmit(&huart2, &ch, 1, 1);
//  	ch = getUart2();
//  	if(ch != 0) HAL_UART_Transmit(&huart1, &ch, 1, 1);
		osDelay(10);
	}
}


