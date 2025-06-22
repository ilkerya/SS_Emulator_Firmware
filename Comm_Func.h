
#ifdef STM32_F407 // 10msec

  #define TIME_10MSEC 1
  #define TIME_20MSEC 2 
  #define TIME_50MSEC 5
  #define TIME_100MSEC 10
  #define TIME_250MSEC 25
  #define TIME_500MSEC 50
  #define TIME_1_SEC 100
  #define TIME_2_SEC 200
  #define TIME_5_SEC 500
  #define TIME_10_SEC 1000
  #define TIME_20_SEC 2000
  #define TIME_60_SEC 6000
#endif


void PSR_StateFind(void){
  switch(PSR.State){
    case PHASE_OFF:
       if(!PSR.Oled_State)PSR.OledCounter = 0;
        PSR.OledCounter++;
        if(PSR.OledCounter > 7) PSR.State = IDLE; // button Start
    break;
    case IDLE:
       if(!PSR.Oled_State) PSR.State = PHASE_OFF;
       if(UI_Command.StartStop && (PSR.Oled_State == ON)) PSR.State = RAMPING_UP;// button Stop
    break;    
    case RAMPING_UP:
        if(!PSR.Oled_State) PSR.State = PHASE_OFF;   // button Stop
        if(!UI_Command.StartStop)PSR.State = IDLE;
        if(!PSR.Rled_State)PSR.RledCounter = 0;
        PSR.RledCounter++;
        if(PSR.RledCounter > 7) PSR.State = TOPOF_RAMP;// button Stop
             
    break;
    case TOPOF_RAMP:
      if(!PSR.Oled_State) PSR.State = PHASE_OFF;   // button Stop
      if(!UI_Command.StartStop)PSR.State = IDLE;
    break;    
    default: PSR.State = IDLE;
    break; 
  }
}


// interrupt vector

  void STM32_IntVectorT1(){
    //  digitalWrite(TIME_DEBUG_OUT_PIN, !digitalRead(TIME_DEBUG_OUT_PIN));
     ///   digitalWrite(TIME_DEBUG_OUT_PIN, OFF); // active time check in interrupt


    Loop.Task_10msec = ON; 
    Loop.IntTimer_20m ++;       
    Loop.IntTimer_50m ++;   
    Loop.IntTimer_100m ++;  
    Loop.IntTimer_250m ++;
    Loop.IntTimer_500m ++;   
    Loop.IntTimer_1 ++;
    Loop.IntTimer_2 ++;
    Loop.IntTimer_5 ++;
  

    if(Loop.IntTimer_20m >= TIME_20MSEC){
      Loop.IntTimer_20m = 0;
      Loop.Task_20msec = ON;    
          #if  defined KEY_DIGITAL
      Key_Functions_Digital();
          #endif
          #if  defined KEY_ANALOG    
          // pinMode(KEY_ANALOG_IN, INPUT_PULLUP); 
          // INPUT_PULLUP 
         // Key.Adc  = analogRead(KEY_ANALOG_IN); 
          Key_Functions_Analog();   
          #endif 
               digitalWrite(TIME_DEBUG_OUT_PIN, !digitalRead(TIME_DEBUG_OUT_PIN));
          // DAQ_Send_Data();
    }
    if(Loop.IntTimer_50m >= TIME_50MSEC){
      Loop.IntTimer_50m = 0;
      Loop.Task_50msec = ON;     

    
    }
    if(Loop.IntTimer_100m >= TIME_100MSEC){
      Loop.IntTimer_100m = 0;
      Loop.Task_100msec = ON;

    }
    if(Loop.IntTimer_250m >= TIME_250MSEC){
      Loop.IntTimer_250m = 0;
      Loop.Task_250msec = ON;
         
   
    }
    if(Loop.IntTimer_500m >= TIME_500MSEC){ // 500 msec
      Loop.IntTimer_500m = 0;
      Loop.Task_500msec = ON;
      
    }
    if(Loop.IntTimer_1 >= TIME_1_SEC){  // 1 sec
      Loop.IntTimer_1 = 0;
      Loop.Task_1Sec = ON;

   
    #ifdef STM32_F407
      digitalWrite(LED_RED, !digitalRead(LED_RED));

    #endif
  
              
    }
       
    if(Loop.IntTimer_2 >= TIME_2_SEC){ // 2 sec
      Loop.IntTimer_2 = 0;
      Loop.Task_2Sec = ON;
    }

}
// https://www.onlinegdb.com/edit/Hkmlxi_08

void Common_Loop(){
  if (Loop.Task_10msec) {
    Loop.Task_10msec = OFF;
  }
  if (Loop.Task_20msec) {
    Loop.Task_20msec = OFF;
      
     
  }
  if (Loop.Task_50msec) {
    Loop.Task_50msec = OFF;

       SerialPortRx_UI();  

  }
  if (Loop.Task_100msec) {
    Loop.Task_100msec = OFF;

      

    PSR_StateFind();

  }
  if (Loop.Task_250msec) {
    Loop.Task_250msec = OFF;

    uint32_t temp; 

    temp = (uint32_t)analogRead(ADC_RAMPUP);
    temp *= 500;
    temp /= 330; 
   // RampUp = (uint16_t)temp>>4; // min 16 max 252
    RampUp.Adc = (uint16_t)temp>>2; // min 70 max 969


    if(RampUp.Adc > RAMPUP_MAX)RampUp.Val = 200;
    else if(RampUp.Adc <= RAMPUP_MIN)RampUp.Val = 10; 
    else  {
        RampUp.Val = RampUp.Adc / 5; // 20-1 -2 = 17 -> (MAX - MIN)/17 = 53
        RampUp.Val +=10;
    }

    temp =(uint32_t)analogRead(ADC_STOP);
    temp *= 500;
    temp /= 330; 
    StopRamp.Adc =(uint16_t)temp>>2; // min 17 max 250
   
    if(StopRamp.Adc > RAMPUP_MAX)StopRamp.Val = 200;
    else if(StopRamp.Adc <= RAMPUP_MIN)StopRamp.Val  = 0; 
    else  {
        StopRamp.Val  = StopRamp.Adc / 5;  // 20-0 -2 = 18 -> (MAX - MIN)/18 = 54
    }

    temp = (uint32_t)analogRead(ADC_VOLTPERCENT);
    temp *= 500;
    temp /= 330; 
    VoltPercent.Adc = (uint16_t)temp>>2; // min 15 max 237

   if(VoltPercent.Adc > VOLTPERCENT_MAX)VoltPercent.Val = 70;
    else if(VoltPercent.Adc <= VOLTPERCENT_MIN)VoltPercent.Val = 40; 
    else  {
        VoltPercent.Val = VoltPercent.Adc / 32;  // 70-40 -2 = 28 -> (MAX - MIN)/28 = 32
        VoltPercent.Val +=40;
    }

	      #ifdef OLEDDISPLAY_EXISTS
        // One time after wake up form sleep
        if (Display.OLED_Init == ON) {
          Display_ReInit_Start(20);
          Display.OLED_Init = OFF;
        }
        if (Display.OLED_Timer) {
         DisplayScreen();
        }
        else {
          Display_SwitchOff();
        }
        Display_ReInit_End();
        #endif

  }
  if (Loop.Task_500msec) {
    Loop.Task_500msec = OFF;
      
  }
  if (Loop.Task_1Sec) {
    Loop.Task_1Sec = OFF;
    #ifndef DEBUG_SIMULATOR_MODE

/*
          Serial.print("R.VoltPin "); Serial.print(digitalRead(R_Phase.IO_Voltage));   
          Serial.print(" S.VoltPin "); Serial.print(digitalRead(S_Phase.IO_Voltage));   
          Serial.print(" TR1 "); Serial.print(digitalRead(R_Thy_FireIO));   
          Serial.print("  TR2 "); Serial.print(digitalRead(S_Thy_FireIO));   
          Serial.print("  Count1 : "); Serial.print(R_Phase.Thyristor_Firing_Angle);  
          Serial.print("  Count2 : "); Serial.println(S_Phase.Thyristor_Firing_Angle);  
*/
  #endif // end of  #ifndef DEBUG_SIMULATOR_MODE
  //  KeyTimeOutCheck();

      if(Display.SleepEnable == ON){
        if(Display.OLED_Timer) Display.OLED_Timer--;   // sleep active
      }
      else Display.OLED_Timer = 32768; // no sleep    
      if(Display.InitDelay == OFF)Display.InitDelay = ON;   
      
          if((MainMenu == MENU5_SUB7)||(MainMenu == MENU2_SUB14)||(MainMenu == MENU3_SUB3) ||
                                  (MainMenu == MENU3_SUB4)||(MainMenu == MENU1_SUB3)||
                                  (MainMenu == MENU1_SUB4)||(MainMenu == MENU6_SUB3)|| 
                                  (MainMenu == MENU7_SUB9)||(MainMenu == MENU8_SUB3)){
          Display.MenuTimeout++;
          if(Display.MenuTimeout > 4){
            Display.MenuTimeout = 0;
            MainMenu = MENU_NULL;
         }
      }
      if(MainMenu == MENU6_SUB7) {  //  Mains Updated !   
        Display.MenuTimeout++;
        if(Display.MenuTimeout > 2){
          Display.MenuTimeout = 2;
          MainMenu = MENU6_SUB3;  // Calibration Started !

        }
      }     
  }
  if (Loop.Task_2Sec) {
    Loop.Task_2Sec = OFF;

       // PrintDisplayBuffer();
        #ifdef DISPLAY_ROLL
        UpdateDispRoll(DOWNROLL); 
        #endif
  }
  if (Loop.Task_5Sec) {
    Loop.Task_5Sec = OFF;

  }
} // end of comon loop 

void IO_Settings() {

  #define LED_BUILTIN       PB0               // Pin 33/PB0 control on-board LED_GREEN on F767ZI
 // #define LED_BLUE          PB7               // Pin 73/PB7 control on-board LED_BLUE on F767ZI
  #define LED_RED           PB14              // Pin 74/PB14 control on-board LED_BLUE on F767ZI
  
  pinMode(LED_BUILTIN,  OUTPUT);
  //pinMode(LED_BLUE,     OUTPUT);
  pinMode(LED_RED,      OUTPUT);
/*
  pinMode(DEBUG_OUT_1, OUTPUT); 
  pinMode(DEBUG_OUT_2, OUTPUT); 
  pinMode(DEBUG_OUT_3, OUTPUT);   

  digitalWrite(DEBUG_OUT_2,LOW);
  */

  pinMode(R_VOLTAGE_CREATE_PIN ,      OUTPUT);
  pinMode(S_VOLTAGE_CREATE_PIN ,      OUTPUT);
  pinMode(T_VOLTAGE_CREATE_PIN ,      OUTPUT);
  pinMode(PSR_START,      OUTPUT);  

  digitalWrite(R_VOLTAGE_CREATE_PIN ,LOW);
  digitalWrite(S_VOLTAGE_CREATE_PIN ,LOW);
  digitalWrite(T_VOLTAGE_CREATE_PIN ,LOW);
  digitalWrite(PSR_START,HIGH); 

  pinMode(R_PSR_Thy_FireDet_IO, INPUT ); // INPUT_PULLUP
  pinMode(S_PSR_Thy_FireDet_IO, INPUT);
  pinMode(T_PSR_Thy_FireDet_IO, INPUT ); // INPUT_PULLUP
 // pinMode(T_Thy_FireIO, INPUT ); // INPUT_PULLUP
 
  pinMode(R_PSR_CURRENT_CREATE_PIN ,      OUTPUT);
  pinMode(S_PSR_CURRENT_CREATE_PIN ,      OUTPUT);
  pinMode(T_PSR_CURRENT_CREATE_PIN ,      OUTPUT);

  digitalWrite(R_PSR_CURRENT_CREATE_PIN ,LOW);
  digitalWrite(S_PSR_CURRENT_CREATE_PIN ,LOW);
  digitalWrite(T_PSR_CURRENT_CREATE_PIN ,LOW);


  pinMode(R_PSTX_VOLTAGE_CREATE_PIN ,      OUTPUT);
  pinMode(R_PSTX_DEBUG_THY_FIRING_CREATE_PIN ,      OUTPUT); 
  pinMode(R_PSTX_CURRENT_CREATE_PIN ,      OUTPUT); 
  pinMode(R_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN ,OUTPUT);

  pinMode(S_PSTX_VOLTAGE_CREATE_PIN ,      OUTPUT); 
  pinMode(S_PSTX_DEBUG_THY_FIRING_CREATE_PIN ,      OUTPUT); 
  pinMode(S_PSTX_CURRENT_CREATE_PIN ,      OUTPUT); 
  pinMode(S_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN ,OUTPUT);

  pinMode(T_PSTX_VOLTAGE_CREATE_PIN ,      OUTPUT); 
  pinMode(T_PSTX_DEBUG_THY_FIRING_CREATE_PIN ,      OUTPUT); 
  pinMode(T_PSTX_CURRENT_CREATE_PIN ,      OUTPUT); 
  pinMode(T_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN ,OUTPUT);

  pinMode(TIME_DEBUG_OUT_PIN ,      OUTPUT); 


  pinMode(RLED, INPUT_PULLUP ); // INPUT_PULLUP
  pinMode(OLED, INPUT_PULLUP ); // INPUT_PULLUP

  pinMode(ADC_STOP, INPUT);
  pinMode(ADC_VOLTPERCENT, INPUT);
  pinMode(ADC_RAMPUP, INPUT);


  PSR.State = PHASE_OFF;


  #ifdef ANALOG_RES_12BIT
    analogReadResolution(12);
  #endif 
  #ifdef ANALOG_RES_10BIT
    analogReadResolution(10);
  #endif 

 pinMode(KEY_ANALOG_IN, INPUT_PULLUP); 
 //   pinMode(KEY_ANALOG_IN, INPUT_PULLDOWN); 

}


void MicroInit() {

 

    
}

//------------------------------------------------------------------------------
void clearSerialInput() {
  uint32_t m = micros();
  do {
    if (Serial.read() >= 0) {
      m = micros();
    }
  } while (micros() - m < 10000);
}
