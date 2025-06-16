
void KeyTimeOutCheck(void) {
  if (Key.BoardTimeOutEnb == ON) {
    if (Key.BoardTimeOut)Key.BoardTimeOut--;
    else {
      Key.BoardTimeOutEnb = OFF;
      MainMenu =  MENU_NULL;
    }
  }
}

void KeyTimeOutStart(void) {
  Key.BoardTimeOutEnb = ON;
  Key.BoardTimeOut  = 24;// 24 sec
}

void DisplayWakeUp(void) {
  if ((Display.OLED_Timer == 0) && (Display.OLED_Init == OFF)) {
    Display.OLED_Init = ON;
  }//else if(OLED_Timer)OLED_Timer += 10; // add additional time every time any key released
}

void  DispExtTimeout(void) {
  if (Display.OLED_Timer <= KEYDISP_TIMER) Display.OLED_Timer = KEYDISP_TIMER;
}
void UpdateInfoQue(uint8_t UpDown){
    #define MAXLINE 9
    #define MINLINE 1
   if (UpDown == DOWNROLL){    // down menu
     DispRollIndex[3] = DispRollIndex[2];
     DispRollIndex[2] = DispRollIndex[1];
     DispRollIndex[1] = DispRollIndex[0];
     DispRollIndex[0]++;
     if (DispRollIndex[0]  > MAXLINE) DispRollIndex[0] = MINLINE;
  } 
  if (UpDown == UPROLL){   
     DispRollIndex[0] = DispRollIndex[1];
     DispRollIndex[1] = DispRollIndex[2];
     DispRollIndex[2] = DispRollIndex[3];
     DispRollIndex[3]--;  
     if (DispRollIndex[3]  < MINLINE) DispRollIndex[3] = MAXLINE; 
   } 
}
void UpdateDispRoll(uint8_t UpDown){
    if(Display.SensorRollTimer){
      Display.SensorRollTimer--;
      return;
    }
    UpdateInfoQue(UpDown);
}
void KeySensonsorRoll(uint8_t UpDown){
      Display.SensorRollTimer = 30; // 2sec x 30 = 60 sec
      UpdateInfoQue(UpDown);      
}
void DispEnable(bool Enable, uint8_t Timer) {
  if (Enable == ON) {
    Display.SleepEnable = ON; //go sleep
    Display.OLED_Timer = Timer;
  }
  else   Display.SleepEnable = OFF;    // no sleep
}
void DispEnable_4SD_Prblm(bool Enable, uint8_t Timer) {
  if (Enable == ON) {
    //Display.SleepEnable = ON; //go sleep
    Display.OLED_Timer = Timer;
  }
  //else   Display.SleepEnable = OFF;    // no sleep
}







/*
void UpdateLogFileNo(void){
  uint16_t No = (File_Que[0]-48)*10;
  No += (File_Que[1]-48);

  Serial.print(F("    No: "));
  Serial.println(No);
  if(No >= 100){
      File_Que[0] =  '0';
      File_Que[1] =  '1';  
      NVRam_Write_QueNo(&File_Que[0]);
  }
}

void UpdateLogFileId(void){
    LOG_FILE[1] = Device_Id[0];
    LOG_FILE[2] = Device_Id[1];
    LOG_FILE[3] = Device_Id[2];
    LOG_FILE[4] = Device_Id[3];
    
    LOG_FILE[6] =  File_Que[0];   
    LOG_FILE[7] =  File_Que[1];  
    
    Serial.print(F("LOG_FILE: ")) ;
    Serial.println(LOG_FILE);
}
*/
void ResetCasePrint() {
#ifdef ARDUINO_MEGA // 8 bit AVR 
    
  uint8_t RESET_CASE = MCUSR;
  Serial.print(F("MCUSR: "));
  Serial.println(RESET_CASE);
  if (MCUSR & WDRF) {
    Serial.print(F("Rebooting from a Watchdog Reset"));
  }
  else if (MCUSR & BORF) {
    Serial.print(F("Rebooting from a Brown-out Reset"));
  }
  else if (MCUSR & EXTRF) {
    Serial.print(F("Rebooting from an External Reset"));
  }
  else if (MCUSR & PORF) {
    Serial.print(F("Rebooting from a Power On Reset"));
  }
  if (MCUSR & (1 << WDRF)) {
    // a watchdog reset occurred
    Serial.print(F("Rebooting from a Watchdog Reset"));
  }
  if (MCUSR & (1 << BORF)) {
    Serial.print(F("Rebooting from a Brown-out Reset"));
  }
  //  a brownout reset occurred
  if (MCUSR & (1 << EXTRF)) {
    //  an external reset occurred
    Serial.print(F("Rebooting from an External Reset"));
  }
  if (MCUSR & (1 << PORF)) {
    //  a power on reset occurred
    Serial.print(F("Rebooting from a Power On Reset"));
  }
  //Clear register
  MCUSR = 0x00;

    #endif
}
//uint8_t KeyCounterFast = 0;
/*
bool FastLog_Terminate(void){
      if(Key.Up_Press || (Key.Down_Press))KeyCounterFast++;
      if((Key.Left_Press) || (Key.Right_Press)) KeyCounterFast = 0;
 
      if(KeyCounterFast > 4){   
        KeyCounterFast = 0;    
     
        SDCard.LogEnable = OFF;
        NVRam_Write(EE_LOGSTATUS,SDCard.LogEnable);    
        MainMenu =  MENU1_SUB4;//MENU1
        
        return OFF;
      }
      else return ON;

}
*/

#ifdef KEY_ANALOG
void Key_Functions_Analog() {


  Key.Adc  = analogRead(KEY_ANALOG_IN);
  // Key.Adc = Adc;
 
#ifdef ARDUINO_DUE // 
      Key.Adc >>= 2; 
    //  if(Key.Adc < 990) {
        Key.Adc *= 66;
        Key.Adc /= 100;        
   //   }
#endif

#if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE) 

    #define KEY_DOWN_ADC_HIGH 588
    #define KEY_DOWN_ADC_LOW 564   

    #define KEY_LEFT_ADC_HIGH 478
    #define KEY_LEFT_ADC_LOW 440

    #define KEY_RELEASED_ADC_HIGH 404
    #define KEY_RELEASED_ADC_LOW 384   

    #define KEY_UP_ADC_HIGH 358
    #define KEY_UP_ADC_LOW 334   

    #define KEY_RIGHT_ADC_HIGH 110
    #define KEY_RIGHT_ADC_LOW 84   

#endif

#if defined (STM32_F407)

  #ifdef ANALOG_RES_10BIT
    #define KEY_DOWN_ADC_HIGH 767 //757
    #define KEY_DOWN_ADC_LOW 747   
    #define KEY_LEFT_ADC_HIGH 612  //602
    #define KEY_LEFT_ADC_LOW 592
    #define KEY_RELEASED_ADC_HIGH 521 //511
    #define KEY_RELEASED_ADC_LOW 501   
    #define KEY_UP_ADC_HIGH 465 //455
    #define KEY_UP_ADC_LOW 445   
    #define KEY_RIGHT_ADC_HIGH 155 //145
    #define KEY_RIGHT_ADC_LOW 135   
  #endif 
/*
  #ifdef ANALOG_RES_12BIT
    #define KEY_DOWN_ADC_HIGH 3068 //757
    #define KEY_DOWN_ADC_LOW  2900   
    
    #define KEY_LEFT_ADC_HIGH 2488  //2080 
    #define KEY_LEFT_ADC_LOW 2348
    
    #define KEY_RELEASED_ADC_HIGH 2100 //2080  2407 1.94V
    #define KEY_RELEASED_ADC_LOW 2024   
    
    #define KEY_UP_ADC_HIGH 1900 //455
    #define KEY_UP_ADC_LOW 1790   
    
    #define KEY_RIGHT_ADC_HIGH 650 //145
    #define KEY_RIGHT_ADC_LOW 540   
  #endif 
  */
  #ifdef ANALOG_RES_12BIT
    #define KEY_DOWN_ADC_HIGH 2343 //757 S2
    #define KEY_DOWN_ADC_LOW  2263   
    
    #define KEY_LEFT_ADC_HIGH 1864  //2080  S1
    #define KEY_LEFT_ADC_LOW 1784
    
    #define KEY_RELEASED_ADC_HIGH 1602 //2080  2407 1.94V
    #define KEY_RELEASED_ADC_LOW 1522   
    
    #define KEY_UP_ADC_HIGH 1420 //455 S3
    #define KEY_UP_ADC_LOW 1340   
    
    #define KEY_RIGHT_ADC_HIGH 450 //145 S4
    #define KEY_RIGHT_ADC_LOW 350   
  #endif 
    
#endif      
    Key.Down_Press = OFF;
    Key.Right_Press = OFF;
    Key.Up_Press = OFF;
    Key.Left_Press = OFF;
    if((Key.Adc < KEY_RELEASED_ADC_HIGH) &&(Key.Adc > KEY_RELEASED_ADC_LOW)){
      Key.Released = ON;
      Key.Error = OFF;
      return;  
    }
    else if((Key.Adc < KEY_RIGHT_ADC_HIGH) && (Key.Adc > KEY_RIGHT_ADC_LOW))Key.Right_Press = ON;
    else if((Key.Adc < KEY_UP_ADC_HIGH) && (Key.Adc > KEY_UP_ADC_LOW))Key.Up_Press = ON;
    else if((Key.Adc < KEY_DOWN_ADC_HIGH) && (Key.Adc > KEY_DOWN_ADC_LOW))Key.Down_Press = ON;
    else if((Key.Adc < KEY_LEFT_ADC_HIGH) && (Key.Adc > KEY_LEFT_ADC_LOW))Key.Left_Press = ON;   
    else  {
      Key.Error = ON;
      return;
    }
    Key.Error = OFF;
    
  if (!Key.Released)return;
  Key.Released = OFF;
  DisplayWakeUp();

 // if(!FastLog_Terminate()){
  /* //TEST
   if (Key.Left_Press) Serial.println(F("EnterMenuKey"));
   if (Key.Down_Press) Serial.println(F("DownMenuKey"));
   if (Key.Right_Press) Serial.println(F("EscMenuKey"));
   if (Key.Up_Press) Serial.println(F("UpMenuKey"));          
    */
    if (Key.Left_Press) EnterMenuKey();
    if (Key.Down_Press) DownMenuKey();
    if (Key.Right_Press) EscMenuKey();
    if (Key.Up_Press)   UpMenuKey();
 // }  
    KeyTimeOutStart();
}
#endif

#ifdef KEY_DIGITAL
void Key_Functions_Digital(void) {

  if (!Key.Left_Rel && !Key.Down_Rel && !Key.Right_Rel&& !Key.Up_Rel) {
    Key.Released = ON;
  }
  if (Key.Released && !digitalRead(KEY_LEFT)) {
    Key.Left_Rel = ON;
    return;
  }
  if (Key.Left_Rel  &&  digitalRead(KEY_LEFT)) {
    Key.Left_Rel = OFF;
    DisplayWakeUp();
    EnterMenuKey();
    KeyTimeOutStart();
    return;
  }
  if (Key.Released && !digitalRead(KEY_DOWN)) {
    Key.Down_Rel = ON;
    return;
  }
  if (Key.Down_Rel  &&  digitalRead(KEY_DOWN)) {
    Key.Down_Rel = OFF;
    DisplayWakeUp();
    DownMenuKey(); 
    KeyTimeOutStart();
    return;
  }
  if (Key.Released && !digitalRead(KEY_UP)) {
    Key.Up_Rel = ON;
    return;
  }
  if (Key.Up_Rel  &&  digitalRead(KEY_UP)) {
    Key.Up_Rel = OFF;
    DisplayWakeUp();
    UpMenuKey();   
    KeyTimeOutStart();
    return;
  }
  
  if (Key.Released && !digitalRead(KEY_RIGHT)) {
    Key.Right_Rel = ON;
    return;
  }
  if (Key.Right_Rel  &&  digitalRead(KEY_RIGHT)) {
    DisplayWakeUp();
    EscMenuKey();
    KeyTimeOutStart();
    return;
  }
}
#endif
void DownMenuKey(void) {
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL :// Menu = MENU_NULL;
            KeySensonsorRoll(DOWNROLL);
      break;
    case MENU1 : MainMenu = MENU8; //
      break;
    case MENU2 : MainMenu = MENU1;
      break;
    case MENU3 : MainMenu = MENU2;
      break;
    case MENU4 : MainMenu = MENU3;
      break;
     case MENU5 : MainMenu = MENU4;
      break;  
     case MENU6 : MainMenu = MENU5;
      break;  
     case MENU7 : MainMenu = MENU6;
      break;    
     case MENU8 : MainMenu = MENU7;
       break; 
                
    case MENU1_SUB1 :  MainMenu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;    
    case MENU2_SUB01 :  
          #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
              MainMenu =  MENU2_SUB12;// 
          #endif   
          #ifdef STM32_F407 //
              MainMenu =  MENU2_SUB09;// 5
          #endif           
      break;
    case MENU2_SUB02 :  MainMenu =  MENU2_SUB01;//
      break;
    case MENU2_SUB03 :  MainMenu =  MENU2_SUB02;//
      break;
    case MENU2_SUB04 :  MainMenu =  MENU2_SUB03;//
      break;
    case MENU2_SUB05 :  MainMenu =  MENU2_SUB04;//
      break;
    case MENU2_SUB06 :  MainMenu =  MENU2_SUB05;//
      break;
    case MENU2_SUB07 : MainMenu =  MENU2_SUB06;// 
      break;
   case MENU2_SUB08 :  MainMenu =  MENU2_SUB07;//
      break;
    case MENU2_SUB09 : MainMenu =  MENU2_SUB08;// 5   
      break;     
   case MENU2_SUB10 : MainMenu =  MENU2_SUB09;// 10
      break;
    case MENU2_SUB11 : MainMenu =  MENU2_SUB10;//  20
      break;  
    case MENU2_SUB12 : MainMenu =  MENU2_SUB11;//  60
      break;  
    case MENU2_SUB13 :  
      break;  
    case MENU2_SUB14 :  
      break; 
      
    case MENU3_SUB1 :  MainMenu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break; 
    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4_SUB4; 
      break; 
    case MENU4_SUB4 :   MainMenu = MENU4_SUB1; 
      break; 
             
    case MENU5_SUB1 :  

      break;
    case MENU5_SUB2 :

      break;
    case MENU5_SUB3 :  

      break;
    case MENU5_SUB4 :

      break;
    case MENU5_SUB5 :  

      break;
    case MENU5_SUB6 :  

      break;   
     case MENU5_SUB7 :      
      break;
      
      case MENU6_SUB1:  MainMenu = MENU6_SUB4; // Calib Start?  ..60sec   
        break;
      case MENU6_SUB2:  //       
        break;        
      case MENU6_SUB3:      
        break;
      case MENU6_SUB4:  MainMenu = MENU6_SUB1; // Calib Start?  ..60sec        
        break;
      case MENU6_SUB5: MainMenu = MENU6_SUB6; // Calib Start?  ..60sec    //       
        break;        
      case MENU6_SUB6:  MainMenu = MENU6_SUB5; // Calib Start?  ..60sec       
        break;    
      case MENU6_SUB7:      
        break; 
        
      case MENU7_SUB1 : MainMenu =  MENU7_SUB7;//
        break;
      case MENU7_SUB2 : MainMenu =  MENU7_SUB1;//
        break;
      case MENU7_SUB3 : MainMenu =  MENU7_SUB2;//
        break;
      case MENU7_SUB4 : MainMenu =  MENU7_SUB3;//
        break;
      case MENU7_SUB5 : MainMenu =  MENU7_SUB4;//
        break;
      case MENU7_SUB6 : MainMenu =  MENU7_SUB5;//
        break;
      case MENU7_SUB7 :MainMenu =  MENU7_SUB6;//
        break;   
      case MENU7_SUB8 :
        break;            
      case MENU7_SUB9 :
        break; 

      case MENU8_SUB1 : MainMenu =  MENU8_SUB2;
        break; 
      case MENU8_SUB2 : MainMenu =  MENU8_SUB1;
        break;         
      case MENU8_SUB3 :
        break; 
        
        default: MainMenu = MENU_NULL;
     break;

  }
} // DownMenuKey

void UpMenuKey(void) {
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU_NULL;
          KeySensonsorRoll(UPROLL);
      break;
    case MENU1 : MainMenu = MENU2; //
      break;
    case MENU2 : MainMenu = MENU3;
      break;
    case MENU3 : MainMenu = MENU4;
      break;
    case MENU4 : MainMenu = MENU5;
      break;
     case MENU5 : MainMenu = MENU6;
      break;     
     case MENU6 : MainMenu = MENU7;
      break; 
     case MENU7 : MainMenu = MENU8;
      break; 
     case MENU8 : MainMenu = MENU1;
      break; 

      
    case MENU1_SUB1 :  MainMenu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;
    case MENU2_SUB01 :  MainMenu =  MENU2_SUB02;//
      break;
    case MENU2_SUB02 :  MainMenu =  MENU2_SUB03;//
      break;
    case MENU2_SUB03 :  MainMenu =  MENU2_SUB04;//
      break;
    case MENU2_SUB04 :  MainMenu =  MENU2_SUB05;//
      break;
    case MENU2_SUB05 :  MainMenu =  MENU2_SUB06;//
      break;
    case MENU2_SUB06 :  MainMenu =  MENU2_SUB07;//
      break;
    case MENU2_SUB07 :  MainMenu =  MENU2_SUB08;//
      break;
    case MENU2_SUB08 :  MainMenu =  MENU2_SUB09;
      break;
     case MENU2_SUB09 :  
      #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE)     
            MainMenu =  MENU2_SUB10;
      #endif
      #ifdef STM32_F407 // 10msec
            MainMenu =  MENU2_SUB01;//
       #endif    
      break;  
    case MENU2_SUB10 :  MainMenu =  MENU2_SUB11;
      break;
     case MENU2_SUB11 :  MainMenu =  MENU2_SUB12;
      break;  
     case MENU2_SUB12 :  MainMenu =  MENU2_SUB01;//
      break; 
     case MENU2_SUB13 :  
      break;  
     case MENU2_SUB14 :  
      break; 
         
    case MENU3_SUB1 :  MainMenu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;    
    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4_SUB4; 
      break;  
    case MENU4_SUB4 :   MainMenu = MENU4_SUB1; 
      break;        
    case MENU5_SUB1 :  

      break;
    case MENU5_SUB2 :
 
    
      break;
    case MENU5_SUB3 :  

      break;
    case MENU5_SUB4 :

      break;
    case MENU5_SUB5 :  

      break;
    case MENU5_SUB6 :  

      break;
     case MENU5_SUB7 :      
      break;
     case MENU6_SUB1: MainMenu = MENU6_SUB4; // Calib Start?  ..60sec     
      break;
      case MENU6_SUB2:  //       
      break;        
       case MENU6_SUB3:      
      break;
       case MENU6_SUB4: MainMenu = MENU6_SUB1;  //   Calibration Started !  
      break;
      case MENU6_SUB5: MainMenu = MENU6_SUB6; //       
      break;        
       case MENU6_SUB6: MainMenu = MENU6_SUB5;     
      break;    
        case MENU6_SUB7:      //   Mains Updated !   
      break; 
      
      case MENU7_SUB1 : MainMenu =  MENU7_SUB2;
        break;
      case MENU7_SUB2 : MainMenu =  MENU7_SUB3;//
        break;
      case MENU7_SUB3 : MainMenu =  MENU7_SUB4;//
        break;
      case MENU7_SUB4 : MainMenu =  MENU7_SUB5;//
        break;
      case MENU7_SUB5 : MainMenu =  MENU7_SUB6;//
        break;
      case MENU7_SUB6 : MainMenu =  MENU7_SUB7;//
        break;
      case MENU7_SUB7 :MainMenu =  MENU7_SUB1;//
        break;   
      case MENU7_SUB8 :
        break;      
      case MENU7_SUB9 :
        break;   
      case MENU8_SUB1 : MainMenu =  MENU8_SUB2;
        break; 
      case MENU8_SUB2 : MainMenu =  MENU8_SUB1;
        break;         
      case MENU8_SUB3 :
        break; 

        
    default: MainMenu = MENU_NULL;
      break;
  }
}


void EscMenuKey(void) {   
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU_NULL;
      break;
    case MENU1 : MainMenu = MENU_NULL; //
      break;
    case MENU2 : MainMenu = MENU_NULL;
      break;
    case MENU3 : MainMenu = MENU_NULL;
      break;
    case MENU4 : MainMenu = MENU_NULL;
      break;        
    case MENU5 : MainMenu = MENU_NULL;
      break;
    case MENU6:   MainMenu = MENU_NULL; //     
      break;
    case MENU7:   MainMenu = MENU_NULL; //     
      break;
    case MENU8:   MainMenu = MENU_NULL; //     
      break;
           
    case MENU1_SUB1 :  MainMenu =  MENU1;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1;//
      break;
    case MENU1_SUB3 :     // LOG Started  
      break;
    case MENU1_SUB4 :     // LOG Stoped
      break;
    case MENU2_SUB01 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB02 : MainMenu =  MENU2;//
      break;
    case MENU2_SUB03 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB04 : MainMenu =  MENU2;//
      break;
    case MENU2_SUB05 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB06 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB07 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB08 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB09 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB10 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB11 :  MainMenu =  MENU2;//
      break;
     case MENU2_SUB12 :  MainMenu =  MENU2;//
      break;     
      case MENU2_SUB13 : 

      break;
      case MENU2_SUB14 : // Sample Time Updated !
      break;

      
    case MENU3_SUB1 :  MainMenu =  MENU3;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3;//
      break;
    case MENU3_SUB3 :   // STANDBYE Updated ! On
      break;
    case MENU3_SUB4 :  // STANDBYE Updated ! Off
      break;
      
     case MENU4_SUB1 :   MainMenu = MENU4; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4; 
      break;  
     case MENU4_SUB4 :   MainMenu = MENU4; 
      break;             
    case MENU5_SUB1 :  MainMenu = MENU5;
      break;
    case MENU5_SUB2 :  MainMenu = MENU5_SUB1;
      break;
    case MENU5_SUB3 :  MainMenu = MENU5_SUB2;
      break;
    case MENU5_SUB4 :  MainMenu = MENU5_SUB3;
      break;
    case MENU5_SUB5 :  MainMenu = MENU5_SUB4;
      break;
    case MENU5_SUB6 :  MainMenu = MENU5_SUB5;
      break;
    case MENU5_SUB7 :      // Date & Time Updated !
      break;
      

    case MENU6_SUB1: MainMenu = MENU6; //     
      break;
    case MENU6_SUB2: MainMenu = MENU6; //       
      break;        
    case MENU6_SUB3:     //  Calibration Started !     
      break;  
    case MENU6_SUB4: MainMenu = MENU6; //     //  Mains EU/US Select 
      break;
     case MENU6_SUB5:  MainMenu = MENU6;//       
      break;        
     case MENU6_SUB6: MainMenu = MENU6;      
      break;    
     case MENU6_SUB7:       //   Mains Updated !   
      break;  
      
      case MENU7_SUB1 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB2 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB3 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB4 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB5 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB6 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB7 : MainMenu =  MENU7;//
        break; 
      case MENU7_SUB8 :  

        break; 
       case MENU7_SUB9 :  // File Size Updated !
        break;     

      case MENU8_SUB1 : MainMenu =  MENU8;
        break; 
      case MENU8_SUB2 : MainMenu =  MENU8;
        break;         
      case MENU8_SUB3 :  // Log Mode Updated !
        break;
                         
    default: MainMenu = MENU_NULL;
    break;
  }
}

void EnterMenuKey(void){
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU1;
        break;
    case MENU1 : //Menu = MENU1MIN; 
        MainMenu = MENU1_SUB2;  //  log disabled  
        
      break;
   case MENU2 : // Menu = MENU2MIN; // call the right menu according to current one

      break;  
    case MENU3 : // Menu = MENU3MIN;
        if (Display.SleepEnable == ON) MainMenu = MENU3_SUB2; //already logging
        else  MainMenu = MENU3_SUB1;
      break;
    case MENU4 :   MainMenu = MENU4_SUB1;       
      break;
    case MENU5 :  MainMenu = MENU5_SUB1;
        
      break;
    case MENU6:   MainMenu = MENU6_SUB1; //     
      break;  
                 
    case MENU1_SUB1 :  //SetResetLog(ON); 
        MainMenu =  MENU1_SUB3;//MENU1
      break;
    case MENU1_SUB2 : // SetResetLog(OFF);

  
        MainMenu =  MENU1_SUB4;//MENU1
      break;
    case MENU1_SUB3 :  
      break;
    case MENU1_SUB4 :   
      break;
  
    case MENU2_SUB13 :  

      
        MainMenu =  MENU2_SUB14;     
      break;
      case MENU2_SUB14 :  
      
      break;  
    case MENU3_SUB1 :  DispEnable(ON,40);

      MainMenu =  MENU3_SUB3;//MENU3
      break;
    case MENU3_SUB2 :  DispEnable(OFF,0);

      MainMenu =  MENU3_SUB4;//MENU3
      break;
    case MENU3_SUB3 :  
      
      break;
    case MENU3_SUB4 :     
      break; 
        

    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;    
    case MENU4_SUB3 :   MainMenu = MENU4_SUB4; 
      break;  
    case MENU4_SUB4 :   MainMenu = MENU4_SUB1; 
      break;               
      
     case MENU5_SUB1 :  MainMenu = MENU5_SUB2;   //Year  
      break;     
     case MENU5_SUB2 :  MainMenu = MENU5_SUB3; // Month
      break;        
     case MENU5_SUB3 :  MainMenu = MENU5_SUB4; // Day
      break;  
     case MENU5_SUB4 :  MainMenu = MENU5_SUB5; // Hour
      break;  
     case MENU5_SUB5 :  MainMenu = MENU5_SUB6; // Minute
      break;
     case MENU5_SUB6 :  MainMenu = MENU5_SUB7; //Second

      break;
     case MENU5_SUB7 :      
      break;
          
     case MENU6_SUB1: MainMenu = MENU6_SUB2; //     
        break;
     case MENU6_SUB2: MainMenu = MENU6_SUB3; //
     
      break;        
     case MENU6_SUB3:      // Calibration Started !
        break;  
     case MENU6_SUB4:   MainMenu = MENU6_SUB5; //  Mains EU/US Select    
        break;
     case MENU6_SUB5:  MainMenu = MENU6_SUB7;  
      
      break;        
     case MENU6_SUB6: MainMenu = MENU6_SUB7;  
           
      break;    
     case MENU6_SUB7:    //   Mains Updated !     
      break;    

    case MENU7:     


        
      case MENU7_SUB8 :  // Sure If Yes Enter
  

      case MENU8:  
        break;       
      case MENU8_SUB1 : // SD Mode
  
        break; 
      case MENU8_SUB2 : //PC Mode
 
        break;         
      case MENU8_SUB3 :  // Log Mode Updated !
            
        break;
            
    default: MainMenu = MENU_NULL; 
    break;
  }
}
