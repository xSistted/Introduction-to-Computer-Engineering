#include <LedControl.h>

LedControl lc = LedControl{11, 13, 10, 4}; // 11_DIN 13_CLK 10_CS 4_Number of LED Module 

const short car_button = 2; //ปุ่มรับสัญญาณรถเข้า
const short gate_button = 3; //ปุ่มเปิด/ปิดเกท
const short full_Led = 5;
const short gate_Led = 6;

int lastCarButtonState = 0;

unsigned long lastDebounceCar = 0; //debouncedelay ปุ่มรถ
unsigned long delayTime = 100;

int devices = 0;
int car_amount = 0;

void display_Dot(int number); //แสดงผลไฟบน dot_matrix
void blink_Dot(); //กระพริบไฟบน dot_matrix

void setup() {
  devices = lc.getDeviceCount();
  //ตั้งค่าแค่ address 0 เนื่องจากใช้ dot_matrix แค่ 1 ช่อง
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(car_button, INPUT);
  pinMode(gate_button, INPUT);
  pinMode(gate_Led, OUTPUT);
  pinMode(full_Led, OUTPUT);
}

void loop() {
  if(digitalRead(gate_button) == 1){
    digitalWrite(gate_Led, 0);
    while(digitalRead(gate_button) == 1){
      int currentCarButtonState = digitalRead(car_button);
      if(lastCarButtonState != currentCarButtonState){
          if(digitalRead(car_button) == 1 && millis() - lastDebounceCar > delayTime){
          lastDebounceCar = millis();
          car_amount += 1;
        }
      }
      lastCarButtonState = currentCarButtonState;
      
      display_Dot(car_amount);
      if(car_amount > 7){
        blink_Dot();
      }
    }
  }
  else{
    digitalWrite(gate_Led, 1);
    display_Dot(car_amount);
    if(car_amount > 7){
      blink_Dot();
    }
  }
}

void display_Dot(int number){
  devices = lc.getDeviceCount();
  for(int row = 0; row < number; row++){
    for(int column = 0; column < 8; column++){
      lc.setLed(0, row, column, true);
    }
  }
  if(car_amount > 7){
    digitalWrite(full_Led, 1);
  }
}

void blink_Dot(){
  devices = lc.getDeviceCount();
  for(int row = 0; row < 8; row++){
    for(int column = 0; column < 8; column++){
      lc.setLed(0, row, column, false);
    }
  }
}