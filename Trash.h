 /** @defgroup GPIO_pins GPIO pins
  * @{
  */
#define GPIO_PIN_0                 ((uint16_t)0x0001U)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002U)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004U)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008U)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010U)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020U)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040U)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080U)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100U)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200U)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400U)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800U)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000U)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000U)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000U)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000U)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFFU)  /* All pins selected */
/*
 void  Detect_Thyristor_Firing_Angle(struct Grid_Thy_I_Chopped_Var *p, const uint16_t IO_ThyristorFire){
    uint8_t Counter;
   if(p->V_ZeroCross){ // 
      p->Thyristor_FireAngle_Counter++;

   //   p->Thyristor_Sim_Firing = ON;
   //   p->Thyristor_Sim_Firing_Angle = 50;
    //  p->IO_Thyristor_Firing_Debug = ON; 

     if(p->Thyristor_Sim_Firing){ // only for hardware debugging
        if((p->Thyristor_FireAngle_Counter >= p->Thyristor_Sim_Firing_Angle) ){ // for debugging 90 radian trigger  100 5 ms can be assigned && !(p->IO_Thyristor_Firing_Debug)
          if(p->IO_Thyristor_Firing_Debug == OFF){
       //   p->V_ZeroCross = OFF;
            p->Thyristor_Firing_Angle = p->Thyristor_Sim_Firing_Angle; // timeout for fire angle cycle ended 
            p->I_CosFi_Fire_Delay = ON;
           // p->I_Chopped_Counter = 0;
            p->Thyristor_Max_Sim_Firing_Counter = 0;
              #ifdef LOGIC_ANALYSER_DEBUG 
                p->IO_Thyristor_Firing_Debug = ON; 
              #endif
          }
        }
        if(p->IO_Thyristor_Firing_Debug){
          p->Thyristor_Max_Sim_Firing_Counter ++;
          if(p->Thyristor_Max_Sim_Firing_Counter >= 20){ // max 500 uS firing
            p->Thyristor_Max_Sim_Firing_Counter = 0;
            p->V_ZeroCross = OFF;
              #ifdef LOGIC_ANALYSER_DEBUG 
                p->IO_Thyristor_Firing_Debug = OFF; 
              #endif
          }
        }              
      }
      else { // no debugging countiniue from here
        if(p->Thyristor_FireAngle_Counter >= Half_Cycle_Time ){ // for debugging 90 radian trigger  100 5 ms can be assigned 
         p->V_ZeroCross = OFF;
          p->Thyristor_Firing_Angle = Half_Cycle_Time; // timeout for fire angle cycle ended 
        }
        if(digitalRead(IO_ThyristorFire) && (p->Thy_Prev_IO_State == 0)){ // fired calculate time,  check if Low 2 High Occured
          p->V_ZeroCross = OFF;
          p->Thyristor_Firing_Angle = Half_Cycle_Time -  p->Thyristor_FireAngle_Counter ; // save the firing angle time
          p->I_CosFi_Fire_Delay = ON;
      }
    }
 }
  p->Thy_Prev_IO_State = digitalRead(IO_ThyristorFire);

}
*/
 /*
bool Create_Chopped_Current(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift, uint8_t ΔtCoSφ){
  // uint8_t CosFi_time = CoSφ;// Cosφ added as a shift 100->cosφ 1 // 0->cosφ 0 //100 variable value stands for 90° radian // 5 mSec 

   if(p->I_Chopped_DutyCycle > 98) p->I_Chopped_DutyCycle = 98; // max %98 if full cycle in case no pahase angle cut 
   if(p->I_Chopped_DutyCycle < 2) p->I_Chopped_DutyCycle = 2; // min %2 

 
  uint8_t I_Chop_DutyCycle = (p->Thyristor_Firing_Angle *p->I_Chopped_DutyCycle) / 100;
  I_Chop_DutyCycle = 100;

  // CosFi 0/90 as radyan 0 - 1,0 1->on phase    0->90radyan->5mSec->100 Δ
 //  PhaseShift += (100-CoSφ); // Cosφ added as a shift 100->cosφ 1 // 0->cosφ 0 //100 variable value stands for 90° radian // 5 mSec 
  ΔtCoSφ  = 100 - ΔtCoSφ;
  p->I_Chopped_Counter++;    //!!!!  p->I_Chopped_DutyCycle cant be bigger than p->Thyristor_Firing_Angle 
  if(p->I_Chopped_Counter  <= I_Chop_DutyCycle + PhaseShift + ΔtCoSφ){ // reference full bypass current trajectory created for full trigger
    if(p->I_CosFi_Fire_Delay){
      p->I_CoSFi_Fire_Delay_Counter ++;
      if(p->I_CoSFi_Fire_Delay_Counter >= ΔtCoSφ){ //90° - CoSφ // counter until  cosfi delay for chopped cur generation
        p->I_CoSFi_Fire_Delay_Counter = 0; // chopped current created when current zero cross occured 
        p->I_CosFi_Fire_Delay = OFF; // when chooped current created will keep its value until phase timeout occures
        p->IO_Current  = ON;       // however will implement duty cycle to have a varible rms current
      }                          
    }
  }
  else{   // timeout occured current half cycle ended
    if(p->I_Chopped_Counter >= Half_Cycle_Time + PhaseShift + ΔtCoSφ) p->I_Chopped_Counter  = PhaseShift + ΔtCoSφ;// Full Period Timeout Start Again %100
    p->IO_Current  = OFF;   
  }
    #ifdef LOGIC_ANALYSER_DEBUG
      if(p->IO_Current)p->IO_Chopped_Current_Debug = ON;   // debugging chopped current 
      else p->IO_Chopped_Current_Debug = OFF;
    #endif
  return p->IO_Current; 
}
*/
 
 #ifdef ARDUINO_MEGA
  wdt_reset();
  wdt_enable(WDTO_8S);   //wdt_enable(WDT0_1S);
  //https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
  #endif

  #ifdef ARDUINO_DUE
  wdt_reset();
  wdt_enable();   
  //https://forum.arduino.cc/t/due-watchdog-timer-not-working/525122/2
  #endif

  #ifdef ARDUINO_MEGA
//  ADCSRA &= ~ (1 << ADEN);            // turn off ADC to save power ,, enable when needed and turn off again
    ADCSRA |= (1 << ADEN); // enable adc
  #endif

#ifdef ARDUINO_DUE // 
  //Due_Memory();
  analogReadResolution(12);
#endif
#ifdef ARDUINO_MEGA // 8 bit AVR 
  wdt_reset();
  wdt_enable(WDTO_8S);
#endif
#ifdef ARDUINO_DUE // 
  wdt_reset();
  wdt_enable();
#endif
  #ifdef ARDUINO_MEGA // 8 bit AVR
#include <EEPROM.h>
#include <avr/wdt.h>
#include <ArduinoUniqueID.h>
#endif
#ifdef ARDUINO_DUE // ARM CortexM3 32 bit
#include <malloc.h>
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;
#include <ArduinoUniqueID.h>
#endif

  if(R_Phase.UI_VoltEnable){
    R_Phase.PhaseCounter++;
    if(R_Phase.PhaseCounter  <= R_Phase.Volt_DutyCycle + R_PHASE_0){
        if(R_Phase.VoltagePin  == 0) {
          R_Phase.VoltZeroCross = ON; // 0->1 low 2 high state //Voltage Zero Cross Occured  Thyristor Trig Enabled 
          R_Phase.Thy_Phase_Counter = 0; 
        }
       R_Phase.VoltagePin  = ON;   
    }
    else{
      if(R_Phase.PhaseCounter >= Half_Cycle_Time + R_PHASE_0) R_Phase.PhaseCounter  = R_PHASE_0;
      R_Phase.VoltagePin = OFF;      
    }
  } 
  else R_Phase.VoltagePin = OFF;  


  if(R_Phase.VoltagePin)digitalWrite(PSR_ZEROCROSS_1, HIGH);
  else digitalWrite(PSR_ZEROCROSS_1, LOW);
  //   const uint16_t VoltIO = PSR_ZEROCROSS_2;
 //   VoltIO = PSR_ZEROCROSS_1;

   //   if(R_Phase.VoltagePin)digitalWrite(VoltIO, HIGH);
   //   else digitalWrite(VoltIO, LOW);


  if(R_Phase.VoltZeroCross){
      R_Phase.Thy_Phase_Counter++;
      if(R_Phase.Thy_Phase_Counter >= Half_Cycle_Time){
        R_Phase.VoltZeroCross = 0;
        R_Phase.Firing_Angle = Half_Cycle_Time;
      }
      if(digitalRead(PSR_THY_TR1) && (R_Phase.Thy_PrevPinState == 0)){ // fired calculate time
        R_Phase.VoltZeroCross = 0;
        R_Phase.Firing_Angle = Half_Cycle_Time - R_Phase.Thy_Phase_Counter;
      }
  }
  R_Phase.Thy_PrevPinState = digitalRead(PSR_THY_TR1);




  if(S_Phase.UI_VoltEnable){
    S_Phase.PhaseCounter++;
    if(S_Phase.PhaseCounter  <= S_Phase.Volt_DutyCycle + S_PHASE_120){
        if(S_Phase.VoltagePin  == 0) {
          S_Phase.VoltZeroCross = ON; // 0->1 low 2 high state //Voltage Zero Cross Occured  Thyristor Trig Enabled 
          S_Phase.Thy_Phase_Counter =  0; 
        }
       S_Phase.VoltagePin  = ON;   
    }
    else{
      if(S_Phase.PhaseCounter >= Half_Cycle_Time + S_PHASE_120) S_Phase.PhaseCounter  = S_PHASE_120;
      S_Phase.VoltagePin = OFF;      
    }
  } 
  else S_Phase.VoltagePin = OFF;  

  if(S_Phase.VoltagePin)digitalWrite(PSR_ZEROCROSS_2, HIGH);
  else digitalWrite(PSR_ZEROCROSS_2, LOW);


   if(S_Phase.VoltZeroCross){
      S_Phase.Thy_Phase_Counter++;
      if(S_Phase.Thy_Phase_Counter >= Half_Cycle_Time ){
      // S_Thy.Counter =0;
       S_Phase.VoltZeroCross = OFF;
       S_Phase.Firing_Angle = Half_Cycle_Time;
       }
      if(digitalRead(PSR_THY_TR2) && (S_Phase.Thy_PrevPinState == 0)){
        S_Phase.VoltZeroCross = OFF;
        S_Phase.Firing_Angle = Half_Cycle_Time - (S_Phase.Thy_Phase_Counter - 0);
       // S_Thy.Counter = 0;
      }
  }
   S_Phase.Thy_PrevPinState = digitalRead(PSR_THY_TR2);
*/
/*
  if(UI_Command.T_VoltTrig){
    T_Thy.CyleTimer++;
    if(T_Thy.CyleTimer  <= UI_Command.Duty_Cyle + T_SHIFT){
      digitalWrite(PSR_ZEROCROSS_3, HIGH);
      T_Thy.PinState = 1;    
    }
    else{
      if(T_Thy.CyleTimer  >= Half_Cycle_Time + T_SHIFT) T_Thy.CyleTimer = T_SHIFT;
      digitalWrite(PSR_ZEROCROSS_3, LOW);  
      T_Thy.PinState = 0;  
    }
  }
  else digitalWrite(PSR_ZEROCROSS_3, LOW); 





// Nucleo-144 boards don't have Serial1 defined by default
HardwareSerial Serial1(PC11, PC10);  // Rx=PC11, Tx=PC10 -- USART3 - F412ZG/F446ZE
HardwareSerial Serial3(PD2, PC12);  // Rx=PC7, Tx=PC6 -- UART5 - F412ZG/F446ZE
// Serial2 is defined to use USART2 by default, but is in fact used as the diag console
HardwareSerial Serial1(PG9, PG14);  // Rx=PG9, Tx=PG14 -- USART6
// Serial3 is defined to use USART3 by default, but is in fact used as the diag console
// via the debugger on the Nucleo-144. It is therefore unavailable for other DCC-EX uses like WiFi, DFPlayer, etc.
// NB:
//    On all of the above, USART3, and USART6 are available but as yet undefined
//    On F446ZE and F429ZI, UART4, UART5 are also available but as yet undefined
//    On F429ZI, UART7 and UART8 are also available but as yet undefined

void Parse_FileString(){
  int DelimCount=0;
  int j = 0;
  int index;
  for (unsigned int i = 0; i < Config_Str.length(); i++) {
    if (Config_Str.substring(i, i+1) == ",") {
      switch(DelimCount){
        case 0:Relay1str = Config_Str.substring(j, i);
        break;
        case 1:RlStr2 = Config_Str.substring(j,i);
        break;
        case 2:RLlVal = Config_Str.substring(j,i);
        break;
        case 3:RlStr4 = Config_Str.substring(j,i);
        break;
        case 4:Relay2str = Config_Str.substring(j,i);
        break;
        case 5:RlStr6 = Config_Str.substring(j,i);
        break; 
        case 6:RL2Val = Config_Str.substring(j,i);
             j = i+1;
             RlStr8 = Config_Str.substring(j);
        break; 
        default:
        break;   
      }
      j = i+1;
      DelimCount++;
    }
  }
 
    index = ELEMENTS;
    Relay1str.trim();//remove leadig & last space characters
    if(Relay1str == "Relay1"){
     // index == 0; ?????????????????
      RLlVal.trim();  // Temperature // Current // PM25
      for( index = 0; index < ELEMENTS; index++){
        if(RLlVal == KeyWords[index]){      
          RlStr2.trim();
          RlStr4.trim();       
          RL1Min =RlStr2.toFloat();
          RL1Max= RlStr4.toFloat();
          
          break;  
        }  
      } 
   }
   if(index == ELEMENTS) {
        Relay1str = "---";     
        RLlVal = "Nan";   
        RlStr2 = "----";
        RlStr4 = "----";
    }
    index = ELEMENTS;
    Relay2str.trim();//remove leadig & last space characters
    if(Relay2str == "Relay2"){
      index = 0;
      RL2Val.trim();  // Temperature // Current // PM25
      for( index = 0; index < ELEMENTS; index++){
        if(RL2Val == KeyWords[index]){      
          RlStr6.trim();
          RlStr8.trim();       
          RL2Min= RlStr6.toFloat();
          RL2Max= RlStr8.toFloat();
          break;  
        }  
      }     
   }
    if(index == ELEMENTS) {
        Relay2str = "---";     
        RL2Val = "Nan";   
        RlStr6 = "----";
        RlStr8 = "----";
    }
   // Serial.println("'''''''''''''''''");
  Serial.println(Relay1str);
  Serial.println(RlStr2);
  Serial.println(RLlVal);
  Serial.println(RlStr4);
  Serial.println(Relay2str);
  Serial.println(RlStr6);
  Serial.println(RL2Val);
  Serial.println(RlStr8);
}
#define Relay1_Val 8
#define Relay2_Val 4

float GetValue(uint8_t Relay){
  String Val = "";
  if(Relay == Relay1_Val) Val =  RLlVal;
  else if(Relay == Relay2_Val) Val =  RL2Val;
  if(Val == KeyWords[0])return Values.Temperature_Ch1;
  if(Val == KeyWords[1])return Values.Temperature_Ch2;
  if(Val == KeyWords[2])return  Values.Temperature_Ch3;
  if(Val == KeyWords[3])return Values.Humidity_Ch1;
  if(Val == KeyWords[4])return  Values.Humidity_Ch2;
  if(Val == KeyWords[5])return  Values.Humidity_Ch3;
  if(Val == KeyWords[6])return  Values.PM25;
  if(Val == KeyWords[7])return  Values.PM10;
  if(Val == KeyWords[8]) return  Values.Current;
  if(Val == KeyWords[9]) return  Values.Voltage;
  if(Val == KeyWords[10]) return  Values.ActivePower;  
  if(Val == KeyWords[11]) return  Values.PowerFactor;     
  return 0;
}


void Relay_loop() {
 // Parse_FileString();
  CompValue = 0;
  if( RLlVal != "Nan"){
    CompValue = GetValue(Relay1_Val);
    if(CompValue > RL1Max && !digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,HIGH);
    if(CompValue < RL1Min &&  digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,LOW); 
  }
  else{
      digitalWrite(RELAY_OUT_1,LOW);
  }
  Serial.print(F("RL1Min: "));Serial.println(RL1Min);
  Serial.print(RLlVal+":  ");Serial.print(CompValue); 
  Serial.print(F("     RELAY1: "));Serial.println(RELAY_OUT_1); 
  Serial.print(F("RL1Max: "));Serial.println(RL1Max); 
  CompValue = 0;
  if( RL2Val != "Nan"){
    CompValue = GetValue(Relay2_Val);
    //if((CompValue < RL2Min)&& RELAY2) RELAY2 = 0;
    //if((CompValue > RL2Max)&& !RELAY2) RELAY2 = 1;  
    if(CompValue > RL2Max && !digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,HIGH);
    if(CompValue < RL2Min &&  digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,LOW);  
  }
  else{
       digitalWrite(RELAY_OUT_2,LOW);  
  }
  Serial.print(F("RL2Min: "));Serial.println(RL2Min);
  Serial.print(RL2Val+":  ");Serial.print(CompValue);
  Serial.print(F("      RELAY2: "));Serial.println(RELAY_OUT_2);
  Serial.print(F("RL2Max: "));Serial.println(RL2Max);  
}
void PrintSensor(void){
/*
      Serial.println();

      Serial.print("Ax: ");   //3
      Serial.println(Accelometer.x);   //5

      Serial.print("Ay: ");   //3
      Serial.println(Accelometer.y);   //5

      Serial.print("Az: ");   //3
      Serial.println(Accelometer.z);   //5

      Serial.print("Temp: ");   //3
      Serial.println(Temperature);   //5

      Serial.print("roll: ");   //3
      Serial.println(roll);   //5

      Serial.print("rollF: ");   //3
      Serial.println(rollF);   //5

      Serial.print("pitch: ");   //3
      Serial.println(pitch);   //5

      Serial.print("pitchF: ");   //3
      Serial.println(pitchF);   //5      

      Serial.print("yaw: ");   //3
      Serial.println(yaw);   //5

      Serial.print("yawF: ");   //3
      Serial.println(yawF);   //5     

      Serial.print("Gx: ");   //3
      Serial.println(Gyro.x);   //5

      Serial.print("Gy: ");   //3
      Serial.println(Gyro.y);   //5

      Serial.print("Gz: ");   //3
      Serial.println(Gyro.z);   //5


      Serial.print("roll_2: ");   //3
      Serial.println(roll_2);   //5

      Serial.print("pitch 2: ");   //3
      Serial.println(pitch_2);   //5 

      Serial.print("yaw 2: ");   //3
      Serial.println(yaw_2);   //5 
      
*/
} 

void Startup_NV_Ram(){
  /*
// #if  defined (ARDUINO_DUE) |  defined (STM32_F407)   
    #ifdef ARDUINO_DUE // 8 bit AVR
      char serial[4] = {'F','5','6','C'}; // DUE is just flash every time compile and right fully cleaned
      char que[2] = {'0','1'};            // give default numbers otherwise need to use serial port to update
      NVRam_Write_QueNo(&que[0]);
      NVRam_Write_SerNo((char*)&serial);
    #endif

    NVRam_Read_SerNo();
    NVRam_Read_QueNo();



  */      

}

void NVRam_Read_QueNo() {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
    File_Que[0] = (char)EEPROM.read(NVRAM_QUE1);
    File_Que[1] = (char)EEPROM.read(NVRAM_QUE2);

  #endif
   #ifdef ARDUINO_DUE // 8 bit AVR
    File_Que[0] = (char)dueFlashStorage.read(NVRAM_QUE1);
    File_Que[1] = (char)dueFlashStorage.read(NVRAM_QUE2);
   #endif
   #ifdef STM32_F407 // 8 bit AVR
    //File_Que[0] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_QUE1),NUMBER_ONLY);
   // File_Que[1] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_QUE2),NUMBER_ONLY);   
   #endif
   

}
void NVRam_Write_QueNo(char* p) {
  Serial.print(F("File_Que:"));
  #ifdef ARDUINO_MEGA // 8 bit AVR
          EEPROM.write(NVRAM_QUE1, p[0]);// high byte
          EEPROM.write(NVRAM_QUE2, p[1]);// high byte                         
    #endif
    #ifdef ARDUINO_DUE
          dueFlashStorage.write(NVRAM_QUE1, p[0]);// high byte
          dueFlashStorage.write(NVRAM_QUE2, p[1]);// high byte                 
    #endif
       #ifdef STM32_F407 // 8 bit AVR
    //      EEPROM.write(NVRAM_QUE1, p[0]);// high byte
    //      EEPROM.write(NVRAM_QUE2, p[1]);// high byte    
   #endif
      //   Serial.println(p[0]);Serial.println(p[1]);
}

void NVRam_Read_SerNo() {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
    Device_Id[0] = (char)EEPROM.read(NVRAM_ID1);
    Device_Id[1] = (char)EEPROM.read(NVRAM_ID2);
    Device_Id[2] = (char)EEPROM.read(NVRAM_ID3);
    Device_Id[3] = (char)EEPROM.read(NVRAM_ID4);
  #endif
   #ifdef ARDUINO_DUE // 8 bit AVR
    Device_Id[0] = (char)dueFlashStorage.read(NVRAM_ID1);
    Device_Id[1] = (char)dueFlashStorage.read(NVRAM_ID2);
    Device_Id[2] = (char)dueFlashStorage.read(NVRAM_ID3);
    Device_Id[3] = (char)dueFlashStorage.read(NVRAM_ID4);
   #endif
      #ifdef STM32_F407 // 8 bit AVR
    //Device_Id[0] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID1),LETTER_NUMBER);
 //   Device_Id[1] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID2),LETTER_NUMBER);
 //   Device_Id[2] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID3),LETTER_NUMBER);
 //   Device_Id[3] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID4),LETTER_NUMBER);   
   #endif
//   Serial.print(F("Device_Id: ")) ;
 //  Serial.println(Device_Id);  
}
void NVRam_Write_SerNo(char* p) {
  Serial.print(F("EECode: "));
  #ifdef ARDUINO_MEGA // 8 bit AVR
          EEPROM.write(NVRAM_ID1, p[0]);// high byte
          EEPROM.write(NVRAM_ID2, p[1]);// high byte                 
          EEPROM.write(NVRAM_ID3, p[2]);// high byte         
          EEPROM.write(NVRAM_ID4, p[3]);// high byte         
    #endif
    #ifdef ARDUINO_DUE
          dueFlashStorage.write(NVRAM_ID1, p[0]);// high byte
          dueFlashStorage.write(NVRAM_ID2, p[1]);// high byte                
          dueFlashStorage.write(NVRAM_ID3, p[2]);// high byte        
          dueFlashStorage.write(NVRAM_ID4, p[3]);// high byte         
    #endif
       #ifdef STM32_F407 // 8 bit AVR
    //      EEPROM.write(NVRAM_ID1, p[0]);// high byte
    //      EEPROM.write(NVRAM_ID2, p[1]);// high byte                 
    //      EEPROM.write(NVRAM_ID3, p[2]);// high byte         
    //      EEPROM.write(NVRAM_ID4, p[3]);// high byte  
   #endif
  //       Serial.print(p[0]);Serial.print(p[1]);Serial.print(p[2]);Serial.println(p[3]);
}

char NVRam_Check_Content(char Content, bool Number){
   char AsciiTable[] =  {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};
   #define NUMBER_SIZE 10
   #define LETTER_NUMBER_SIZE 22   
   uint8_t Size;
   if (Number == LETTER_NUMBER)Size = LETTER_NUMBER_SIZE;// number and letter 
   if (Number == NUMBER_ONLY) Size = NUMBER_SIZE; // only numbers 
   for(uint8_t i=0; i<Size; i++){
       if(Content == AsciiTable[i])return Content;
   } 
   //return '0'; 
   if (Number == LETTER_NUMBER)  return AsciiTable[random(LETTER_NUMBER_SIZE)];
   if (Number == NUMBER_ONLY)    return AsciiTable[random(NUMBER_SIZE)];   
   return '0'; 
}
void NVRam_Write(uint8_t Address, uint8_t Sample){
  #ifdef ARDUINO_MEGA
      EEPROM.write(Address, Sample);// ON  
  #endif
     #ifdef ARDUINO_DUE 
    dueFlashStorage.write(Address,Sample); 
  #endif
     #ifdef CHIPKIT_MAX32 // 32 bit ARM
       
    #endif  
    #ifdef STM32_F407  
     EEPROM.write(Address, Sample);
    #endif        
}
uint8_t NVRam_Read(uint8_t Address) {  //
   #ifdef ARDUINO_MEGA // 8 bit AVR
    return EEPROM.read(Address);// OFF
    #endif
   #ifdef ARDUINO_DUE // 32 bit ARM
       return dueFlashStorage.read(Address);
    #endif
     #ifdef ARDUINO_MKRZERO
       // uint8_t Mode = 0;  
      #endif    
   #ifdef CHIPKIT_MAX32 // 32 bit ARM
      // uint8_t Mode = 0;
    #endif   
      #ifdef STM32_F407  
      return EEPROM.read(Address);// OFF
    #endif       
}