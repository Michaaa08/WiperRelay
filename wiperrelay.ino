    /* 
     Michael Kipp
     Version : 0.5
      
     */  
      
      
    // General Purpose  
    int IntWipe = 0;                       // Flag to show we mode we are in. 0= off, 1= first press, 2= 2nd press.  
    int LedState = LOW;                    // ledState used to set the LED  
    unsigned long startMillis = 1;  //some global variables available anywhere in the program
    unsigned long currentMillis;
    unsigned long interval;
    
    unsigned long WipeInterval = 5000; // Time between wipes as set by start/stop action                            [x secs]  
    unsigned long LastWipeTime =0;          // Time the last wipe was started.  
    unsigned long TimerTime =0;             // used in Timer(), = millis()-LastWipeTime.  
    int ButtonTimeout = 30;                 // Timeout if no button is pressed. Note wipe interval is lower than this.   [30 seconds]  
    int RelayOnTime = 1500;                 // Time the relay is held on to perform a single wipe.                       [1.5 secs]  
    boolean WiperRelayState = LOW;          // Relay Output state  
      
    // Button handling variables  
    unsigned long LastButtonCheck = 0;      // Time the Buttons were last checked.  
    unsigned long ButtonPressTime = 0;      // Time the last button was pressed.  
      
    boolean StartButtonState = LOW;        // records the StartButton State  
    int StartCount = 0;                     // Start Button counter  
    boolean StopButtonState = HIGH;         // records the StopButton State  
    int StopCount = 0;                      // Stop Button counter  
      
      
    //inputs  
    int StartButton = A0;  
    int StopButton = 5;  
    int CalButton = 6;  
    int mode = 0;
      
    //Outputs  
    int WiperRelay = A2;                     // Wiper relay output  
    const int LEDPin = 13;                  // Pin 13  LED Output  
      
    //Settings  
      
    /*  
      Default values for the intermittant wiper 
      Button timeout        = 30 secs 
      ON time for relay     = 1.5 secs 
       
    */  
        
        
    //-------------------------------------------------------------------------  
      
      
    void setup()   
    {  
      Serial.begin(9600);  
      
      //Define the inputs  
      pinMode (StartButton, INPUT);  
      digitalWrite (StartButton, LOW);  
      pinMode (StopButton, INPUT);  
      digitalWrite (StopButton, HIGH);  
        
      //Define the outputs  
      pinMode(LEDPin, OUTPUT);  
      pinMode(WiperRelay, OUTPUT);  
      
      ButtonTimeout = ButtonTimeout *1000;                    // Convert secs to milliseconds  
      //RelayOnTime = RelayOnTime *1000;                      // Convert secs to milliseconds  
    }  
      
      
    void loop()   
    {  
      setIntervall();    
    Timer();//See what is needed to be done 

      
      
    }  
      
    void Timer()  
    {  
      // This controls the various routines and calls them as necessary  
      TimerTime = millis() - LastWipeTime;  
        
      if (((millis()-ButtonPressTime) > ButtonTimeout) && (IntWipe == 1))        // 30 Sec timeout for no button press, but only after stopped, or 1 start.  
      {  
        IntWipe = 0;  
        LastWipeTime = 0;  
        WipeInterval = 0;     
      }  
          // The SingleWipe() will be called when the start button is pressed, we need to turn the relay off.  
      if ((TimerTime > RelayOnTime) && WiperRelayState == true)    // RelayOnTime has been exceeded AND WipeRelay is HIGH  
      {  
        SingleWipe();                                          // It will turn off the relay and not modify any variables  
      }  
           
      // Time to wipe but only if there is a WipeInterval AND the relay is off.        
      if ((TimerTime > WipeInterval) && (mode == 5 || mode== 1)  && WiperRelayState == false && StartButtonState == HIGH)         
      {  
        SingleWipe();  
      }  

      if((TimerTime > 30000) && (mode == 5 || mode == 3) ) {
        WipeInterval = 30000;
        }

//     else if((TimerTime> FirstWipeInterval)  && (WiperRelayState == false) && StartButtonState == HIGH){
//        SingleWipe();
//        }
        
    }  
      
   

void setIntervall()  
    {  
      
      StartButtonState = digitalRead(StartButton);
      currentMillis = millis();
     
      switch(mode){
        //Wenn Schalter auf HIGH ist, dann Intervall mit 5 Sekunden und auf Modus 1 umgeschaltet
       case 0:
       if(StartButtonState == HIGH && startMillis ==1){
        startMillis++;
        mode = 1;
        break;
        }
        else{break;
          }
        
        //Wenn Schalter auf LOW ist, wird nichts ausgeführt und auf Modus 2 umgeschaltet
       case 1: 
        if(StartButtonState == LOW && startMillis ==2){
          startMillis++;
          mode=2;
          break;
          }
         break;


       //Wenn Schalter wieder auf HIGH ist, dann wird dort angefangen das Intervall zu messen und auf Modus 3 umgeschaltet
       case 2: 
        if(StartButtonState == HIGH && startMillis == 3){
            startMillis = currentMillis;
            WipeInterval = (currentMillis - LastWipeTime); 
            SingleWipe();
            mode = 3;
            break;
             }
             break;
             
            //Wenn Schalter  auf LOW ist, dann wird dort aufgehört das Intervall zu messen und auf Modus 4 umgeschaltet
        case 3:
        if(StartButtonState == LOW && startMillis > 0){
          interval = currentMillis - startMillis;
          WipeInterval= (currentMillis - LastWipeTime);
          SingleWipe();
            mode =4;
            break;
        }
        else {
          break;
          }
          //Wenn Start Button auf High geschaltet wird läuft das Intervall mit dem Scheibenwischer an und wechselt auf Modus 5
            case 4:  
                  if(StartButtonState == HIGH){
                      mode=5;
                    }
                  
                   
                break;  

            //Wenn Schalter auf LOW geschaltet wird, geht der Scheibenwischer asu und wechselt auf Modus 6
            case 5:
            if(StartButtonState == LOW){
                 mode=6;
                  break;
        }
        break;
        // Neues Intervall wird gemessen und wechselt auf Modus 7
      case 6:
      interval = currentMillis - startMillis;
      WipeInterval= (currentMillis - LastWipeTime);
       SingleWipe();
          mode =7;
           break;

  // Messung von Intervall wird beendet und schaltet auf Modus 4 wieder zurück, sodass das Intervall normal durchläuft mit Scheibenwischer an und ab dort eine Schleife entsteht
    case 7:
      if(StartButtonState == HIGH){
         WipeInterval = (currentMillis - LastWipeTime); 
           SingleWipe();
                    mode=4;
                      break;
                  }
                    break;
       
//            }
      
    }
    Serial.println(mode);
    Serial.println(TimerTime);
    }
  
    
      
    void SingleWipe()  
    {  
      if (WiperRelayState == false)                          // Not much point in wiping if we already doing a wipe.  
      {  
        LastWipeTime = millis();  
        WiperRelayState = true;  
        digitalWrite(WiperRelay, HIGH);  
        digitalWrite(LEDPin, HIGH);  
      }  
      else  
       {  
        WiperRelayState = false;  
        digitalWrite(WiperRelay, LOW);  
        digitalWrite(LEDPin, LOW);  
       }  
       return;  
    }  
