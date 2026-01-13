// 


class sound_wrapper { 
  public: 

    void init() {

    }


    void run() {

      uint16_t a = analogRead(PIN_SOUND);
      if ( a > _a ) _a = a;

    }


    uint16_t read() {

      uint16_t a = _a / 10.23;
      _a = 0;

      return a;

    }


  private: 

    uint16_t _a;

};


sound_wrapper sound;

