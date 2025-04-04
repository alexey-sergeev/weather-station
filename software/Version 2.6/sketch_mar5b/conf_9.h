// 
// Конфигурация библиотеки
// 


#define HELLO "conf_9"
#define CONF_9


// Wi-Fi

#define SET_WIFI
// #define SET_LED_REVERSE_ORDER
// #define SSID_0 ""
// #define PASS_0 ""

// #define SET_OTA
// #define PASSWORD_OTA "" 


// Сервисы

#define SET_BOT
#define SET_ID_DEVICE
#define SIZE_USER 25
// #define KEY "sa:iYEp"
// #define KEY_SCH "sa:LaJR"
// #define BOT_TOKEN "" // Смотри https://t.me/BotFather

#define SET_DB

#define SET_HTTP
// #define LOGIN_FTP ""
// #define PASSWORD_FTP "" 

#define SET_MQTT
#define MQTT_TOPIC "ESP/DataRow9"
// #define MQTT_SERVER "" // Смотри https://clusterfly.ru/ или https://www.wqtt.ru/
// #define MQTT_PORT 
// #define MQTT_USER ""
// #define MQTT_PASSWORD "" 

// #define SET_TELNET 
// #define SET_UPDATE


// Датчик температуры

#define SET_TEMPERATURE

#define SET_SHT
// #define SET_SHT_HEATER
#define PREFIX_TEMPERATURE "t"


// Датчик влажности

#define SET_HUMIDITY
#define PREFIX_HUMIDITY "h"


// Параметры датчиков

#define MY_SDA 0
#define MY_SLC 3

#define DATA_BUF_COUNT 64

#define SAMPLE_SIZE 8 // День
#define SAMPLE_SIZE_2 32 // Месяц
#define SAMPLE_SIZE_3 4 // Год

