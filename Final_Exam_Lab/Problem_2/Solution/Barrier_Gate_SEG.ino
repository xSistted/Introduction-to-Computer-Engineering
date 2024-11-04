const short gate_Led = 5;
const short	full_Led = 4;
const short	gate_button = 3;
const short car_button = 2;

int lastCarButtonState = 0;
int car_amount = 0;

unsigned long lastDebounceCar = 0;
unsigned long delayTime = 100;

const short segment_pin[8] = {13, 12, 11 ,10, 9, 8, 7, 6};
int bit_pattern[10] = {0b11111100,  //0
                       0b01100000,  //1
                       0b11011010,  //2
                       0b11110010,  //3
                       0b01100110,  //4
                       0b10110110,  //5
                       0b10111110,  //6
                       0b11100000,  //7
                       0b11111110}; //8

void display_Seg(int number);
void blink_Seg();

void setup(){
  Serial.begin(9600);
  pinMode(gate_button, INPUT);
  pinMode(car_button, INPUT);
  pinMode(gate_Led, OUTPUT);
  pinMode(full_Led, OUTPUT);
  
  for(int i = 0; i < 8; i++){
    pinMode(segment_pin[i], OUTPUT);
  }
}

void loop(){
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
      
      display_Seg(car_amount);
      if(car_amount > 7){
        blink_Seg();
        digitalWrite(full_Led, 1);
      }
    }
  }
  else{
    digitalWrite(gate_Led, 1);
    display_Seg(car_amount);
    if(car_amount > 7){
      blink_Seg();
      digitalWrite(full_Led, 1);
    }
  }
}

void display_Seg(int number){
  int pattern = bit_pattern[number];
  for(int i = 0; i < 8; i++){
    bool isOn = bitRead(pattern, 7-i);
    digitalWrite(segment_pin[i], isOn ? LOW:HIGH);
  }
  delay(delayTime);
}

void blink_Seg(){
  for(int i = 0; i < 8; i++){
    digitalWrite(segment_pin[i], HIGH);
  }
  delay(delayTime);
}