
//https://github.com/adafruit/Adafruit_SSD1306
// https://upload.wikimedia.org/wikipedia/commons/f/f8/Codepage-437.png

#ifdef OLEDDISPLAY_EXISTS

#ifdef ARDUINO_MKRZERO
#define OLED_GND A2// 13
#define OLED_POWER A3// 13
#define OLED_CS    A5// 13
#define OLED_RESET A6
#define OLED_DC    0// 12  // common 50
#define OLED_CLK   1 //13 // common  52
#define OLED_MOSI  2// 11 //common 51
#endif

#ifdef  ARDUINO_DUE
#define OLED_GND 47// 13
#define OLED_POWER 45// 13
#define OLED_CS    41// 13
#define OLED_RESET 39
#define OLED_DC    37// 12  // common 50
#define OLED_CLK   35 //13 // common  52
#define OLED_MOSI  33// 11 //common 51
#endif

#ifdef ARDUINO_MEGA
#define OLED_GND 47// 13
#define OLED_POWER 45// 13
#define OLED_CS    43// 13
#define OLED_RESET 41
#define OLED_DC    39// 12  // common 50
#define OLED_CLK   37 //13 // common  52
#define OLED_MOSI  35// 11 //common 51
#endif

#ifdef STM32_F407
int OLED_CS = PG4;  
int OLED_RESET= PG6 ; 
int OLED_DC = PG5; 
int OLED_CLK = PG7; 
int OLED_MOSI = PG8; 
//int OLED_GND ; 
// int OLED_POWER ; 
#endif

//#endif


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

/*
  In Adafruit_SSD1306.h
  1. uncomment #define SSD1306_128_64
  2. comment #define SSD1306_128_32
  3. comment #define SSD1306_96_16
  In the example ssd1306_128x64_i2c
  4. add #define SSD1306_LCDHEIGHT 64

*/
/*
  #ifdef SSD1306_LCDHEIGHT
  #define SSD1306_LCDHEIGHT 64
  #endif
  #ifndef SSD1306_LCDHEIGHT
  #define SSD1306_LCDHEIGHT 64
  #endif

  #if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
  #endif
*/

//#define DISPLAY_POWEROFF_STANDBYE
#define DISPLAY_SOFTOFF_STANDBYE
#if defined (DISPLAY_POWEROFF_STANDBYE)  & defined (DISPLAY_SOFTOFF_STANDBYE)
#error Select Only One Standbye Type
#endif
#if !(!defined (DISPLAY_POWEROFF_STANDBYE) ^ !defined (DISPLAY_SOFTOFF_STANDBYE))
#error Select At Least One Standbye Type
#endif

//DISPLAY INITIALIZER  // software SPI
#ifdef OLED_128X64
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif
#ifdef OLED_128X128
  Adafruit_SSD1327 display(128, 128, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif

void Display_SwitchOff() {
  MainMenu = MENU_NULL;
  display.clearDisplay();
  display.display();
#ifdef DISPLAY_POWEROFF_STANDBYE
//  digitalWrite(OLED_POWER, LOW);       // turn on pullup resistors
 // digitalWrite(OLED_GND, LOW);       // keep GND Level
#endif

}
void DisplaySetPowerIO() {
  //-- DISPLAY INIT --//
  /*
  pinMode(OLED_GND, OUTPUT);
  digitalWrite(OLED_GND, LOW);       // keep GND Level
  pinMode(OLED_POWER, OUTPUT);
  digitalWrite(OLED_POWER, HIGH);       // turn on pullup resistors
  */
}
void Display_ReInit_Start(uint8_t Timer) {
  DisplaySetPowerIO();
  Display.OLED_Timer = Timer; // 10 sec
  Display.ReInit_Enable = ON;
  Display.ReInit_Timer = 4;

}
void Display_ReInit_End() {
#ifdef DISPLAY_SOFTOFF_STANDBYE
  return;
#endif
#ifdef DISPLAY_POWEROFF_STANDBYE
  if (!Display.ReInit_Enable)return;
  if (Display.ReInit_Timer) {
    Display.ReInit_Timer--;
    return;
  }
  Display.ReInit_Enable = OFF;
  Display.ReInit_Timer = 4;

  // delay(300); // Pause for 2 seconds // critical
#if defined  ARDUINO_DUE | defined (ARDUINO_MKRZERO)
  delay(1300); // Pause for 2 seconds
  if (!display.begin(SSD1306_SWITCHCAPVCC)) { //    SSD1306_EXTERNALVCC
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don’t proceed, loop forever
  }
#endif
#if defined (ARDUINO_MEGA)  | defined (CHIPKIT_MAX32)
  if (!display.begin(SSD1306_SWITCHCAPVCC)) { //    SSD1306_EXTERNALVCC
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don’t proceed, loop forever
  }
#endif

  Serial.println(F("Display ReInitilized"));
  //   display.clearDisplay();
  //   display.setTextSize(3);
  //   display.setTextColor(WHITE);  //0  white on black
  //    display.setTextColor(0);  //1     Black on white
  display.dim(0); // lower brightness
  //   display.setCursor(0, 0);
  //   display.println();
  //    display.println("DATALOG");
  //   display.display();
#endif
}

void DisplayInit(void) {
  DisplaySetPowerIO();
  //-- DISPLAY INIT --//
  Display.OLED_Timer = 40; // 20-> 10 sec
  delay(300); // Pause for 2 seconds // critic !!
#ifdef ARDUINO_DUE
  delay(1300); // Pause for 2 seconds
#endif


#ifdef OLED_128X128
  delay(1300); // Pause for 2 seconds
  Serial.println("SSD1327 OLED test");
  if ( ! display.begin(0x3D) ) {
     Serial.println("Unable to initialize OLED");
     while (1) yield();
  }
 //   display.dim(0); // lower brightness
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1327_WHITE);  //0  white on black
  //    display.setTextColor(0);  //1     Black on white
  display.setCursor(0, 0);
  display.println();
  display.println(F("DATALOG"));
  display.display();
#endif
  
#ifdef OLED_128X64
  if (!display.begin(SSD1306_SWITCHCAPVCC)) { //  SSD1306_EXTERNALVCC
    Serial.println(F("SSD1306 allocation failed"));
    // for(;;); // Don’t proceed, loop forever
  }
    display.dim(0); // lower brightness
    // display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);  //0  white on black
  //    display.setTextColor(0);  //1     Black on white
  display.setCursor(0, 0);
  display.println();
  display.println(F("DATALOG"));
  display.display();

  // Adafruit_SSD1306::dim  ( 1 ) //1 == lower brightness // 0 == full brightness
  //display.dim
#endif 
}

void testdrawchar(void) {
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
#ifdef OLED_128X64  
  display.setTextColor(SSD1306_WHITE); // Draw white text
#endif
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for (int16_t i = 0; i < 256; i++) {
    if (i == '\n') display.write(' ');
    else          display.write(i);
  }
  display.display();
  // delay(2000);
}

void UpdateDispSpChar(uint8_t Index, uint8_t Line) {
    switch (Line) {
      case 4: display.setCursor(36, 24); //x,y
        break;
      case 5: display.setCursor(36, 32);
        break;
      case 6: display.setCursor(36, 40);
        break;
      case 7: display.setCursor(36, 48);
        break;
      default:              //
        break;
    }
    switch (Index) {
      case DISPROLL_LINE1:
        if (Display.ExpSensOnb ) {
          Display.ExpSensOnb = OFF;
          display.write(247); // 5th character  '°';
        }
      break;  
    case DISPROLL_LINE2:
        if (Display.ExpSens1 ) {
          Display.ExpSens1 = OFF;
          display.write(247); // 5th character  '°';
        }
      break;
    case DISPROLL_LINE3:
        if (Display.ExpSens2 ) {
          Display.ExpSens2 = OFF;
          display.write(247); // 5th character  '°';
        }
      break;
    case DISPROLL_LINE4:
        if (Display.ExpSens3 ) {
          Display.ExpSens3 = OFF;
          display.write(247); // 5th character  '°';
        }
      break;
    default:              //
      break;
  }
}

void DisplayScreen(void){
   if (Display.InitDelay == OFF)return;
 //  if(Log_Status == LOG_OFF) 
   UpdateDisplayBuffer(); //update buffer 2 display in normal mode!!
 //  else DisplayFast_Log();
  
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  //display.print(Display_Line1);   //10

  switch(PSR.State){
    case RAMPING_UP: display.print("Ramping"); break;
    case IDLE:       display.print("Idle   ");break;
    case TOPOF_RAMP: display.print("TopRamp");   break;       
    default:         display.print("PhsFail");break;       

  }
  display.print(" Ol "); 
  display.print(PSR.OledCounter); 
  display.print(" Rl ");
  display.print(PSR.RledCounter); 

  


  display.setCursor(0, 8);
  //display.print(Display_Line2);   //10 

    display.print("RUp.");
    display.print(RampUp.Val/10); //5
    display.print(" RSt.");
    display.print(StopRamp.Val/10); //7
    display.print(" V.%");
    display.print(VoltPercent.Val); //6

         
  display.setCursor(0, 16);
    display.print("CoSFi ");
    display.print(MotorCoSφ/100); //5
    display.print(".");
    display.print(MotorCoSφ/10); //5  
     //  display.print(".");  
    display.print(MotorCoSφ%10); //5


  
  display.setCursor(0, 24);




  if(R_Phase.GridEnable_UI)display.print("R");
  else display.print(".");
  if(S_Phase.GridEnable_UI)display.print("S");
    else display.print(".");
  if(T_Phase.GridEnable_UI)display.print("T");
    else display.print("."); 
    display.print("G% ");

    display.print(R_Phase.Grid_DutyCycle); //5
    display.print(' ');
    display.print(S_Phase.Grid_DutyCycle); //5
    display.print(' ');
     display.print(T_Phase.Grid_DutyCycle); //5

  //display.print(Display_Line4);   //10

  display.setCursor(0, 32);
  //display.print(Display_Line5);   //10
  display.print("Sm ");
  if(R_Phase.Thyristor_Sim_Firing ) display.print("R");
  else display.print(".");
  if(S_Phase.Thyristor_Sim_Firing ) display.print("S");
  else display.print(".");
  if(T_Phase.Thyristor_Sim_Firing ) display.print("T");
  else display.print(".");

  display.print(" Ang "); 
  display.print(R_Phase.Thyristor_Sim_Firing_Angle);   
  display.print(" ");
  display.print(S_Phase.Thyristor_Sim_Firing_Angle);   
  display.print(" ");
   display.print(T_Phase.Thyristor_Sim_Firing_Angle);  

  display.setCursor(0, 40);
  display.print("ChpI %");   
  display.print(R_Phase.I_Chopped_DutyCycle);  
  display.print(" ");
  display.print(S_Phase.I_Chopped_DutyCycle);  
  display.print(" ");
  display.print(T_Phase.I_Chopped_DutyCycle);   


  display.setCursor(0, 48);
  display.print("Drp: "); 
  if(R_Phase.Fire_Drop_Enable_UI ) display.print("R");
  else display.print(".");
  if(S_Phase.Fire_Drop_Enable_UI ) display.print("S");
  else display.print(".");
  if(T_Phase.Fire_Drop_Enable_UI ) display.print('T');
  else display.print('.');

  //display.print(Display_Line7);   //10
/*
    UpdateDispSpChar(DispRollIndex[0], 4); // Line 4
    UpdateDispSpChar(DispRollIndex[1], 5); // Line 5
    UpdateDispSpChar(DispRollIndex[2], 6); // Line 6
    UpdateDispSpChar(DispRollIndex[3], 7); // Line 7
  */
  display.setCursor(0, 56); // 8th line
  //display.print(Display_Line8);   //10

  


  /*
       display.print(Key.Adc);
       display.print(" L");display.print(Key.Left_Press);
       display.print(" D");display.print(Key.Down_Press);
       display.print(" R");display.print(Key.Right_Press);
       display.print(" U");display.print(Key.Up_Press);
       display.print(" Rl");display.print(Key.Released);
  */
  /*
      if( DispExpLin8_1){
        DispExpLin8_1 = OFF;
        display.setCursor(60, 56); //x,y  6*10 = 60
        display.write(30); // 5th character  '>';

        display.setCursor(72, 56); //x,y  6*12 = 72
        display.write(31); // 5th character  '<';
      }
  */
  switch (MainMenu) {
    case MENU_NULL :
      //   if(!Key.Error) {
      display.setCursor(60, 56); //x,y  6*10 = 60
      display.write(30); // 5th character  '>';
      display.setCursor(72, 56); //x,y  6*12 = 72
      display.write(31); // 5th character  '<';
      // }
      break;
    case MENU5_SUB1 :
      break;
    default:
      break;
  }
  display.display();
}

bool DisplayFlash(void) {
  Display.Flash++;
  if (Display.Flash > 2) {
    Display.Flash = 0;
    return OFF;
  }
  return ON;
}



void UpdateDisplayMenu(void) {
  String str;
  if (Key.Error) {
    str = CopyFlashToRam(Disp_MENU_ERROR);
    str += String(Key.Adc);
    Display_Line8 = str;
    return;
  }
  switch (MainMenu) {
    case MENU_NULL :
      str = CopyFlashToRam(Disp_MENU_NULL);
      //str = Disp_MENU_NULL_ENT;
      //      str += "   ";
      //      str += CopyFlashToRam(Disp_MENU_NULL_ESC);
      break;
    case MENU1 :  str = CopyFlashToRam(Disp_MENU1);
      break;
    case MENU1_SUB1 :    str = CopyFlashToRam(Disp_MENU1_SUB1);  //str +=CopyFlashToRam(Disp_MENU1_SUB1);
      if (DisplayFlash() == 0) {
        str[4] = ' ';
        str[5] = ' ';
        str[6] = ' ';
        str[7] = ' ';
        str[8] = ' ';
      }
      break;
    case MENU1_SUB2 :     str = CopyFlashToRam(Disp_MENU1_SUB2); //   str +=CopyFlashToRam(Disp_MENU1_SUB2);
      if (DisplayFlash() == 0) {
        str[4] = ' ';
        str[5] = ' ';
        str[6] = ' ';
        str[7] = ' ';
        str[8] = ' ';
      }
      break;
    case MENU1_SUB3 : str = CopyFlashToRam(Disp_MENU1_SUB3) ;
      break;
    case MENU1_SUB4 : str = CopyFlashToRam(Disp_MENU1_SUB4) ;//str +=CopyFlashToRam(Disp_MENU1_SUB4);
      break;
    case MENU2 :   str = CopyFlashToRam(Disp_MENU2);
      break;
     case MENU2_SUB01 : 
     if(SampleTime == TASK_1MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);  
      str += CopyFlashToRam(Disp_MENU2_SUB01);

      break;  
     case MENU2_SUB02 : 
     if(SampleTime == TASK_5MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);  
      str += CopyFlashToRam(Disp_MENU2_SUB02);

      break;  



     case MENU2_SUB03 : 
     if(SampleTime == TASK_10MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);  
      str += CopyFlashToRam(Disp_MENU2_SUB03);

      break;  
     case MENU2_SUB04 : 
     if(SampleTime == TASK_20MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);  
      str += CopyFlashToRam(Disp_MENU2_SUB04);

      break;  
     case MENU2_SUB05 : 
     if(SampleTime == TASK_50MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);  
      str += CopyFlashToRam(Disp_MENU2_SUB05);

      break;     
    case MENU2_SUB06 : 
     if(SampleTime == TASK_100MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);  
      str += CopyFlashToRam(Disp_MENU2_SUB06);

      break;
    case MENU2_SUB07 : //str = CopyFlashToRam(Disp_MENU2_SUB);
      if(SampleTime == TASK_250MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);
      str += CopyFlashToRam(Disp_MENU2_SUB07);
     
    
      break;
    case MENU2_SUB08 :// str = CopyFlashToRam(Disp_MENU2_SUB);
      if(SampleTime == TASK_500MSEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);
      str += CopyFlashToRam(Disp_MENU2_SUB08);
      
      
      break;
    case MENU2_SUB09 : //str = CopyFlashToRam(Disp_MENU2_SUB);
      if(SampleTime == TASK_1SEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);  
      str += CopyFlashToRam(Disp_MENU2_SUB09);
;              

      break;
    case MENU2_SUB10 : //str = CopyFlashToRam(Disp_MENU2_SUB);
      if(SampleTime == TASK_2SEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);   
      str += CopyFlashToRam(Disp_MENU2_SUB10);         


      break;
    case MENU2_SUB11 : //str = CopyFlashToRam(Disp_MENU2_SUB);
      if(SampleTime == TASK_5SEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);
      str += CopyFlashToRam(Disp_MENU2_SUB11);  


      break;   
    case MENU2_SUB12 :// str = CopyFlashToRam(Disp_MENU2_SUB);
     if(SampleTime == TASK_10SEC)str = CopyFlashToRam(Disp_MENU_Activ);
      else str = CopyFlashToRam(Disp_MENU_Enter);   
      str += CopyFlashToRam(Disp_MENU2_SUB12);
      if(SampleTime != TASK_10SEC)       
        if (DisplayFlash() == OFF) {
            str[10] = ' ';str[11] = ' ';
        }       
      break;

      
    case MENU2_SUB13 : str = CopyFlashToRam(Disp_MENU2_SUB13);
      break;
    case MENU2_SUB14 : str = CopyFlashToRam(Disp_MENU2_SUB14);
      break;
      
    case MENU3 :   str = CopyFlashToRam(Disp_MENU3);
      break;
    case MENU3_SUB1 : str = CopyFlashToRam(Disp_MENU3_SUBMAIN) ;
      str += CopyFlashToRam(Disp_MENU3_SUB1);
      if (DisplayFlash() == OFF) {
        str[9] = ' ';
        str[10] = ' ';
        str[11] = ' ';
        str[12] = ' ';
        str[13] = ' ';
        str[14] = ' ';
        str[15] = ' ';
      }
      break;
    case MENU3_SUB2 :  str = CopyFlashToRam(Disp_MENU3_SUBMAIN) ;
      str += CopyFlashToRam(Disp_MENU3_SUB2);
      if (DisplayFlash() == OFF) {
        str[9] = ' ';
        str[10] = ' ';
        str[11] = ' ';
        str[12] = ' ';
        str[13] = ' ';
        str[14] = ' ';
        str[15] = ' ';
      }
      break;
    case MENU3_SUB3 : str = CopyFlashToRam(Disp_MENU3_SUBMAIN) ;
      str += CopyFlashToRam(Disp_MENU3_SUB3);
      break;
    case MENU3_SUB4 : str = CopyFlashToRam(Disp_MENU3_SUBMAIN) ;
      str += CopyFlashToRam(Disp_MENU3_SUB4);
      break;
    case MENU4 :   str = CopyFlashToRam(Disp_MENU4);
      break;
    case MENU4_SUB1 :
      str = "Fw: " + String(__DATE__) + ' ' + String(__TIME__);
      //Fw: mmm dd yyyy hh:mm:ss //24
      //  str.setCharAt(10 , '.');
      str.remove(11, 2); // Remove 2 characters starting at index=11
      str.remove(7, 1); // Remove 1 characters starting at index=7
      break;
    case MENU4_SUB2 :

      break;
    case MENU4_SUB3 :
      str = CopyFlashToRam(Disp_MENU4_SUB3_1); 
        #ifndef STM32_F407
      for (uint8_t i = 0; i < 8; i++){
            if (UniqueID8[i] < 0x10) str += '0';
            str += String(UniqueID8[i], HEX);
            if(i==3)str += '.';
       }
       #endif
      break;
    case MENU4_SUB4 :
      str = CopyFlashToRam(Disp_MENU4_SUB4);
      break;
      
    case MENU5 :   str = CopyFlashToRam(Disp_MENU5);
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
    case MENU5_SUB7 : str = CopyFlashToRam(Disp_MENU5_SUB7);
      break;
    case MENU6 : str = CopyFlashToRam(Disp_MENU6);
      break;
    case MENU6_SUB1 : str = CopyFlashToRam(Disp_MENU6_SUB1);
      break;
    case MENU6_SUB2 : str = CopyFlashToRam(Disp_MENU6_SUB2);
      break;
    case MENU6_SUB3 : str = CopyFlashToRam(Disp_MENU6_SUB3);
      break;
    case MENU6_SUB4 : str = CopyFlashToRam(Disp_MENU6_SUB4);
             #ifdef ENERGYMETER_EXISTS 
          if ( Mains_Frequency == FREQUENCY_50HZ)str += CopyFlashToRam(Disp_MENU6_SUB42); 
          if ( Mains_Frequency == FREQUENCY_60HZ)str += CopyFlashToRam(Disp_MENU6_SUB43);   
           #endif      
      break;
    case MENU6_SUB5 : str = CopyFlashToRam(Disp_MENU6_SUB5);
      break;
    case MENU6_SUB6 : str = CopyFlashToRam(Disp_MENU6_SUB6);
      break;
    case MENU6_SUB7 : str = CopyFlashToRam(Disp_MENU6_SUB7);
     
      break;
    case MENU7 : str = CopyFlashToRam(Disp_MENU7);
      break;

  }
  Display_Line8 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);
}
#endif
