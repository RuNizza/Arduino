#include <Wire.h>
#include <LiquidCrystal_I2C.h> // библиотека LCD экрана
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
#include <DS3231.h>       //  библиотека часов

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно


// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

DS3231 clock;   // какие то настройки часов
RTCDateTime dt; // что то от часов

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

void setup()
{
  //Устанавливаем для 7 цифрового пина режим выход
  pinMode(7, OUTPUT);
  digitalWrite(7,1);
  
  // Настройка радио модуля

  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_2MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
 
  // Инициализация часов
  Serial.println("Initialize DS3231");;
  clock.begin();

  // Установка времени
  clock.setDateTime(__DATE__, __TIME__);

  // Инициализация экрана
    
	lcd.begin();
	lcd.backlight();

	lcd.print("Radio clock");
	lcd.setCursor(0, 1);
	lcd.print("ver. 1.0");
	delay(5000);


  int u=1200;
  int z=0;
  while ( u != 0 ) {
      lcd.clear(); // отчистка экрана
      dt = clock.getDateTime(); // Получаем время 
      lcd.print(dt.hour);
      lcd.print(":");
      lcd.print(dt.minute);
      lcd.print(":");
      lcd.print(dt.second);
      radio.write(&dt.hour, sizeof(dt.hour));
      radio.write(&dt.minute, sizeof(dt.minute));
      radio.write(&dt.second, sizeof(dt.second));
      if ( dt.second == 0) {
        lcd.print(" Rele ON");
        digitalWrite(7,0);
        z=1;
      }
      else {
        if (z == 1) {
          digitalWrite(7,1);
          z=0;  
        }
        
      }
      delay(1000);
  }
}






void loop()
{
	// Do nothing here...
}

