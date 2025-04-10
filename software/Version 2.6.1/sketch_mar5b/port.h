// 

#define TTL_NOW 60000 // 60 * 1000
#define TTL_DAY 1200000 // 60 * 20 * 1000
#define TTL_MONTH 10800000 // 60 * 60 * 3* 1000
#define TTL_YEAR 43200000 // 60 * 60 * 12 * 1000

// #define ARR_BUF_SIZE 24 // 4 * 10
#define ARR_BUF_SIZE 40 // 4 * 10

class Port { 
  public: 

    Port() {

      for (int i = 0; i < ARR_BUF_SIZE; i++) {

        _items[i].clear();
        _timers[i] = 0;

      }

    }


    void read(char* s) {

      char item[strlen(s) + 1]; 
      char prefix[strlen(s) + 1]; 

      for ( int8_t i = 0; i < strlen(s); i++ ) item[i] = s[i + 1];

      if ( int16_t k = _searchDoublePoint(item) ) {

        for ( int8_t i = 0; i <= k; i++ ) prefix[i] = item[i];
        prefix[k + 1] = 0;
          
        for (int i = 0; i < ARR_BUF_SIZE; i++) {

          if ( _items[i].startsWith(prefix) || _items[i].equals("") ) {

            _items[i] = item;
            _timers[i] = millis();

            break;

          };   

        }

      }

    }



    String getBufData(uint8_t n = 0) {

      String s = "";

      mString<DATA_BUF_COUNT_ITEM> item;

      const char mark[] = { 'N', 'D', 'M', 'Y' };
      uint32_t ttl[] = { TTL_NOW, TTL_DAY, TTL_MONTH, TTL_YEAR };

      for (int i = 0; i < ARR_BUF_SIZE; i++) {

        if ( _items[i].buf[0] == mark[n] )
          if ( millis() - _timers[i] < ttl[n] ) {

            s += " ";

            item = _items[i];
            item.remove(0, 1);

            s += item.buf;

          }
        
      }

      return s;

    } 

      
  private: 

    mString<DATA_BUF_COUNT_ITEM> _items[ARR_BUF_SIZE];
    uint32_t _timers[ARR_BUF_SIZE];

      int16_t _searchDoublePoint(char* item) {

        for ( int8_t i = 0; i < strlen(item); i++ ) if ( item[i] == ':' ) return i;
        return -1;

      }

};



