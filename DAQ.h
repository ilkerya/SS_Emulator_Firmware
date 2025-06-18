

struct DAQ{
  uint8_t Busy;
  uint8_t SendEnable; // SeriPort.SendEnable
  uint8_t ContBytes;
  uint16_t Length;
  uint8_t SendBuf[DEFAULT_TX_LENGTH];
  uint16_t CRC_Send;
  uint16_t CRC_Receive;
  uint16_t CRC_Calc_Receive;
  bool Ack;
  //uint16_t Inc_Encoder = 150;
  uint16_t Current = 4 ; 
  uint16_t Voltage = 24;
  uint16_t Power = 0; 
  float Temperature = 25;
}DAQ;

void DAQ_Convert_IMU(void){
/*
      int32_t Temp;  
      Accelometer.x_Mantissa = (int8_t) Accelometer.x; 
      Temp = (int32_t)(1000 * Accelometer.x);    
      Temp %=  1000;
      Temp = abs(Temp);
      Accelometer.x_Exponient = (int16_t)Temp;

      Accelometer.y_Mantissa = (int8_t) Accelometer.y; 
      Temp = (int32_t)(1000 * Accelometer.y);    
      Temp %=  1000;
      Temp = abs(Temp);
      Accelometer.y_Exponient = (int16_t)Temp;

      Accelometer.z_Mantissa = (int8_t) Accelometer.z; 
      Temp = (int32_t)(1000 * Accelometer.z);    
      Temp %=  1000;
      Temp = abs(Temp);
      Accelometer.z_Exponient = (int16_t)Temp; 
        p[12] = (int8_t)Accelometer.x_Mantissa;
  p[13] = Accelometer.x_Exponient >> 8;
  p[14] = Accelometer.x_Exponient;

  p[15] = (int8_t)Accelometer.y_Mantissa;
  p[16] = Accelometer.y_Exponient >> 8;
  p[17] = Accelometer.y_Exponient; 

  p[18] = (int8_t)Accelometer.z_Mantissa;
  p[19] = Accelometer.z_Exponient >> 8;
  p[20] = Accelometer.z_Exponient;
  */
}
  int32_t GetMod_Float(float Acc){
   if(Acc > 128)  return 1280000;
   if(Acc < -128) return -1280000;
   return (int32_t)(Acc*10000)  ;
  }

void DAQ_Send_Data(void){

  uint8_t *p;
  p = &DAQ.SendBuf[0];

  uint8_t i = 0; //Uint32 CRC = 0;
     
  p[0] = 0XAA; // preamble D170
  p[1] = 0XAA; // preamble D170
  //p[2] = 0XAA; // preamble D170
  //p[3] = 0XAA; // preamble D170 

  DAQ.Length = DEFAULT_TX_LENGTH;
  p[2] = DAQ.Length >> 8;
  p[3] = DAQ.Length; //  packet total size
/*
  if(DAQ.Ack) p[4] = ACKNOWLEDGE;
  else        p[4] = DATA_BYTE;
  DAQ.Ack = OFF;
*/
 p[4] = DATA_BYTE;


/*
  p[6] = Values.Bat_Current_Adc >> 8;
  p[7] = Values.Bat_Current_Adc;
*/
 // p[4] = RX_UI_ARR[0];
 // p[5] = RX_UI_ARR[1];

  //p[6] = RX_UI_ARR[2];
  //p[7] = RX_UI_ARR[3];

    long  randNumber = random(23, 27);
    DAQ.Voltage =  (uint16_t)randNumber; 

    randNumber = random(3, 5);
    DAQ.Current =  (uint16_t)randNumber; 

 //DAQ.Power = DAQ.Voltage * DAQ.Current;
   // DAQ.Voltage++;
  //  if(DAQ.Voltage > 55)DAQ.Voltage = 0;
 //   DAQ.Current--;
 //   if(DAQ.Current <= 1)DAQ.Current = 16;
 //   DAQ.Power = DAQ.Voltage * DAQ.Current;
 // p[8] = DAQ.Voltage >> 8;
  // p[9] = DAQ.Voltage;

  uint8_t temp = 0;
   if(PSR.Oled_State ) temp |= 0B00000001;
   if(PSR.Rled_State ) temp |= 0B00000010;
   temp  |=  (PSR.State << 2);
  p[5] = temp;

  p[6] = R_Phase.Thyristor_Firing_Angle>> 8; //11 di
  p[7] = R_Phase.Thyristor_Firing_Angle;

  p[8] = S_Phase.Thyristor_Firing_Angle>> 8;
  p[9] = S_Phase.Thyristor_Firing_Angle;
  
  p[10] = T_Phase.Thyristor_Firing_Angle>> 8;
  p[11] = T_Phase.Thyristor_Firing_Angle;

  p[12] = RampUp.Val;
  p[13] = StopRamp.Val; 
  p[14] = VoltPercent.Val;


  p[18] = temp;
  p[19] = temp;
  p[20] = temp;
  //p[17] = Oled_Pos;
  //p[18] = Rled_Pos;


/*
  p[19] = RampStop>> 8;
  p[20] = RampStop;

  p[21] = VoltPercent>> 8;
  p[22] = VoltPercent;

  p[23] = RampUp>> 8;
  p[24] = RampUp;
    
*/
    randNumber = random(25, 35);
    DAQ.Temperature = (float)randNumber; 
    randNumber = random(0, 9);
    float Tempf = (float)randNumber;
    DAQ.Temperature += (Tempf/10); 
      
/*
  int32_t Temp = (int32_t)roundf(DAQ.Temperature *100);
  p[14] = (uint8_t)(Temp >> 24); 
  p[15] = (uint8_t)(Temp >> 16); 
  p[16] = (uint8_t)(Temp >> 8); 
  p[17] = Temp ;
*/

  /*
   SendAcc_Data = GetMod_Float(Accelometer.z);

  p[20] = SendAcc_Data >> 24;
  p[21] = SendAcc_Data >> 16;
  p[22] = SendAcc_Data >> 8;
  p[23] = SendAcc_Data;

  uint8_t* p2;
  p2 =(uint8_t*) &Accelometer.x ;
  p[24] = *p2;
  p[25] = *p2++;
  p[26] = *p2++;
  p[27] = *p2++;//18
  p2 =(uint8_t*) &Accelometer.y ;
  p[28] = p2[0];
  p[28] = p2[1];
  p[30] = p2[2];
  p[31] = p2[3];//18
  p2 =(uint8_t*) &Accelometer.z ;
  p[32] = p2[0];
  p[33] = p2[1];
  p[34] = p2[2];
  p[35] = p2[3];//18

  */
  DAQ.CRC_Send = DEFAULT_CRC_INIT;//2-14
  for(i=PREAMBLE_BYTES; i< (DEFAULT_TX_LENGTH-CRC_BYTES); i++){ // crc haric
    DAQ.CRC_Send ^= p[i];
  }
  
  DAQ.CRC_Send <<= 8;
  for(i=PREAMBLE_BYTES; i< (DEFAULT_TX_LENGTH-CRC_BYTES); i++){ // crc haric
    DAQ.CRC_Send ^= p[i];
  }

  
  p[DEFAULT_TX_LENGTH-2] = DAQ.CRC_Send >> 8;// DEFAULT_LENGTH-2
  p[DEFAULT_TX_LENGTH-1] = DAQ.CRC_Send; // DEFAULT_LENGTH-1

    /*
        Serial.print("  p[16]"); Serial.print(p[20]); 
        Serial.print("  p[17]"); Serial.print(p[21]);    
  //    int16_t Acc = (int16_t)p[16]*256;
  //    Acc += (int16_t)p[17];
      int16_t Acc = p[16]*256;
      Acc += p[17];
       Serial.print(" Acc "); Serial.print(Acc); 
       Serial.print(" SendAcc_Data "); Serial.print(SendAcc_Data);        
       Serial.print(" Accelometer.z "); Serial.print(Accelometer.z);             
       Serial.println();    
       */         

  VS_Port.write(p,DEFAULT_TX_LENGTH); 
}

 void SerialPortRx_UI() {
      uint8_t i = 0 ;
      uint8_t j = 0;            uint8_t temp;
      //while (Serial1.available()) {
    //    Serial.println("Serial");    
       if (VS_Port.available() > 0) {
          #ifdef DEBUG_MODE
        Serial.println();
         #endif
/*         
        for(j = 0; j < PREAMBLE_BYTES ; j++) {
          RX_UI_ARR[j] =  VS_Port.read();
        }
*/
         RX_UI_ARR[0] =  VS_Port.read();
         RX_UI_ARR[1] =  VS_Port.read();         
         if((RX_UI_ARR[0] != 0xAA) || (RX_UI_ARR[1] != 0xAA)){
            Serial.print(F("Preamble Mismatch "));
          //  uint8_t temp;
            for(j = 0; j < DEFAULT_RX_LENGTH ; j++) {
              temp =  VS_Port.read();
            }
            VS_Port.flush();
            return;
        }        
            #ifdef DEBUG_MODE
        Serial.print(RX_UI_ARR[0],HEX);Serial.print("."); 
        Serial.print(RX_UI_ARR[1],HEX);Serial.print("."); 
        for(j = PREAMBLE_BYTES; j < DEFAULT_RX_LENGTH ; j++) {
          RX_UI_ARR[j] =  VS_Port.read();
          Serial.print(RX_UI_ARR[j],HEX);Serial.print(".");  
        }
                   #endif  


         uint16_t CrcRx_Calc =  RX_UI_ARR[DEFAULT_RX_LENGTH-2]<<8;      
        CrcRx_Calc += RX_UI_ARR[DEFAULT_RX_LENGTH-1];
    //    Serial.print(F("."));Serial.print(CrcRx_Calc,HEX);

        DAQ.CRC_Calc_Receive = DEFAULT_CRC_INIT;//2-14
        for(i=PREAMBLE_BYTES; i< (DEFAULT_TX_LENGTH-CRC_BYTES); i++){ // crc haric 2-62 0-63  62 63 CRC 
            DAQ.CRC_Calc_Receive ^= RX_UI_ARR[i];
        }
        DAQ.CRC_Calc_Receive <<= 8;
        for(i=PREAMBLE_BYTES; i< (DEFAULT_TX_LENGTH-CRC_BYTES); i++){ // crc haric
          DAQ.CRC_Calc_Receive ^= RX_UI_ARR[i];
        }
     //   Serial.print(F(" CRC "));Serial.print(DAQ.CRC_Calc_Receive,HEX);

        if(DAQ.CRC_Calc_Receive != CrcRx_Calc) {
            Serial.println(F("CRC Mismatch "));
            VS_Port.flush();

            for(j = 0; j < 64 ; j++) {
              temp =  VS_Port.read(); 
            }
               return; 
        }
        DAQ.Ack = ON; 
        uint8_t temp;
          temp = RX_UI_ARR[5];  /// RST Trigger
          if(0B00000001 & temp) R_Phase.GridEnable_UI = 1;
          else R_Phase.GridEnable_UI = 0;
          if((0B00000010 & temp)) S_Phase.GridEnable_UI = 1;
          else S_Phase.GridEnable_UI = 0;     
          if((0B00000100 & temp)) T_Phase.GridEnable_UI = 1;  
           else T_Phase.GridEnable_UI = 0;  
          if((0B00001000 & temp)) UI_Command.StartStop = 1;  
           else UI_Command.StartStop = 0;  
          if((0B00010000 & temp)) R_Phase.Thyristor_Sim_Firing = true;  
          else R_Phase.Thyristor_Sim_Firing = false;       
          if((0B00100000 & temp)) R_Phase.Fire_Drop_Enable_UI = true;  
          else R_Phase.Fire_Drop_Enable_UI = false;  


          S_Phase.Fire_Drop_Enable_UI = R_Phase.Fire_Drop_Enable_UI; 
          T_Phase.Fire_Drop_Enable_UI = R_Phase.Fire_Drop_Enable_UI; 

          S_Phase.Thyristor_Sim_Firing = R_Phase.Thyristor_Sim_Firing; 
          T_Phase.Thyristor_Sim_Firing = R_Phase.Thyristor_Sim_Firing;           



/*
          R_VoltTrig = 0B00000001 & temp;
          S_VoltTrig = 0B00000010 & temp;
          T_VoltTrig = 0B00000100 & temp; 
          */                  
          R_Phase.Grid_DutyCycle = RX_UI_ARR[6];   // Duty_Cycle
          S_Phase.Grid_DutyCycle = R_Phase.Grid_DutyCycle;
          T_Phase.Grid_DutyCycle = R_Phase.Grid_DutyCycle;

          R_Phase.I_Chopped_DutyCycle = RX_UI_ARR[7];
          S_Phase.I_Chopped_DutyCycle = R_Phase.I_Chopped_DutyCycle;
          T_Phase.I_Chopped_DutyCycle = R_Phase.I_Chopped_DutyCycle;

          MotorCoSφ = RX_UI_ARR[8];

          R_Phase.Thyristor_Sim_Firing_Angle = RX_UI_ARR[9];
          S_Phase.Thyristor_Sim_Firing_Angle = R_Phase.Thyristor_Sim_Firing_Angle;
          T_Phase.Thyristor_Sim_Firing_Angle = R_Phase.Thyristor_Sim_Firing_Angle;

                     #ifdef DEBUG_MODE          
              Serial.println();
              Serial.print("Grid R S T ");Serial.print(R_Phase.GridEnable_UI);
              Serial.print("  Sim:");Serial.print(R_Phase.Thyristor_Sim_Firing);     
              Serial.print(" Drop:");Serial.print(R_Phase.Fire_Drop_Enable_UI);  
              Serial.print(" GridDC:");Serial.print(R_Phase.Grid_DutyCycle);  
              Serial.print(" ChopDC:");Serial.print(R_Phase.I_Chopped_DutyCycle);  

              //Serial.print(" ");Serial.print(RX_UI_ARR[6]);
              if(UI_Command.StartStop) Serial.print(" Start");
              else {           
                Serial.print(" Stop");
              }

            #endif
       }
  /*     
       Serial.print(F("Preamble: "));Serial.print(RX_UI_ARR[0],HEX);
     //     Serial1.flush();
        Serial.print(F("."));Serial.print(RX_UI_ARR[1],HEX);    
       uint16_t Length =  RX_UI_ARR[2]* 255;
       Length += RX_UI_ARR[3];
        Serial.print(F(" Length: "));Serial.print(Length);
*/
      //RX_UI_ARR[DEFAULT_RX_LENGTH-1] = 0x1A;
        //Serial.print(F("3 : "));Serial.println(RX_UI_ARR[3]);
    //    Serial.print(F(" 4: "));Serial.print(RX_UI_ARR[4]);   
    //    Serial.print(F(" 5: "));Serial.print(RX_UI_ARR[5]);  
/*
        Serial.print(F(" DEFAULT_RX_LENGTH-3: "));Serial.print(RX_UI_ARR[DEFAULT_RX_LENGTH-3],HEX); 
        Serial.print(F(" DEFAULT_RX_LENGTH-2: "));Serial.print(RX_UI_ARR[DEFAULT_RX_LENGTH-2],HEX);   
        Serial.print(F(" DEFAULT_RX_LENGTH-1: "));Serial.print(RX_UI_ARR[DEFAULT_RX_LENGTH-1],HEX);    
        Serial.print(F(" DEFAULT_RX_LENGTH: "));Serial.print(RX_UI_ARR[DEFAULT_RX_LENGTH],HEX);   
 

        uint8_t Crc =  RX_UI_ARR[DEFAULT_RX_LENGTH-2];
         Serial.print(F(" CRC Rec: "));Serial.print(Crc,HEX);
         
        Crc = RX_UI_ARR[DEFAULT_RX_LENGTH-1];
         Serial.print(F("."));Serial.print(Crc,HEX);

        //uint16_t Crc =  RX_UI_ARR[DEFAULT_RX_LENGTH-1];
        //Crc += RX_UI_ARR[DEFAULT_RX_LENGTH-2]*0x00FF;

       

        /// Calculate CRC Received

      DAQ.CRC_Calc_Receive = DEFAULT_CRC_INIT;//2-14
      for(i=PREAMBLE_BYTES; i< (DEFAULT_TX_LENGTH-CRC_BYTES); i++){ // crc haric 2-62 0-63  62 63 CRC 
        DAQ.CRC_Calc_Receive ^= RX_UI_ARR[i];
      }
  
      DAQ.CRC_Calc_Receive <<= 8;
      for(i=PREAMBLE_BYTES; i< (DEFAULT_TX_LENGTH-CRC_BYTES); i++){ // crc haric
        DAQ.CRC_Calc_Receive ^= RX_UI_ARR[i];
      }

  
        Serial.print(F(" CRC Calc: "));Serial.print(DAQ.CRC_Calc_Receive,HEX);
       */             
      //  Serial.print(F(" j : "));Serial.print(j);
     
      //else return;
     // if(RX_UI_ARR[0]) == 'A';
       // int i = 0;
        /*
           while (Serial1.available() > 0 ) {
                
                i++;
           }
           */
    /*       
        for(int j = 0; j < DEFAULT_RX_LENGTH; j++) {
            RX_UI_ARR[j] = Serial1.read();
            
            Serial.print(j);Serial.print(F(": ")); Serial.print(RX_UI_ARR[j]);Serial.print("  ");
            if((j==20) || (j==40) || (j==60) || (j==80)|| (j==100)|| (j==120)|| (j==140)){
                Serial.println();
            }
            
            } 
            Serial.print(F("Preamble: ")); Serial.print(RX_UI_ARR[0]);
            Serial.print(F("."));Serial.print(RX_UI_ARR[1]);
            Serial.print(F("."));Serial.print(RX_UI_ARR[2]);
            Serial.print(F("."));Serial.print(RX_UI_ARR[3]);
            Serial.print(F("CRC: ")); Serial.print(RX_UI_ARR[140]);
            Serial.print(F("."));Serial.print(RX_UI_ARR[141]);
       
       Serial.println();     
*/
    
 }

/*
 
  uint8_t* p2 =(uint8_t*) &Accelometer.x ;
  p[12] = p2[0];
  p[13] = p2[1];
  p[14] = p2[2];
  p[15] = p2[3];//18
  
  p2 =(uint8_t*) &Accelometer.y ;
  p[16] = *(p2+3);
  p[17] = *(p2+2);
  p[18] = *(p2+1);
  p[19] = *p2;    //22  

  p[16] = 1;
  p[17] = 2;
  p[18] = 3;
  p[19] = 4;    //22  
  
 // p2 =(uint8_t*) &Accelometer.z ;
  float float_try = 32.00;
   p2 =(uint8_t*) &float_try ;
  p[20] = p2[3];
  p[21] = p2[2];
  p[22] = p2[1];
  p[23] = p2[0];   //26
  */
  
