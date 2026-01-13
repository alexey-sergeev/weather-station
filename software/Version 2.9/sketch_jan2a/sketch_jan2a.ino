// Проект "Метеостанция"
// Версия 
// 

#define VERSION "2.9"

// #include "conf_0.h" // Сервер

// #include "conf_1.h" // Дома
// #include "conf_3.h" // Улица
// #include "conf_6.h"    // Балкон
// #include "conf_7.h"    // Балкон

// #include "conf_2.h" // Дача
// #include "conf_4.h" // Дача

// #include "conf_5.h" // Лагерь
// #include "conf_8.h" // Серверная
// #include "conf_9.h" // Библиотека
// #include "conf_10.h" // Кабинет
// #include "conf_13.h" // Кафедра
// #include "conf_14.h" // Лаборатория Arduino
#include "conf_15.h" // Mini


#include "passwd.h"
#include "func.h"
#include "mString.h"
#include <StringUtils.h>

#include "fs.h"

#ifndef SET_WITHOUT_SENSORS
  #include "sensor.h"
#endif

#define INTERVAL_SENSOR_READIGS 1000 // 1 * 1000
#define INTERVAL_SAVE_TO_DB_DAY 120000 // 2 * 60 * 1000
#define INTERVAL_PUBLISH_NOW 20000 // 10 (должно больше, чем INTERVAL_PRINT_TX_NOW) * 1000
#define INTERVAL_PRINT_TX_NOW 10000 // 5 (должно меньше, чем INTERVAL_PUBLISH_NOW) * 1000
// #define INTERVAL_PUBLISH_NOW 10000 // 10 (должно больше, чем INTERVAL_PRINT_TX_NOW) * 1000
// #define INTERVAL_PRINT_TX_NOW 5000 // 5 (должно меньше, чем INTERVAL_PUBLISH_NOW) * 1000

#define PREFIX_NOW "$Now:"
#define PREFIX_DAY "$Day:"
#define PREFIX_MONTH "$Mon:"
#define PREFIX_YEAR "$Yea:"

#define GMT 10800 // 60 * 60 * 3

#ifdef SET_SETTINGS 
  #include "settings.h"
#endif

#ifdef SET_PORT_RX
  #include "port_rx.h"
  PortRX rx;
#endif

#include "all_sensors.h"

#ifdef SET_SLEEP
  #include <GyverPower.h>
#endif

#ifdef SET_PORT
  #include "port.h"

  #ifdef SET_PORT_0
    Port port0; 
  #endif

  #ifdef SET_PORT_1
    Port port1; 
  #endif

  #ifdef SET_PORT_2
    Port port2; 
  #endif

  #ifdef SET_PORT_3
    Port port3; 
  #endif

#endif

#ifdef SET_RADIO
  #include "radio.h"
#endif

#ifdef SET_BUTTON 
  #include "button.h"
#endif

#ifdef SET_DISPLAY 
  #include "display.h"
#endif

#ifdef SET_STRIP 
  #include "strip.h"
#endif

#ifdef SET_ALARM 
  #include "alarm.h"
#endif

#ifdef SET_OLED_M 
  #include "oled_m.h"
#endif

#ifdef SET_OLED 
  #include "oled.h"
#endif

#ifdef SET_WIFI
  #include "wifi.h"
#endif

#ifdef SET_FORECAST 
  #include "forecast.h"
#endif


#ifdef SET_DB 
  // #include "fs.h"
  #include "ftp.h"
  #include "db.h"
#endif

#ifdef SET_HTTP 
  #include "http.h"
#endif

#ifdef SET_BOT 
  #include "bot_ws.h"
#endif

#ifdef SET_MQTT 
  #include "mqtt.h"
#endif



void setup() {


  #ifdef SET_SLEEP
    
    Serial.begin(115200 * 2L);
    // Serial.begin(115200 * 1L);
    
    power.autoCalibrate();
    // power.hardwareDisable(PWR_ALL);
    // power.hardwareDisable(PWR_ADC | PWR_TIMER1);
    power.hardwareDisable(
      PWR_ADC |
      PWR_TIMER1 |
      // PWR_TIMER0 |
      PWR_TIMER2 |
      PWR_TIMER3 |
      PWR_TIMER4 |
      PWR_TIMER5 |
      PWR_UART0 |
      PWR_UART1 |
      PWR_UART2 |
      PWR_UART3 |
      PWR_I2C |
      PWR_SPI |
      PWR_USB
    );
    power.setSystemPrescaler(PRESCALER_1);
    // power.setSystemPrescaler(PRESCALER_2);
    power.setSleepMode(STANDBY_SLEEP);

  #else

    Serial.begin(115200);

  #endif

  Serial.println();
  Serial.print("Hello (");
  Serial.print(HELLO);
  Serial.println(")");

  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  Serial.print("Version: ");
  Serial.println(VERSION);

  // #ifdef SET_TELNET
  // #endif

  storage.init();

  #ifdef SET_SETTINGS 
    settings.init();
  #endif

  #ifdef MY_SDA
    Wire.begin(MY_SDA, MY_SLC);
  #endif

  #ifdef SET_AP
    wifi.initAP();
  #endif

  #ifdef SET_WIFI
    // wifi.initWiFi();
    wifi.initMulti();
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  #endif

  #ifdef SET_TELNET 
    Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));
    Log.begin();
  #endif

  #ifdef SET_BOT 
    telegramBot.init();
  #endif

  #ifdef SET_DB
    // storage.init();
    ftp.init();
    db.init();
  #endif

  #ifdef SET_HTTP
    http.init();
  #endif

  #ifdef SET_MQTT
    mqtt.init();
  #endif

  #ifdef SET_DISPLAY 
    display.init();
  #endif

  #ifdef SET_STRIP 
    strip.init();
  #endif

  #ifdef SET_OLED_M
    oledM.init();
  #endif

  #ifdef SET_OLED
    oled.init();
  #endif

  #ifdef SET_RADIO 
    radio.init();
  #endif

  #ifdef SET_BUTTON 
    button.init();
  #endif

  #ifdef SET_ALARM 
    alarm.init();
  #endif

  #ifdef SET_FORECAST 
    forecast.init();
  #endif

  sensors.init();

  #ifndef ESP8266
    analogReference(EXTERNAL);
  #endif

}



void loop() {


  // Проверка wi-fi
  #ifdef SET_WIFI

    wifi.runMulti();
  
  #endif

  #ifdef SET_TELNET 

    Log.loop();

  #endif

  // Проверка MQTT
  #ifdef SET_MQTT
  
    mqtt.run();
  
  #endif


  #ifdef SET_PORT_RX

    rx.read();

  #endif


  #ifdef SET_RADIO
  
    radio.run();
  
  #endif

  // Ввод показаний датчиков
  sensors.run();


  #ifdef SET_ALARM

     alarm.run(); 

  #endif


  #ifdef SET_BOT 

    // Писать и читать сообщения бота 
    telegramBot.tick();
    telegramBot.run();
  
  #endif

  #ifdef SET_DISPLAY 

    display.run();

  #endif

  #ifdef SET_STRIP 

    strip.run();

  #endif

  #ifdef SET_OLED_M 

    // Писать в OLED 
     oledM.run(); 

  #endif


  #ifdef SET_OLED

    // Писать в OLED 
     oled.run(); 

  #endif


  #ifdef SET_FORECAST 

    forecast.run();

  #endif


  #ifdef SET_SETTINGS 

    settings.run();

  #endif


  #ifdef SET_UNIT

    // Вывод показаний датчиков
    sensors.txPrint();
  
  #endif


  #ifdef SET_HTTP

    // Помощник серверов
    http.handle();

  #endif

  #ifdef SET_DB

    ftp.handle();
    
    // Вставка данных
    // Интервал обновления:
    //  - 2 минуты (дневная таблица)
    //  - 1 час (месячная таблица)
    //  - 3 часа (годовая таблица)
    db.insert();

    // Очистка базы данных
    // Один раз в день с 3 до 4 часов, но один раз
    db.purge();

  #endif

  #ifdef SET_SLEEP

    delay(10); 
    power.sleepDelay(3000); 

  #endif

}


