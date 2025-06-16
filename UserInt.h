char Dispbuffer[22];  // make sure this is large enough for the largest string it must hold
char* CopyFlashToRam(const char* p){
  //length.Dispbuffer; 
    for (uint8_t i = 0; i < 22; i++) {
      Dispbuffer[i] = 0;
    }
    for (uint8_t i = 0; i < strlen_P(p); i++) {
       char c = pgm_read_byte_near(p + i);
      Dispbuffer[i] = c;
      //Serial.print(c);
    }
   //  Serial.print(F("Dispbuffer : ")); Serial.println(Dispbuffer);
    return Dispbuffer;
}
void UpdateDisplayBuffer(void){  

//    Display_Line1 = Str_Date + "   ";
  //  Display_Line1 +=  Str_Time;
    
   // Display_Line1 =  String(Key.Adc); 

    String str;

 //   if (Log_Serial.Mode && Log_Serial.PrevMode){
        // if PC Mode Enabled 
      str = CopyFlashToRam(MESSAGE_PC_MODE1);
      Display_Line2 = str;
      str = CopyFlashToRam(MESSAGE_PC_MODE2);
      Display_Line3 = str;    
    
    UpdateProperLine(DispRollIndex[0], 4); // Line 4
    UpdateProperLine(DispRollIndex[1], 5); // Line 5
    UpdateProperLine(DispRollIndex[2], 6); // Line 6
    UpdateProperLine(DispRollIndex[3], 7); // Line 7
    
    // DisplayFullSensors();   
    UpdateDisplayMenu(); // Line8    
}
// Flash // RAM   // Usage
// 61438 // 2856  // UpdateFileSize
// 61926 // 2848 // ConvertFileSize with log
// 61898 // 2848 // ConvertFileSize without log
void ConvertFileSize(uint32_t Number){
  /*
  #define MAXDIGIT 10  // max 10 char 9.999.999.999 max 10GB
  #define ARRSIZE 21
    char str[ARRSIZE]; uint8_t i = 0;uint8_t Digit = 1;uint32_t Division=1 ;
    //Serial.print("Number ");Serial.println(Number);
    for( i = 0; i < ARRSIZE; i++)
       str[i] = ' '; //clean all    
              
   for(uint32_t j = Number ; j >= 10; j /= 10){
         Digit++;
         Division *= 10;
   }
    i = MAXDIGIT- Digit; // max 10 char 9.999.999.999 max 10GB 
    for(; Digit != 0; Digit--){
        str[i++] =  (Number /Division) +'0';
        if((Digit == 10) ||  (Digit == 4) || (Digit == 7)){
            str[i++] ='.';
        }
        Number %= Division;
        Division /= 10;
    }
    str[i++] = ' ';   //10 1
    str[i++] = 'B';
    str[i++] = 'y';
    str[i++] = 't';
    str[i++] = 'e';
    str[i++] = 's';  
    for( i = 0; i < ARRSIZE; i++){
      Display_Line3[i] =  str[i];
    }
     // Display_Line3 += " Bytes";
    //  Serial.print("Display_Line3 ");Serial.println(Display_Line3);
   //     Serial.print(str[i]);
   */
}

void UpdateFileSize(){
  /*
    String str;unsigned long Remain;
    str = "    "; //4
    if( FileSize.Total == 0) str += " ";//10
    
      if(FileSize.Total < 100000000)str += ' ';   // 100 Mb     
      if(FileSize.Total < 10000000)str += ' ';   // 10 Mb    
      if(FileSize.Total < 1000000)str += ' ';   // 1 Mb
      if(FileSize.Total < 100000)str += ' ';   // 100 kb
      if(FileSize.Total < 10000)str += ' ';   // 10 kb      
      if(FileSize.Total < 1000)str += ' ';   // 1 kb
      if(FileSize.Total < 100)str += ' ';   // 100 byte
      if(FileSize.Total < 10)str += ' ';   // 10  byte  
      if(FileSize.Total >= 1000000){  // 1 mb   // find out the 1millions dot place and remaining leading zeros
        str += String(FileSize.Total / 1000000);  // 9 digit 
        str +='.';  // 9 digit 
        Remain = FileSize.Total % 1000000;        
        if(Remain < 100000)str += '0'; //  _50000
        if(Remain < 10000)str += '0';  //  __5000
        if(Remain < 1000)str += '0';  //  ___500
      
        if(Remain  >= 1000){
          unsigned long Remain2 = Remain % 1000;
          str += String(Remain / 1000);  // 9 digit 
          str +='.';   // 9 digit
          if(Remain2 < 100)str += '0';
          if(Remain2 < 10)str += '0';
          str += String(Remain2);  // 9 digit       
        }  
      }
      else if(FileSize.Total >= 1000){ // find out the 1000s dot place and remaining leading zeros
        Remain = FileSize.Total % 1000;
        str += String(FileSize.Total / 1000);  // 9 digit 
        str +='.';   // 9 digit 
        if(Remain < 100)str += '0';
        if(Remain < 10)str += '0';
        str += String(Remain);  // 9 digit 
      }
      else str += String(FileSize.Total); // less than 1000 
             
      str += String(" Bytes"); //9 digit  total 15 
        
      Display_Line3 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);
      */
}
    
/*
void Print_Accelometer(float Acc, uint8_t  Type){
                        
    DAQ_Convert_IMU();

      Serial.print(F("Accelometer.x: ")); Serial.println(Accelometer.x);  // 11 1 8 
      Serial.print(F("Final : ")); Serial.print(Accelometer.x_Mantissa);Serial.print('.');  
      if(Accelometer.x_Exponient< 100)Serial.print('0');
      if(Accelometer.x_Exponient< 10)Serial.print('0');     
      Serial.println(Accelometer.x_Exponient); 

      Serial.print(F("Accelometer.y: ")); Serial.println(Accelometer.y);  // 11 1 8 
      Serial.print(F("Final : ")); Serial.print(Accelometer.y_Mantissa);Serial.print('.');  
      if(Accelometer.y_Exponient< 100)Serial.print('0');
      if(Accelometer.y_Exponient< 10)Serial.print('0');     
      Serial.println(Accelometer.y_Exponient);     
      
      Serial.print(F("Accelometer.z: ")); Serial.println(Accelometer.z);  // 11 1 8 
      Serial.print(F("Final : ")); Serial.print(Accelometer.z_Mantissa);Serial.print('.');  
      if(Accelometer.z_Exponient< 100)Serial.print('0');
      if(Accelometer.z_Exponient< 10)Serial.print('0');     
      Serial.println(Accelometer.z_Exponient); 
 
      Serial.println();      
        
}
*/
void PrintDisplayBuffer(void){


  
/*
      Serial.println(Display_Line1);
      Serial.println(Display_Line2);
      Serial.println(Display_Line3);
      Serial.println(Display_Line4);
      Serial.println(Display_Line5);
      Serial.println(Display_Line6);
      Serial.println(Display_Line7);
      Serial.println(Display_Line8);
      Serial.println();
 

    // Additionals
    Serial.print(F("DevId: "));Serial.print(Device_Id);
    
    Serial.print(F(" SensorOnBrd: "));Serial.print(SensorId.OnBoard,HEX);   
    Serial.print(F("    Sensor1: "));Serial.print(SensorId.No1,HEX);
    Serial.print(F("    Sensor2: "));Serial.print(SensorId.No2,HEX);
    Serial.print(F("    Sensor3: "));Serial.print(SensorId.No3,HEX);
    */ 
    //Compiled: Jul 21 2020 15:55:39 7.3.0

    /*
    Serial.print(F("   Version: ")); 
    Serial.println(F(__VERSION__));  // 11 1 8  
  //  Serial.print(F("File: ")); 
  //  Serial.println(F(__FILE__));  // 11 1 8 
 
  //  Serial.print(F("Cpp Standart: ")); 
 //   Serial.print(__cplusplus);  // 11 1      
   // Serial.print(F("Line: ")); 
   // Serial.println(__LINE__);  // 11 1 8   

    Serial.print(F("  DATE:")); 
    Serial.print(__DATE__);  // 11 1 8        
    Serial.print(F("  TIME:")); 
    Serial.println(__TIME__);  // 11 1 8 
    */  
     /*   
    Serial.print(F("Repository: "));  

    for (byte k = 0; k < strlen_P(Repository); k++){
     char c = pgm_read_byte_near(Repository + k);
      Serial.print(c);
    } 
    */
/*
    Serial.print(F("MainMenu No: ")); Serial.println(MainMenu);  // 11 1 8  
    Serial.print(F("Key.Adc: ")); Serial.println(Key.Adc);  // 11 1 8  
    Serial.print(F("Display.OLED_Timer: ")); Serial.println(Display.OLED_Timer);  // 11 1 8  
    Serial.print(F("Log_Status  : ")); Serial.println(Log_Status);  // 11 1 8     
    Serial.print(F("SDCard.LogEnable : ")); Serial.println(SDCard.LogEnable);  // 11 1 8       
    Serial.print(F("SampleTime : ")); Serial.println(SampleTime);  // 11 1 8        
        Serial.println(); 

    */    
    Serial.print(F("Key.Adc : ")); Serial.println(Key.Adc);  // 11 1 8  

  //Print_Accelometer(Accelometer.x, 0);

    

/*
    int result = system(NULL);  
    Serial.print(F("int result = system(NULL): ")); 
    Serial.println(result);  // 11 1 8   
  */      
  //  Serial.println();         
    Serial.println();   
    
//61164
//2890
    
  /*  
     Serial.print(F("GNUC: ")); 
    Serial.println(__GNUC__ );  
     Serial.print(F("GNUC_MINOR: ")); 
    Serial.println(__GNUC_MINOR__ );  
     Serial.print(F("GNUC_PATCHLEVEL: ")); 
    Serial.println(__GNUC_PATCHLEVEL__  );  
*/
   // Serial.print(F("RELAY 1:"));Serial.println(digitalRead(RELAY_OUT_1));
   // Serial.print(F("RELAY 2:"));Serial.println(digitalRead(RELAY_OUT_2));   
     
  //  Serial.println();
  //  Serial.println();     
}
 /*
  String s = "geeksforgeeks";   
    int n = s.length();  
    // declaring character array 
    char char_array[n + 1]; 
    // copying the contents of the 
    // string to char array 
    strcpy(char_array, s.c_str()); 
*/

#define MAXNOCHAR 4
void UpdateProperLine(uint8_t Index, uint8_t Line){
    #define TEMPERATURE_DEGREE_ASCII 247  // FOR OLED DISPLAY https://en.wikipedia.org/wiki/Code_page_437
    String CelsiusDegree = "";  
    CelsiusDegree = (char)TEMPERATURE_DEGREE_ASCII;
    CelsiusDegree +=  "C";
    
    String str = String(Index)+ ".";    
    switch(Index){
      case DISPROLL_LINE0: 
            str = "";//// show nothing                         
      break; 
        case DISPROLL_LINE1:  
         
              #ifdef VOLTAGE_MEASURE_EXISTS
               // str += " " + String(Mains_Volt) + "V ";  
                str += " " + String(Mains_Volt) + "V ";  
             #endif
             #ifdef LEM_CURRENT_EXISTS               
                str += String(Current_Mains_Rms) + "A ";  
            #endif

      break;        
      case DISPROLL_LINE2:          

                
     break;
     case DISPROLL_LINE3:
           
   
     break;
     case DISPROLL_LINE4:   
                  
     break;
     case DISPROLL_LINE5:   
                //str += ICERROR;
              //   str += CopyFlashToRam(ICERROR);
       
         
     break;      
     case DISPROLL_LINE6:       
   
                               
     break; 
     case DISPROLL_LINE7:
            
     break;  
     case DISPROLL_LINE8:    
       
      break;  
      case DISPROLL_LINE9:       
  
      break;                
      default: str = F("default");
      break; 
    }
    uint16_t Length = str.length();
    if(Length > MAX_DISPLAY_CHAR){ // 34 > 21 //Limit the string to display size
      str.remove(MAX_DISPLAY_CHAR, (Length - MAX_DISPLAY_CHAR));// 21, (34-21) 13
      //str =  String(Index) +  "..." + String(Line);
      //str +=  F(".error");
    }
    if(Length < MAX_DISPLAY_CHAR){// Fill the string to reach full 
      for(uint16_t i = 0; i < (MAX_DISPLAY_CHAR - Length);i++){
        str.setCharAt(Length+i, 'x');
      }    
    }
    
    switch(Line){
      case 4:    
          Display_Line4 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);                  
      break; 
      case 5:    
          Display_Line5 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);             
      break;
      case 6:   
          Display_Line6 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);
      break;
      case 7:
          Display_Line7 = str;   
      break;
      default:              // 
          Display_Line4 = "";Display_Line5 = "";Display_Line6 = "";Display_Line7 = "";
      break;
    }        
}
/*
#ifdef SDCARD_EXISTS

void UpdateSD_LogTime(){
    String str;
    if(SDCard.PauseTimer){
      if(SDCard.Status == SD_NOT_Present)str = F("SD Error!  Retry-> "); //18
      if(SDCard.FatError == ON)          str = F("FAT Error! Retry-> "); //18      
      str += String(SDCard.PauseTimer);//13
      Display_Line2 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR); 
      return;     
  }
     if(SDCard.Status != SD_NOT_Present){
    //if((SDCard.Status != SD_NOT_Present) || (SDCard.Status != SD_NO_FAT)){
         switch(Display.ValueTimer){
            case 0:
            case 2:
            case 4:       
                if(SDCard.Status == SD1_TYPE)      str = F("SD1 ");         
                else if(SDCard.Status == SD2_TYPE) str = F("SD2 ");           
                else if(SDCard.Status == SDHC_TYPE)str = F("SDH ");
                str += String(SDCard.Volume);   // 4+5+3
                str +=  F("Gb ") ;   // 4+5+3
                
         break;
          case 1:
          case 3: 
                str = String(LOG_FILE) ; // 12
         break;
   //   case 4:
   //      break;
         default:
         break;  
    }           
      str += "   "; // 3   

      switch(SampleTime){
       case TASK_1MSEC:str += CopyFlashToRam(LOG_1MSEC); //5 
          break; 
       case TASK_5MSEC:str += CopyFlashToRam(LOG_5MSEC); //5 
          break;     
       case TASK_10MSEC:str += CopyFlashToRam(LOG_10MSEC); //5 
          break; 
       case TASK_20MSEC:str += CopyFlashToRam(LOG_20MSEC); //5 
          break;       
       case TASK_50MSEC:str += CopyFlashToRam(LOG_50MSEC); //5 
          break; 
       case TASK_100MSEC:str += CopyFlashToRam(LOG_100MSEC); //5 
          break;           
       case TASK_250MSEC:str += CopyFlashToRam(LOG_250MSEC); //5 
          break;          
       case TASK_500MSEC:str += CopyFlashToRam(LOG_500MSEC); //5 
          break;
          default:        
        case TASK_1SEC : str += CopyFlashToRam(LOG_1SEC);//5
          break; 
        case TASK_2SEC : str += CopyFlashToRam(LOG_2SEC);
          break;        
        case TASK_5SEC : str += CopyFlashToRam(LOG_5SEC);
          break;  
        case TASK_10SEC :str += CopyFlashToRam(LOG_10SEC);
          break; 
        case TASK_20SEC :str += CopyFlashToRam(LOG_20SEC);
          break;            
        case TASK_60SEC :str += CopyFlashToRam(LOG_60SEC);
          break;     
      } 
  
    }

    else str = F("SD Error             ");
    Display_Line2 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR); 
}
#endif
*/
