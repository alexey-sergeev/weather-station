// Класс датчиков


class Sensor { 
  public: 

    // Читать показания датчика
    
    void read(float a, bool f = false) {

      if ( f ) _flag = true;

      if ( _flag ) {

        for (uint8_t i = 0; i < SAMPLE_SIZE; i++) _buf[i] = a;
        for (uint8_t i = 0; i < SAMPLE_SIZE_2; i++) _buf2[i] = a;
        for (uint8_t i = 0; i < SAMPLE_SIZE_3; i++) _buf3[i] = a;
        _flag = false;

      }

      _a = a; 

      _buf[_count] = a;
      if (++_count >= SAMPLE_SIZE) _count = 0; 

    } 



    // Получить показания датчика

    float get(uint8_t n = N_NOW) {

      // Сейчас
      
      float a = 0;
      for (uint8_t i = 0; i < SAMPLE_SIZE; i++) a += _buf[i];
      a = a / SAMPLE_SIZE;

      if ( n == N_NOW ) return a;

      // Для дневной базы данных

      _buf2[_count2] = a;
      if (++_count2 >= SAMPLE_SIZE_2) _count2 = 0; 

      if ( n == N_DAY ) return a;

      // Для ежемесячной базы данных

      if ( n == N_MONTH ) {

        float a2 = 0;
        for (uint8_t i = 0; i < SAMPLE_SIZE_2; i++) a2 += _buf2[i];
        a2 = a2 / SAMPLE_SIZE_2;

        _buf3[_count3] = a2;
        if (++_count3 >= SAMPLE_SIZE_3) _count3 = 0; 

        return a2;

      }

      // Для годовой базы данных
      
      if ( n == N_YEAR ) {
        
        float a3 = 0;
        for (uint8_t i = 0; i < SAMPLE_SIZE_3; i++) a3 += _buf3[i];
        a3 = a3 / SAMPLE_SIZE_3;
       
        return a3;
      
      }

      return 0;

    }



    // Получить "сырые" показания датчика

    float getRaw() {

      return _a;

    }

 
 
  private: 

    float _a;
    bool _flag = true;
    
    float _buf[SAMPLE_SIZE]; 
    uint8_t _count = 0;
    
    float _buf2[SAMPLE_SIZE_2]; 
    uint8_t _count2 = 0;
    
    float _buf3[SAMPLE_SIZE_3]; 
    uint8_t _count3 = 0;

};