// 
// Конфигурация серверной 
// 


#define HELLO "conf_8"
#define CONF_8


// Wi-Fi

#define SET_WIFI
#define SET_LED_REVERSE_ORDER
// #define SSID_0 ""
// #define PASS_0 ""

#define SET_OTA
// #define PASSWORD_OTA "" 

// Сервисы

#define SET_BOT
#define SIZE_USER 25
// #define KEY "sa:iYEp"
// #define KEY_SCH "sa:LaJR"
// #define BOT_TOKEN "" // Смотри https://t.me/BotFather

#define SET_DB

#define SET_HTTP
// #define LOGIN_FTP ""
// #define PASSWORD_FTP "" 

#define SET_MQTT
#define MQTT_TOPIC "ESP/DataRow8"
// #define MQTT_SERVER "" // Смотри https://clusterfly.ru/ или https://www.wqtt.ru/
// #define MQTT_PORT 
// #define MQTT_USER ""
// #define MQTT_PASSWORD "" 

#define SET_TELNET 

#define SET_UPDATE


// Кнопка

#define SET_BUTTON
#define PIN_BUTTON 3


// Дисплей

#define SET_DISPLAY
#define PIN_CLK 12
#define PIN_DIO 13


// Тревога

#define SET_ALARM 

#define PIN_BUZZER 16
#define PIN_LED_ALARM 0

#define TEMPERATURE_MIN 17
#define TEMPERATURE_MAX 28

#define BATTERY_CHARGED_75 3.9
#define BATTERY_CHARGED_50 3.8
#define BATTERY_CHARGED_25 3.7
#define BATTERY_CHARGED_0 3.6


// Прогноз погоды

#define SET_FORECAST
// #define FORECAST_APPID "" // Смотри https://openweathermap.org/


// Датчик температуры

#define SET_TEMPERATURE

#define SET_BMP 
#define PREFIX_TEMPERATURE "t8"


// Напряжение в аккумуляторе

#define SET_VOLTAGE_0

#define PIN_VOLTAGE_0 A0
#define COEF_VOLTAGE_0 4.81
#define PREFIX_VOLTAGE_0 "va8"


// Датчик питания

#define SET_POWER

#define PIN_POWER 14
#define POWER_MAX 5
#define PREFIX_POWER "vb8"


// Параметры датчиков

#define DATA_BUF_COUNT 64

#define SAMPLE_SIZE 8 // День
#define SAMPLE_SIZE_2 32 // Месяц
#define SAMPLE_SIZE_3 4 // Год

