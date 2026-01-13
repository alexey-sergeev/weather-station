// 
// Конфигурация в лаборатории Arduino
// 

#define HELLO "conf_14"

#define CONF_14

#define SET_WIFI
// #define SET_LED_REVERSE_ORDER
// #define PIN_LED_WIFI 16

#define SET_DB
#define SET_HTTP
#define SET_BOT
#define SET_MQTT
#define SET_OTA
#define SET_TELNET 
#define SET_UPDATE

// Прогноз погоды

#define SET_FORECAST
// #define FORECAST_APPID "" // Смотри https://openweathermap.org/

// #define SET_AHT 

#define SET_TEMPERATURE_B
// #define SET_HUMIDITY
// #define SET_PRESSURE
// #define SET_GAS

// #define PREFIX_LIGHT "l"
#define PREFIX_TEMPERATURE_B "t14"
// #define PREFIX_HUMIDITY "h10"
// #define PREFIX_PRESSURE "p2"
#define PIN_TEMPERATURE_B 14

// #define DATA_BUF_COUNT_RADIO 64
// #define PIN_RADIO_RX 2

#define SET_TEMPERATURE_S
#define PREFIX_TEMPERATURE_S "st14"

// Дисплей

#define SET_DISPLAY
#define DISPLAY_COLON
#define PIN_CLK 5
#define PIN_DIO 4
#define DISPLAY_LEN 10
#define SHOW_INTERVAL 2000
#define SHOW_LEN 10

// Лента

#define SET_STRIP
#define PIN_STRIP 0
#define LED_NUM 1




#define MQTT_TOPIC "ESP/DataRow14"

#define SIZE_USER 25

#define DATA_BUF_COUNT 128
// #define DATA_BUF_COUNT_RADIO 16
#define DATA_BUF_COUNT_ITEM 16

#define SAMPLE_SIZE 8 // День
#define SAMPLE_SIZE_2 32 // Месяц
#define SAMPLE_SIZE_3 4 // Год

