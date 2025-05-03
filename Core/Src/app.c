/*
 * app.c
 *
 *  Created on: Mar 11, 2025
 *      Author: wjdek
 */
#include "app.h"
#include "uart.h"				// for rgbLED
#include "string.h"			// for rgbLED
#include "stdlib.h"			// for rgbLED

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;		// for ESP32, rgbLED
extern TIM_HandleTypeDef htim1;
extern TimerHandle_t motorStopTimer;
extern TIM_HandleTypeDef htim3;			// for rgbLED


uint32_t timeoutWifiRx;
uint32_t timeoutReceiveWifi;
char* rxMessage;


// 모터 정지 타이머 콜백 함수
void motorStopTimerCallback(TimerHandle_t xTimer) {
	HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 0);
	HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
	htim1.Instance -> CCR1 = 0;
}

void appMotor(){
	// 초기화
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	// 큐에서 내용을 읽어오기
//	xQueueReceive(motorCmdQueue, &data, osWaitForever);

	/*if(rxMessage[0] == '0'){
		if(rxMessage[1] == '0'){
			// without BEMF (Back Electro Motive Force : 역기전력)
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance -> CCR1 = 99;
		}else if(rxMessage[1] == '1'){
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance -> CCR1 = 89;
		}else if(rxMessage[1] == '2'){
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance -> CCR1 = 79;
		}else if(rxMessage[1] == '3'){
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 0);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance -> CCR1 = 0;
		}
		if(rxMessage[2] == '0'){
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(5000), 0);
			xTimerStart(motorStopTimer, 0);
		}else if(rxMessage[2] == '1'){
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(10000), 0);
			xTimerStart(motorStopTimer, 0);
		}else if(rxMessage[2] == '2'){
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(15000), 0);
			xTimerStart(motorStopTimer, 0);
		}else if(rxMessage[2] == '3'){
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 0);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance -> CCR1 = 0;
		}
		rxMessage[0] = '2';
	}*/


	while (1) {
		if (rxMessage[0] == '0' && rxMessage[1] == '0' && rxMessage[2] == '0') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 0);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 0;
		} else if (rxMessage[0] == '0' && rxMessage[1] == '1'&& rxMessage[2] == '1') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 79;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(5000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '1'&& rxMessage[2] == '2') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 79;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(10000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '1'&& rxMessage[2] == '3') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 79;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(15000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '2'&& rxMessage[2] == '1') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 89;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(5000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '2'&& rxMessage[2] == '2') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 89;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(10000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '2'&& rxMessage[2] == '3') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 89;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(15000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '3'&& rxMessage[2] == '1') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 99;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(5000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '3' && rxMessage[2] == '2') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 99;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(10000), 0);
			xTimerStart(motorStopTimer, 0);
		} else if (rxMessage[0] == '0' && rxMessage[1] == '3'&& rxMessage[2] == '3') {
			HAL_GPIO_WritePin(Motor1_GPIO_Port, Motor1_Pin, 1);
			HAL_GPIO_WritePin(Motor2_GPIO_Port, Motor2_Pin, 0);
			htim1.Instance->CCR1 = 99;
			xTimerChangePeriod(motorStopTimer, pdMS_TO_TICKS(15000), 0);
			xTimerStart(motorStopTimer, 0);
		}
		osDelay(5);
	}

}

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
        rxMessage = receiveWifiPacket();
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

/////////////////////////////////////////////////////////
void setRGB(uint16_t r, uint16_t g, uint16_t b){
	htim3.Instance->CCR1 =r;
	htim3.Instance->CCR2 =g;
	htim3.Instance->CCR3 =b;
}


void rgbLEDapp() {
	//uint16_t data[3];				//ESP32로 부터 받는 값
	//uint16_t data1 = 0;
	// 타이머 시작
	// HAL_TIM_Base_Start(&htim1); //아무 기능 없이 카운트로만 사용할 때
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  // PWM
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);  // PWM
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);  // PWM
	// uart 장치 초기화
	//initUart(&huart2);

	setRGB(0, 0, 0);
	while(1){
		if (rxMessage[0] == '0' && rxMessage[1] == '0' && rxMessage[2] == '0') {
			setRGB(0, 0, 0);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '1'&& rxMessage[2] == '1') {
			setRGB(333, 0, 0);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '1'&& rxMessage[2] == '2') {
			setRGB(666, 0, 0);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '1' && rxMessage[2] == '3') {
			setRGB(999, 0, 0);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '2'&& rxMessage[2] == '1') {
			setRGB(0, 333, 0);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '2'&& rxMessage[2] == '2') {
			setRGB(0, 666, 0);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '2'&& rxMessage[2] == '3') {
			setRGB(0, 999, 0);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '3'&& rxMessage[2] == '1') {
			setRGB(0, 0, 333);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '3'&& rxMessage[2] == '2') {
			setRGB(0, 0, 666);
		} else if (rxMessage[0] == '1' && rxMessage[1] == '3'&& rxMessage[2] == '3'){
			setRGB(0, 0, 999);
		}
		osDelay(3);
	}
/*		static char rxData[5]; // 수신된 데이터를 순차적으로 저장 [x][100][10][1][null]
		static uint8_t pos = 0;   // 수신된 데이터의 저장위치
		static uint16_t valueR = 0, valueG =0, valueB =0;
		char ch = getUart();
		if(ch != 0) {
			//개형문자를 만나면
			if(ch == '\n'){
				//데이터 파싱
				int value = atoi(&rxData[1]); //ascii 문자열(string)을 숫자로 변환
				// 명텽에 따른 동작
				switch(rxData[0]){
					case 'R':
						valueR = value;
						break;
					case 'G':
						valueG = value;
						break;
					case 'B':
						valueB = value;
						break;
				}
				setRGB(valueR, valueG, valueB);
				// 기존 데이터 삭제
				memset(rxData, 0, 5);
				pos =0;
			}
			else{
				rxData[pos++] =ch;
				pos %= 5;
			}
		}
	}
*/
}






