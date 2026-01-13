// 
// Конфигурация в кабинете
// 

#define HELLO "conf_10"

#define CONF_10

#define SET_WIFI
// #define SET_LED_REVERSE_ORDER
// #define PIN_LED_WIFI 16

#define SET_DB
#define SET_HTTP
#define SET_BOT
// #define SET_PORT
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
#define PREFIX_TEMPERATURE_B "t10"
// #define PREFIX_HUMIDITY "h10"
// #define PREFIX_PRESSURE "p2"
#define PIN_TEMPERATURE_B 14

// #define DATA_BUF_COUNT_RADIO 64
// #define PIN_RADIO_RX 2

#define SET_TEMPERATURE_S
#define PREFIX_TEMPERATURE_S "st10"

// Дисплей

#define SET_DISPLAY
#define PIN_CLK 12
#define PIN_DIO 13
#define DISPLAY_LEN 10
#define SHOW_INTERVAL 4000
#define SHOW_LEN 3

#define MQTT_TOPIC "ESP/DataRow10"

#define SIZE_USER 25

#define DATA_BUF_COUNT 128
// #define DATA_BUF_COUNT_RADIO 16
#define DATA_BUF_COUNT_ITEM 16

#define SAMPLE_SIZE 8 // День
#define SAMPLE_SIZE_2 32 // Месяц
#define SAMPLE_SIZE_3 4 // Год

