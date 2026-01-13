// 

#define BUF_NOW_COUNT 8


// #include "sensor.h"

// Библиотека датчиков

#include <Wire.h>
#ifdef SET_LIGHT

  Sensor sLight;

  #include <BH1750.h>
  BH1750 lightMeter;

#endif

#ifdef SET_LIGHT_A

  Sensor sLightA;

#endif

#ifdef SET_TEMPERATURE 

  Sensor sTemperature;

  // #ifdef SET_BMP 
  #if defined SET_BMP

    #include <Wire.h>
    #include <iarduino_Pressure_BMP.h>
    iarduino_Pressure_BMP bmp; 

  // #ifdef SET_SHT
  #elif defined SET_SHT

    #include <Wire.h>
    #include "Adafruit_SHT31.h"
    Adafruit_SHT31 sht = Adafruit_SHT31();
  
  #elif defined SET_AHT

    #include <Wire.h>
    #include <AHT20.h>
    AHT20 aht;

  #else
    
    #include <GyverHTU21D.h>
    GyverHTU21D htu;

  #endif
  
#endif

#ifdef SET_TEMPERATURE_B 

  Sensor sTemperatureB;

  #include <microDS18B20.h>
  MicroDS18B20<PIN_TEMPERATURE_B> mds; 

#endif


#ifdef SET_HUMIDITY

  Sensor sHumidity;

  // #include <GyverHTU21D.h>
  // GyverHTU21D htu;

#endif

#ifdef SET_PRESSURE 

  Sensor sPressure;

  #ifdef SET_BMP 

    // #include <Wire.h>
    // #include <iarduino_Pressure_BMP.h>
    // iarduino_Pressure_BMP bmp; 

  #else
    
    #include <GyverBME280.h>
    GyverBME280 bme;

  #endif

#endif

#ifdef SET_GAS 

  Sensor sGasAQI;
  Sensor sGasTVOC;
  Sensor sGasECO2;

  #include <DFRobot_ENS160.h>
  DFRobot_ENS160_I2C ENS160(&Wire, 0x53);

#endif

#ifdef SET_SOUND 

  Sensor sSound;
  #include "sound.h"

#endif

#ifdef SET_RAIN 

  Sensor sRain;

#endif

#ifdef SET_GROUND 

  Sensor sGround;

#endif

#ifdef SET_ULTAVIOLET

  Sensor sUltraviolet;

#endif

#ifdef SET_WIND 

  Sensor sWind;

#endif

#ifdef SET_VOLTAGE_0 

  Sensor sVotage0;

#endif

#ifdef SET_VOLTAGE_1 

  Sensor sVotage1;

#endif

#ifdef SET_POWER 

  Sensor sPower;

#endif

#ifdef SET_TEMPERATURE_S

  Sensor sTemperatureS;

#endif

float sTemperatureS_buf;  //####!!!!!


class all_sensors { 
  public: 

    all_sensors() {
    
      #ifdef SET_HTTP
  
        for ( int i = 0; i < BUF_NOW_COUNT; i++ ) _bufDataNow[i] = "";
  
      #endif    

    }


    // Оформить строку данных

    String creatingDataRow(uint8_t n = 0) {

      // pln(memoryFree());

      // String s = "";
      static String s = "";
      s.reserve(DATA_BUF_COUNT);

      // pln(memoryFree());

      static uint32_t timer = 0;
      static bool flag = false;

      if ( millis() - timer < 1000 && flag && millis() > 10000 ) return s;
      
      timer = millis();
      flag = true;
      s = "";

      #ifdef SET_WIFI
        time_t t = time(NULL);
        s += (String) t;
      #endif

      #ifdef SET_UNIT
        if ( n == N_NOW ) s += PREFIX_NOW;
        if ( n == N_DAY ) s += PREFIX_DAY;
        if ( n == N_MONTH ) s += PREFIX_MONTH;
        if ( n == N_YEAR ) s += PREFIX_YEAR;
      #endif

      #ifdef SET_ID_DEVICE
        s +=  " id:";
        s += idDevice();
        // s += wifi.idDevice();
      #endif

      #ifdef SET_HTTP
        s += " ";
        s += _joinBufDataNow();
      #endif

      #ifdef SET_TEMPERATURE
        s +=  " ";
        s +=  PREFIX_TEMPERATURE;
        s +=  ":";
        s += sTemperature.get(n);
      #endif

      #ifdef SET_HUMIDITY
        s +=  " ";
        s += PREFIX_HUMIDITY;
        s +=  ":";
        s += sHumidity.get(n);
      #endif

      #ifdef SET_PRESSURE
        s +=  " ";
        s += PREFIX_PRESSURE;
        s +=  ":";
        s += sPressure.get(n);
      #endif

      #ifdef SET_LIGHT
        s +=  " ";
        s += PREFIX_LIGHT;
        s +=  ":";
        s += sLight.get(n);
      #endif

      #ifdef SET_LIGHT_A
        s +=  " ";
        s += PREFIX_LIGHT_A;
        s +=  ":";
        s += sLightA.get(n);
      #endif

      #ifdef SET_ULTAVIOLET
        s +=  " ";
        s += PREFIX_ULTAVIOLET;
        s +=  ":";
        s += sUltraviolet.get(n);
      #endif

      #ifdef SET_RAIN
        s +=  " ";
        s += PREFIX_RAIN;
        s +=  ":";
        s += sRain.get(n);
      #endif

      #ifdef SET_GROUND
        s +=  " ";
        s += PREFIX_GROUND;
        s +=  ":";
        s += sGround.get(n);
      #endif

      #ifdef SET_GAS

        s +=  " ";
        s += PREFIX_GAS_AQI;
        s +=  ":";
        s += sGasAQI.get(n);

        s +=  " ";
        s += PREFIX_GAS_TVOC;
        s +=  ":";
        s += sGasTVOC.get(n);

        s +=  " ";
        s += PREFIX_GAS_ECO2;
        s +=  ":";
        s += sGasECO2.get(n);

      #endif

      #ifdef SET_TEMPERATURE_B
        s +=  " ";
        s +=  PREFIX_TEMPERATURE_B;
        s +=  ":";
        s += sTemperatureB.get(n);
      #endif

      #ifdef SET_VOLTAGE_0
        s +=  " ";
        s +=  PREFIX_VOLTAGE_0;
        s +=  ":";
        s += sVotage0.get(n);
      #endif

      #ifdef SET_VOLTAGE_1
        s +=  " ";
        s +=  PREFIX_VOLTAGE_1;
        s +=  ":";
        s += sVotage1.get(n);
      #endif

      #ifdef SET_POWER
        s +=  " ";
        s +=  PREFIX_POWER;
        s +=  ":";
        s += sPower.get(n);
      #endif

      #ifdef SET_SOUND
        s +=  " ";
        s +=  PREFIX_SOUND;
        s +=  ":";
        s += sSound.get(n);
      #endif

      #ifdef SET_PORT_RX
        s += " ";
        s += rx.getBufData(n);
      #endif
      
      #ifdef SET_PORT_0
        s += port0.getBufData(n);
      #endif
      
      #ifdef SET_PORT_1
        s += port1.getBufData(n);
      #endif
      
      #ifdef SET_PORT_2
        s += port2.getBufData(n);
      #endif
      
      #ifdef SET_PORT_3
        s += port3.getBufData(n);
      #endif
    

      #ifdef SET_TEMPERATURE_S
        s +=  " ";
        s +=  PREFIX_TEMPERATURE_S;
        s +=  ":";
        s += sTemperatureS.get(n);
      #endif


      return s;

    }




    void init() {

      // Инициализация датчиков 

      #ifdef SET_TEMPERATURE

        #if defined SET_BMP
          bmp.begin();
        #elif defined SET_SHT
          sht.begin(0x44);
          // sht.heater(false);
        #elif defined SET_AHT
          aht.begin();
        #else
          htu.begin();
        #endif

      #endif

      #ifdef SET_PRESSURE

        #ifdef SET_BMP 
          // bmp.begin();
        #else
          bme.begin(0x76);
        #endif

      #endif

      // #ifdef SET_PRESSURE

      //   #ifdef SET_BMP 
      //     bmp.begin();
      //   #else
      //     bme.begin(0x76);
      //   #endif

      // #endif

      // #ifdef SET_PRESSURE

      //   #ifdef SET_BMP 
      //     bmp.begin();
      //   #else
      //     bme.begin(0x76);
      //   #endif

      // #endif

      // #ifdef SET_HUMIDITY
      //   htu.begin();
      // #endif

      #ifdef SET_LIGHT
        lightMeter.begin();
      #endif

      #ifdef SET_POWER
        pinMode(PIN_POWER, INPUT);
      #endif

      #ifdef SET_GAS 
        ENS160.begin();
        ENS160.setPWRMode(ENS160_STANDARD_MODE);
        ENS160.setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);
      #endif

    }


    void run() {

      // Ввод показаний датчиков
      // Интервал ввода - 1 секунда

      static uint32_t timerSensor = 0;
      static bool flag = true;

      #ifdef SET_SOUND

        sound.run();
      
      #endif

      if ( millis() - timerSensor >  INTERVAL_SENSOR_READIGS  || flag ) {

        #ifdef SET_BMP
          bmp.read(1);
        #endif

        #ifdef SET_SHT_HEATER

          static uint32_t timerHeater = 0;
          static bool enableHeater = true;
          
          if ( millis() - timerHeater > 30000 ) {
          
              timerHeater = millis();
              enableHeater = !enableHeater;

              sht.heater(enableHeater);
              pln("Heater Enabled State: ");

          }
          
        #endif

        #ifdef SET_LIGHT
          sLight.read(lightMeter.readLightLevel());
        #endif

        #ifdef SET_LIGHT_A
          sLightA.read(analogRead(PIN_LIGHT_A) / 10.23);
        #endif

        #ifdef SET_TEMPERATURE
          #if defined SET_BMP
            sTemperature.read(bmp.temperature);
          #elif defined SET_SHT
            sTemperature.read(sht.readTemperature());
          #elif defined SET_AHT
            sTemperature.read(aht.getTemperature());
          #else
            sTemperature.read(htu.getTemperatureWait());
            // sTemperature.read(bme.readTemperature());
          #endif
        #endif

        #ifdef SET_TEMPERATURE_B
          if ( mds.readTemp() ) {
            // #ifdef TEMPERATURE_B_DELTA
            //   sTemperatureB.read(mds.getTemp() + TEMPERATURE_B_DELTA);  
            // #else
            //   sTemperatureB.read(mds.getTemp());  
            // #endif
            #ifdef SET_SETTINGS
              String delta = settings.getDelta();
              sTemperatureB.read(mds.getTemp() + delta.toInt());  
            #else
              sTemperatureB.read(mds.getTemp());  
            #endif
          }
          // sTemperatureB.read(mds.getTemp());
          mds.requestTemp();
        #endif

        #ifdef SET_HUMIDITY
          #if defined SET_SHT
            sHumidity.read(sht.readHumidity());
          #elif defined SET_AHT
            sHumidity.read(aht.getHumidity());
          #else
            sHumidity.read(htu.getHumidityWait());
          #endif
          // #ifdef SET_SHT
          //   sHumidity.read(sht.readHumidity());
          // #else
          //   sHumidity.read(htu.getHumidityWait());
          // #endif
        #endif

        #ifdef SET_PRESSURE
          #ifdef SET_BMP
            // if ( bmp.read(1) ) sTemperature.read(bmp.pressure);
            sPressure.read(bmp.pressure);
          #else
            sPressure.read(bme.readPressure() / 133.321995);
          #endif
        #endif

        #ifdef SET_GAS
          
          // Get the air quality index
          // Return value: 1-Excellent, 2-Good, 3-Moderate, 4-Poor, 5-Unhealthy
          sGasAQI.read(ENS160.getAQI()); 
          
          // Get TVOC concentration
          // Return value range: 0–65000, unit: ppb
          sGasTVOC.read(ENS160.getTVOC());
          
          // Get CO2 equivalent concentration calculated according to the detected data of VOCs and hydrogen (eCO2 – Equivalent CO2)
          // Return value range: 400–65000, unit: ppm
          // Five levels: Excellent(400 - 600), Good(600 - 800), Moderate(800 - 1000), 
          //               Poor(1000 - 1500), Unhealthy(> 1500)
          sGasECO2.read(ENS160.getECO2());

        #endif

        #ifdef SET_RAIN
          sRain.read(analogRead(PIN_RAIN) / 10.23);
        #endif

        #ifdef SET_SOUND
          sSound.read(sound.read());
          // sSound.read(analogRead(PIN_SOUND) / 10.23);
        #endif

        #ifdef SET_GROUND 
          sGround.read(analogRead(PIN_GROUND) / 10.23);
        #endif

        #ifdef SET_ULTAVIOLET
          sUltraviolet.read(analogRead(PIN_ULTAVIOLET) / 10.23);
        #endif

        #ifdef SET_VOLTAGE_0 
          sVotage0.read( (float) (analogRead(PIN_VOLTAGE_0) * COEF_VOLTAGE_0) / 1024 );
        #endif

        #ifdef SET_VOLTAGE_1 
          sVotage1.read( (float) (analogRead(PIN_VOLTAGE_1) * COEF_VOLTAGE_1) / 1024 );
        #endif

        #ifdef SET_POWER 
          sPower.read( ( digitalRead(PIN_POWER) ) ? POWER_MAX : 0, true );
        #endif

        #ifdef SET_TEMPERATURE_S
          sTemperatureS.read(sTemperatureS_buf);
        #endif

        timerSensor = millis();
        flag = false;

      }

    }



    #ifdef SET_UNIT

      // Вывод показаний датчиков

      void txPrint() {

        // Now

        static uint32_t timerNow = 0;
        static bool flagNow = true;
        static String s;

        s.reserve(DATA_BUF_COUNT);

        // pln(memoryFree());

        if ( millis() - timerNow > INTERVAL_PRINT_TX_NOW || flagNow ) {

          // String s = creatingDataRow(N_NOW);
          s = creatingDataRow(N_NOW);
          Serial.println(s); 

          #ifdef SET_RADIO_TX

            radio.sendData(s);
    
          #endif

          timerNow = millis();
          flagNow = false;

        }


        // Day

        static uint32_t timerDay = 0;
        static bool flagDay = true;

        if ( millis() - timerDay + 1000 > INTERVAL_SAVE_TO_DB_DAY || flagDay ) {

          // String s = creatingDataRow(N_DAY);
          s = creatingDataRow(N_DAY);
          Serial.println(s); 
                
          #ifdef SET_RADIO_TX

            radio.sendData(s);
    
          #endif

          timerDay = millis();
          flagDay = false;

        }


        // Month 

        static uint32_t timerMonth = 0;
        static bool flagMonth = true;

        if ( millis() - timerMonth + 2000 > 3600000 || flagMonth ) {

          // String s = creatingDataRow(N_MONTH);
          s = creatingDataRow(N_MONTH);
          Serial.println(s); 
                
          #ifdef SET_RADIO_TX

            radio.sendData(s);
    
          #endif

          timerMonth = millis();
          flagMonth = false;

        }


        // Year

        static uint32_t timerYear = 0;
        static bool flagYear = true;

        if ( millis() - timerYear + 3000 > 14400000 || flagYear ) {

          // String s = creatingDataRow(N_YEAR);
          s = creatingDataRow(N_YEAR);
          Serial.println(s); 
                
          #ifdef SET_RADIO_TX

            radio.sendData(s);
    
          #endif

          timerYear = millis();
          flagYear = false;

        }


      }

    #endif


    #ifdef SET_HTTP
  
      void updateBufDataNow(uint8_t n, String data) {

        _bufDataNow[n] = data;

      }
  
    #endif


  private: 

    #ifdef SET_HTTP
  
      String _joinBufDataNow() {

        String s = "";

        for ( int i = 0; i < BUF_NOW_COUNT; i++ ) {
          
          s += _bufDataNow[i];
          if ( _bufDataNow[i] != "" ) s += " ";

        }

        s.trim();

        return s;

      }
      
      String _bufDataNow[BUF_NOW_COUNT];
  
    #endif

};

all_sensors sensors;
