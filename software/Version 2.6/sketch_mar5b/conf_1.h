// 

#define HELLO "conf_1"

#define CONF_1

#define SET_WIFI
#define SET_LED_REVERSE_ORDER

// #define SET_DB
// #define SET_HTTP
#define SET_BOT
#define SET_MQTT
// #define SET_OLED_M
#define SET_OLED
#define SET_TELNET 

#define SET_RADIO
#define SET_RADIO_RX

#define SET_PORT
#define SET_PORT_0
#define MAC_IN_PORT_0 'F'
#define SET_PORT_1
#define MAC_IN_PORT_1 'B'
#define SET_PORT_2
#define MAC_IN_PORT_2 'M'

#define SET_LIGHT
#define SET_TEMPERATURE
#define SET_HUMIDITY
#define SET_PRESSURE
#define SET_GAS

#define PREFIX_LIGHT "l"
#define PREFIX_TEMPERATURE "t"
#define PREFIX_HUMIDITY "h"
#define PREFIX_PRESSURE "p"
#define PREFIX_GAS_AQI "ai"
#define PREFIX_GAS_TVOC "at"
#define PREFIX_GAS_ECO2 "ac"

const char* items[] = { "time", "t", "t3", "t6", 
                        "h", "h3", "p", "p3", 
                        "@Воздух", "ai", "at", "ac", 
                        "@Освещён.", "l", "l3", "@", 
                        "@Разное", "sd3", "va3"};


#define MQTT_TOPIC "ESP/DataRow1"

// #define BOT_TOKEN 
// #define CHAT_TOKEN
// #define KEY 
// #define KEY_SCH 
#define SIZE_USER 25


#define DATA_BUF_COUNT 150
#define DATA_BUF_COUNT_RADIO 16
#define DATA_BUF_COUNT_ITEM 16
// #define PIN_RADIO_RX 13

// #define PIN_RADIO_RX 2
#define PIN_RADIO_RX 3
#define INTERRUPT_NUMDER 3
#define PIN_RADIO_LED 0
#define PIN_BUTTON_OLED 1

#define ADRESS_BUTTON_OLED 0x3E

#define SAMPLE_SIZE 8 // День
#define SAMPLE_SIZE_2 32 // Месяц
#define SAMPLE_SIZE_3 4 // Год

