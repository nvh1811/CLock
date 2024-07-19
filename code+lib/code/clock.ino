#include <RTClib.h>
#include <LedControl.h>

LedControl segment = LedControl(11, 13, 12, 1);

RTC_DS1307 RTC;

int analog_value = 1023;

class Port {
  byte length;
  byte* pins;
  byte level;
public: 
  Port(byte length, byte level)
  : length(length), level(level){}
  void Connect(byte* pins, byte mode){
    this->pins = pins;
    for(int i = 0; i < length; i++){
      pinMode(pins[i], mode);
    }
  }
  void Write(int value) {
    if(level == 0) value = ~value;
    for(int i = 0; i < length; i++){
      digitalWrite(pins[i], value & (1 << i));
    }
  }
};
class Led7Seg : Port {
byte* pin;
public:
  Led7Seg(byte* pins, bool anode = true) 
  : Port(7, anode ? 0 : 1) {
    this->pin = pins;
    Port::Connect(pins, OUTPUT);
  }
  void Display(int d){
    static int code[]= {
      0x3F,    //0
      0x06,    //1
      0x5B,    //2
      0x4F,    //3
      0x66,    //4
      0x6D,    //5
      0x7D,    //6
      0x07,    //7
      0x7F,    //8
      0x6F     //9
    };
      Port::Write(code[d]);
    }   
};
byte pins[] = { 8, 7, 6, 5, 4, 3, 2};  // g - a
int digit[] = { A0, A1, A2, A3, 10};  // 1 - 6
int dl = 4;

Led7Seg led(pins, 1);

int button = 9;
int check;

void setup() {
    Serial.begin(57600);
    RTC.begin();

    segment.shutdown(0, false); // Bật hiển thị
    segment.setIntensity(0, 1); // Đặt độ sáng
    segment.clearDisplay(0); // Tắt tất cả led

    pinMode(button, INPUT_PULLUP);
}

void loop() {
  
  check = digitalRead(button);
  Serial.println(check);
  if(check == 0){
    ngoclinh();
    gio_phut();
  }
  else {
    ngay_thang();
    gio_phut();
  } 
}

void ngoclinh(){
    segment.setChar(0, 7, 'N', 0); //ngày
    segment.setChar(0, 6, 'g', 0); //ngày
    segment.setChar(0, 5, 'O', 0); //năm
    segment.setChar(0, 4, 'C', 0); //tháng
    segment.setChar(0, 3, 'L', 0); //tháng
    segment.setChar(0, 2, 'i', 0); //năm
    segment.setChar(0, 1, 'n', 0); //năm
    segment.setChar(0, 0, 'h', 0); //năm
}

void gio_phut(){
    DateTime now = RTC.now();

    int hour = now.hour();
    int minute = now.minute();
    int second = now.second();

    int thu = now.dayOfTheWeek() + 1;

    led.Display(hour / 10);
    analogWrite(digit[0], analog_value);
    delay(dl);
    analogWrite(digit[0], 0);

    led.Display(hour % 10);
    analogWrite(digit[1], analog_value);
    delay(dl);
    analogWrite(digit[1], 0);

    led.Display(minute / 10);
    analogWrite(digit[2], analog_value);
    delay(dl);
    analogWrite(digit[2], 0);

    led.Display(minute % 10);
    analogWrite(digit[3], analog_value);
    delay(dl);
    analogWrite(digit[3], 0);

    led.Display(thu);
    analogWrite(digit[4], analog_value);
    delay(dl);
    analogWrite(digit[4], 0);


    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println();  
}

void ngay_thang(){
    DateTime now = RTC.now();

    int day = now.day();
    int month = now.month();
    int year = now.year();

    segment.setDigit(0, 7, day / 10, 0); //ngày
    segment.setDigit(0, 6, day % 10, 0); //ngày
    segment.setChar(0, 5, '-', 0); //năm
    segment.setDigit(0, 4, month / 10, 0); //tháng
    segment.setDigit(0, 3, month % 10, 0); //tháng
    segment.setChar(0, 2, '-', 0); //năm
    segment.setDigit(0, 1, (year / 10) % 10, 0); //năm
    segment.setDigit(0, 0, year % 10, 0); //năm

    Serial.print(now.day());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.year());
    Serial.println();  

}
