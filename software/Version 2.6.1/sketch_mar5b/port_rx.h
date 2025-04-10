// 

#define TTL_NOW 60000 // 60 * 1000
#define TTL_DAY 1200000 // 60 * 20 * 1000
#define TTL_MONTH 10800000 // 60 * 60 * 3* 1000
#define TTL_YEAR 43200000 // 60 * 60 * 12 * 1000
// #define TTL_NOW 60000 // 60 * 1000
// #define TTL_DAY 600000 // 60 * 10 * 1000
// #define TTL_MONTH 10800000 // 60 * 60 * 3* 1000
// #define TTL_YEAR 43200000 // 60 * 60 * 12 * 1000

// #define DATA_BUF_COUNT 128

class PortRX { 
  public: 

    PortRX() {

      _bufDataNow.clear();
      _bufDataDay.clear();
      _bufDataMonth.clear();
      _bufDataYear.clear();

    }


    void read() {

      if ( Serial.available() > 0)  {
 
        mString<DATA_BUF_COUNT> s;
        s = Serial.readStringUntil('\n');

        char prefix[] = "$Now:";
        for ( int i = 0; i < 5; i++ ) prefix[i] = s.buf[i];

        s.remove(0, 6);

        _dataSave(prefix, s.buf);

      } 

    }


    String getBufData(uint8_t n = 0) {

      mString<DATA_BUF_COUNT> data;
      data.clear();

      if ( n == N_NOW && millis() - _timerNow < TTL_NOW ) data = _bufDataNow.buf;
      if ( n == N_DAY && millis() - _timerDay < TTL_DAY ) data = _bufDataDay.buf;
      if ( n == N_MONTH && millis() - _timerMonth < TTL_MONTH) data = _bufDataMonth.buf;
      if ( n == N_YEAR && millis() - _timerYear < TTL_YEAR ) data = _bufDataYear.buf;
 
      return data.buf;

    } 



  private: 

    mString<DATA_BUF_COUNT> _bufDataNow;
    mString<DATA_BUF_COUNT> _bufDataDay;
    mString<DATA_BUF_COUNT> _bufDataMonth;
    mString<DATA_BUF_COUNT> _bufDataYear;

    uint32_t _timerNow = 0;
    uint32_t _timerDay = 0;
    uint32_t _timerMonth = 0;
    uint32_t _timerYear = 0;


    void _dataSave(char* prefix, char* s) {

      if ( (String) prefix == PREFIX_NOW ) {
      
        _bufDataNow = s;
        _timerNow = millis();
      
      }

      if ( (String) prefix == PREFIX_DAY ) {
        
        _bufDataDay = s;
        _timerDay = millis();

      }
      
      if ( (String) prefix == PREFIX_MONTH ) {
        
        _bufDataMonth = s;
        _timerMonth = millis();

      }

      if ( (String) prefix == PREFIX_YEAR ) {
        
        _bufDataYear = s;
        _timerYear = millis();

      }
 
    }

};



