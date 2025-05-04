# What to do?
- 2개의 stm32보드를 가지고 ESP32모듈을 사용하여 한쪽은 Ap mode, 다른 한쪽은 Station mode로 통신하여 Ap mode 쪽에선 OLed모니터에 나와있는 메뉴를 선택하면 제어 명령을 반대편 Station쪽으로 날려
station쪽에 달려있는 모터 및 RGB LED를 제어하는 프로젝트


- 각각의 모드에선 여러 Task가 동시에 실행되야 하므로 FreeRTOS를 사용하여 여러 테스크가 동시에 실행되도록 구성

# Block Diagram
![image](https://github.com/user-attachments/assets/a87a190a-18da-4680-8ddd-5b7cc7a01fdf)
