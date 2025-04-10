
#define N_NOW 0
#define N_DAY 1
#define N_MONTH 2
#define N_YEAR 3


#ifdef SET_TELNET 
  #include <TLog.h>
  #include <TelnetSerialStream.h>
  TelnetSerialStream telnetSerialStream = TelnetSerialStream();
#endif


void pln() {
  #ifdef SET_TELNET 
    Log.println();
  #else
    Serial.println();
  #endif
}

void p(char a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(char a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(char* &a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(char* &a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(const char* a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(const char* a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(String &a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(String &a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(uint8_t a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(uint8_t a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(uint16_t a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(uint16_t a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(int a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(int a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(uint32_t a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(uint32_t a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}

void p(float a) {
  #ifdef SET_TELNET 
    Log.print(a);
  #else
    Serial.print(a);
  #endif
}

void pln(float a) {
  #ifdef SET_TELNET 
    Log.println(a);
  #else
    Serial.println(a);
  #endif
}





#ifdef SET_ID_DEVICE

  #include <ESP8266WiFi.h>

  uint16_t idDevice() {

    uint16_t id;

    uint8_t mac[6];
    WiFi.macAddress(mac);

    id += mac[0] >> 6 << 14;
    id += mac[1] >> 6 << 12;
    id += mac[2] >> 6 << 10;
    id += mac[3] >> 6 << 8;
    id += mac[4] >> 4 << 4;
    id += mac[5] >> 4 << 0;

    return id;

  }

#endif



#ifndef ESP8266

  extern int __bss_end;
  extern void *__brkval;

#endif
  

  
// Функция, возвращающая количество свободного ОЗУ (RAM)
uint16_t memoryFree()
{

  #ifndef ESP8266

    uint16_t freeValue;
    if((int)__brkval == 0)
        freeValue = ((int)&freeValue) - ((int)&__bss_end);
    else
        freeValue = ((int)&freeValue) - ((int)__brkval);
    return freeValue;

  #else

    return 0;

  #endif

}

