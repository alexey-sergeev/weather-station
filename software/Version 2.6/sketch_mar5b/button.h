// Класс 

void (*callback)(); 


class Button { 
  public: 

    void init() {
     
      pinMode(PIN_BUTTON, INPUT_PULLUP);
      attachInterrupt(PIN_BUTTON, isr, CHANGE);

    }



    IRAM_ATTR static void isr() {
    
      static uint32_t timerButton;
      
      if ( millis() - timerButton > 200 && digitalRead(PIN_BUTTON) ) {
    
        timerButton = millis();
        (*callback)();

      }
    
    }




    void attachCallback(void (*function)()) { 

      callback = *function;

    }



  private:   

};

Button button;
