// Класс прогноз погоды

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include <Stamp.h>

#define WEATHER_URL "http://api.openweathermap.org/data/2.5/weather?q=Volgograd,ru,pt&APPID="
#define FORECAST_URL "http://api.openweathermap.org/data/2.5/forecast?q=Volgograd,ru,pt&cnt=7&APPID="

#define WEATHER_TTL 3600000 // 60 * 60 * 1000
#define WEATHER_INTERVAL 885000 // 15 * 59 * 1000
#define WEATHER_INTERVAL_START 51000 // 51 * 1000

#define FORECAST_TTL 10800000 // 3 * 60 * 60 * 1000
#define FORECAST_INTERVAL 3600000 // 60 * 60 * 1000
#define FORECAST_INTERVAL_START 53000 // 53 * 1000

#define FORECAST_SIZE 8


struct ForecastAttribute {
  uint32_t time;
  uint32_t dt;
  float temp;
};

ForecastAttribute weather;
ForecastAttribute fa[FORECAST_SIZE];



class Forecast { 
  public: 

    void init() {



    }




    void run() {

      static bool flagWeather = true;
      static bool flagWeatherStrart = true;
      static uint32_t timerWeather;
      
      if ( ( millis() - timerWeather > WEATHER_INTERVAL && !flagWeather ) ||
           ( millis() - timerWeather > WEATHER_INTERVAL_START && flagWeather ) ||
           flagWeatherStrart ) {

        timerWeather = millis();
        flagWeatherStrart = false;

        if ( _parsing(_getForecast('w'), 'w') ) flagWeather = false;

      }


      static bool flagForecast = true;
      static uint32_t timerForecast;
      
      if ( ( millis() - timerForecast > FORECAST_INTERVAL && !flagForecast ) ||
           ( millis() - timerForecast > FORECAST_INTERVAL_START && flagForecast ) ) {

        timerForecast = millis();
        if ( _parsing(_getForecast('f'), 'f') ) flagForecast = false;

      }

    }




    String showForecast(char wf = 'w', uint8_t n = 0) {

      String s;

      if ( wf == 'w' ) {

        if ( millis() - weather.time < WEATHER_TTL ) {
          
          s = (String) weather.temp;
          p("Forecast (now): ");
          pln(s);
        
        } else pln("Has expired (weather)");

      }

      if ( wf == 'f' ) {


        if ( millis() - fa[0].time < FORECAST_TTL ) {
          
          if ( n == 0 ) s = _creatingTemp(0, 5);
          if ( n == 1 ) s = _creatingTemp(6, 11);
          if ( n == 2 ) s = _creatingTemp(12, 17);
          if ( n == 3 ) s = _creatingTemp(18, 23);

        } else pln("Has expired (forecast)");

      }

      return s;

    }




  private:



    String _creatingTemp(uint8_t t1, uint8_t t2) {

      String s = "";

      bool f = false;
      int16_t ma = -300;
      int16_t mi = 10000;
      
      for ( uint8_t i = 0; i < FORECAST_SIZE; i++ ) {

        if ( fa[i].dt != 0 ) {

          Stamp stamp(fa[i].dt);
          Datime d = stamp.get();

          if ( t1 <= d.hour && d.hour <= t2 ) {

            mi = min(mi, (int16_t) fa[i].temp);
            ma = max(ma, (int16_t) fa[i].temp);

            f = true;

          }

        }

      } 
            
      if ( f ) {

        if ( ma == mi ) s = (String) ma + "°";
        else s = (String) mi + "°..." + (String) ma + "°";  

      } else pln("Forecast: No data");

      return s;

    }





    String _getForecast(char wf = 'w') {

      String s = "";

      WiFiClient client;
      HTTPClient h;

      char urlWeather[] = WEATHER_URL FORECAST_APPID;
      char urlForecast[] = FORECAST_URL FORECAST_APPID;

      if ( h.begin(client, ( wf == 'w' ) ? urlWeather : urlForecast) ) {
      
        uint16_t httpCode = h.GET();

        if ( httpCode > 0 ) {

          if ( httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY ) {
  
            s = h.getString();

          } else pln( "Forecast: HTTP error (1)");

        } else pln( "Forecast: HTTP error (2)");

        h.end();
    
      } else pln( "Forecast: HTTP error (3)");;

      return s;

    }




    bool _parsing(String s, char wf = 'w') {

      StaticJsonDocument<1500> doc;
      DeserializationError error = deserializeJson(doc, s);

      if ( error ) {

        p("Forecast: deserializeJson() failed: ");
        pln(error.c_str());

        return false;

      }

      if ( wf == 'w' ) {

        weather.time = millis();
        weather.temp = (float) doc["main"]["temp"] - 273.15;

      }

      if ( wf == 'f' ) {

        for ( uint8_t i = 0; i < FORECAST_SIZE; i++ ) fa[i].time = 0;
        
        for ( uint8_t i = 0; i < FORECAST_SIZE; i++ ) {

          fa[i].time = millis();
          
          fa[i].temp = (float) doc["list"][i]["main"]["temp"] - 273.15;
          fa[i].dt = (uint32_t) doc["list"][i]["dt"];
          
        }

      }

      return true;

    }

};

Forecast forecast;
