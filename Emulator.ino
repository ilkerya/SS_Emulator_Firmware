/*
  Rev   Date         Description
  1.0   18.03.2025   Initial release
*/

#define STM32_F407  // STM32H743ZI2 MCU -> // https://www.st.com/resource/en/data_brief/nucleo-h743zi.pdf   // STM32H743ZI2 MCU ->
                    // from board manager select Nucleo-144 afterwards Nucleo H743ZI2
                    // STM32 MCU boards v2.7.1 working verified board / do not use auoto update for STM32 boards // 2.9.0 definitely doesnt work
                    // https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json  /

// References for MCU itself
// https://www.st.com/en/microcontrollers-microprocessors/stm32h743zi.html#documentation
// for IOs and all other spects refer to -> stm32h743zi.pdf
// DS12110 32-bit Arm® Cortex®-M7 480MHz MCUs, up to 2MB flash, up to 1MB RAM, 46 com. and analog interfaces

// References for Nucleo-144 board
// https://docs.particle.io/reference/device-os/api/introduction/introduction/
// https://dcc-ex.com/reference/hardware/microcontrollers/microcontrollers.html#gsc.tab=0
//https://os.mbed.com/platforms/ST-Nucleo-H743ZI2/

#define SERIAL_RX_BUFFER_SIZE 64
// Change the buffer size to 128 from default 64
// C:\Users\ilker\AppData\Local\Arduino15\packages\STMicroelectronics\hardware\stm32\2.7.1\cores\arduino\HardwareSerial.h
//HardwareSerial Serial1(PB7, PB6); // CN10 // 5V tolerant // D0 D1 <- Arduino D0 Rx / D1 Tx // D0 -> Cable Txd Orange
HardwareSerial VS_Port(PB7, PB6);  //D0/D1 Serial1 port // CN10 // 5V tolerant // D0 D1 <- Arduino D0 Rx / D1 Tx // D0 -> Cable Txd Orange
//HardwareSerial ddd(PB8, PB9); // CN9

// #define PSR_THY_TR1 PG14       // D2
// #define PSR_THY_TR2 PE13       // D3

/*
HardwareSerial Serial2(PD6, PD5); // CN9
HardwareSerial Serial3(PC11, PC10); // CN9
HardwareSerial Serial4(PD0, PD1); // CN9
HardwareSerial Serial5(PD2, PC12); // CN9
HardwareSerial Serial6(PB7, PB6); // CN10
HardwareSerial Serial7(PB7, PB6); // CN10
*/
// TTL-234x-5V
// https://ftdichip.com/products/ttl-234x-5v/
// https://ftdichip.com/wp-content/uploads/2023/07/DS_TTL234X-SERIES-RANGE-OF-CABLES.pdf
// BLACK GND
// ORANGE TxD OUTPUT -> RxD uC
// YELLOW RxD INPUT  -> TxD uC


#include "Defs.h"
#include "Variables.h"
#ifdef STM32_F407  // ARM CortexM3 32 bit STM32H743ZIT6 NUCLEO-144 H743ZI2
#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"
#endif

#include "Prototypes.h"
#include "DAQ.h"
#include "UserInt.h"
#include "Display.h"
#include "Menu.h"
#include "Comm_Func.h"
#include "Emul_Func.h"


STM32Timer ITimer(TIM2);
int STM32_addr = 0;
// Init STM32_ISR_Timer
// Each STM32_ISR_Timer can service 16 different ISR-based timers
STM32_ISR_Timer ISR_Timer;

// Init STM32 timer TIM1
uint16_t DAQ_Counter = 0;
void TimerHandler()  // 50 uS
{
  ISR_Timer.run();
 // digitalWrite(TIME_DEBUG_OUT_PIN, ON); // active time check in interrupt 
//#define PSR_
   #define PSTX_   //

#if (defined PSR_ && defined PSTX_)
    #error SELECT ONLY ONE MODEL !! // COMPILER ERROR
#endif

#if (defined PSR_ || defined PSTX_)  // 2 phase for PSR
 // R Phase
  digitalWrite(R_VOLTAGE_CREATE_PIN, Generate_Grid_Voltage(&R_Phase, R_PHASE_0)); // create grid Voltage
  Detect_Thyristor_Firing_Angle(&R_Phase, R_PSR_Thy_FireDet_IO);   // Detect Fring Angle From PSR or PSTX
  Generate_Simulated_Thyristor_Firing_Angle(&R_Phase,R_PHASE_0);  // Create Simulated Firing Angles with respect to PC UI
  digitalWrite(R_PSR_CURRENT_CREATE_PIN, Generate_Chopped_Current(&R_Phase, R_PHASE_0)); // Creata current signals according to PSTX firing
  Generate_Simulated_Chopped_Current(&R_Phase, R_PHASE_0);  // Create Simulated Current Signals from the simulated Firing Signals
  // S Phase
  digitalWrite(S_VOLTAGE_CREATE_PIN, Generate_Grid_Voltage(&S_Phase, S_PHASE_120));
  Detect_Thyristor_Firing_Angle(&S_Phase, S_PSR_Thy_FireDet_IO);
  Generate_Simulated_Thyristor_Firing_Angle(&S_Phase, S_PHASE_120); 
  digitalWrite(S_PSR_CURRENT_CREATE_PIN, Generate_Chopped_Current(&S_Phase, S_PHASE_120));
  Generate_Simulated_Chopped_Current(&S_Phase, S_PHASE_120);

#endif

#if (defined PSTX_)  // FULL 3 Phase for PSTX
  // T Phase
  digitalWrite(T_VOLTAGE_CREATE_PIN, Generate_Grid_Voltage(&T_Phase, T_PHASE_240));
  Detect_Thyristor_Firing_Angle(&T_Phase, T_PSR_Thy_FireDet_IO);
  Generate_Simulated_Thyristor_Firing_Angle(&T_Phase, T_PHASE_240);  
  digitalWrite(T_PSR_CURRENT_CREATE_PIN, Generate_Chopped_Current(&T_Phase, T_PHASE_240));
  Generate_Simulated_Chopped_Current(&T_Phase, T_PHASE_240);
#endif



#ifdef LOGIC_ANALYSER_DEBUG
 // Sets or Resets the relevant  IO's for the logic Analyser 

  if (R_Phase.IO_Grid_Voltage_Debug) digitalWrite(R_PSTX_VOLTAGE_CREATE_PIN, ON);
  else digitalWrite(R_PSTX_VOLTAGE_CREATE_PIN, OFF);
  if (S_Phase.IO_Grid_Voltage_Debug) digitalWrite(S_PSTX_VOLTAGE_CREATE_PIN, ON);
  else digitalWrite(S_PSTX_VOLTAGE_CREATE_PIN,OFF);
  if (T_Phase.IO_Grid_Voltage_Debug) digitalWrite(T_PSTX_VOLTAGE_CREATE_PIN, ON);
  else digitalWrite(T_PSTX_VOLTAGE_CREATE_PIN, OFF);

  if (R_Phase.IO_Thyristor_Firing_Debug) digitalWrite(R_PSTX_DEBUG_THY_FIRING_CREATE_PIN, ON);
  else digitalWrite(R_PSTX_DEBUG_THY_FIRING_CREATE_PIN, OFF);
  if (S_Phase.IO_Thyristor_Firing_Debug) digitalWrite(S_PSTX_DEBUG_THY_FIRING_CREATE_PIN, ON);
  else digitalWrite(S_PSTX_DEBUG_THY_FIRING_CREATE_PIN, OFF);
  if (T_Phase.IO_Thyristor_Firing_Debug) digitalWrite(T_PSTX_DEBUG_THY_FIRING_CREATE_PIN, ON);
  else digitalWrite(T_PSTX_DEBUG_THY_FIRING_CREATE_PIN, OFF);

  if (R_Phase.IO_Chopped_Current_Debug) digitalWrite(R_PSTX_CURRENT_CREATE_PIN, ON);
  else digitalWrite(R_PSTX_CURRENT_CREATE_PIN, OFF);
  if (S_Phase.IO_Chopped_Current_Debug) digitalWrite(S_PSTX_CURRENT_CREATE_PIN, ON);
  else digitalWrite(S_PSTX_CURRENT_CREATE_PIN, OFF);
  if (T_Phase.IO_Chopped_Current_Debug) digitalWrite(T_PSTX_CURRENT_CREATE_PIN, ON);
  else digitalWrite(T_PSTX_CURRENT_CREATE_PIN, OFF);

  if (R_Phase.IO_Thyristor_Voltage_Debug) digitalWrite(R_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN, ON);
  else digitalWrite(R_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN, OFF);
  if (S_Phase.IO_Thyristor_Voltage_Debug) digitalWrite(S_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN, ON);
  else digitalWrite(S_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN, OFF);
  if (T_Phase.IO_Thyristor_Voltage_Debug) digitalWrite(T_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN, ON);
  else digitalWrite(T_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN, OFF);

  
/*
  digital_io_write(GPIOC, GPIO_PIN_3,1);
  digital_io_write(GPIOC, GPIO_PIN_3,0);
  digital_io_write(GPIOC, GPIO_PIN_2,1);
  digital_io_write(GPIOC, GPIO_PIN_2,0); 
*/
  ////HAL_GPIO_WritePin( ports[x], pinmaps[x], value );

  //  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_2, 1 );
   //  return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) ? 1:0);
/*
     static void MX_GPIO_Init(void){
void HAL_GPIO_Init (GPIO_TypeDef* GPIOx, GPIO_InitTypeDef *GPIO_Init)

HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(led_GPIO_Port, &amp;GPIO_InitStruct);
  }
  */
  
 //  HAL_GPIO_Init (GPIOC, GPIO_MODE_OUTPUT_PP *GPIO_Init);
#endif
  DAQ_Counter++;
  if(DAQ_Counter>400){ //50*400 = 20000 us 20 mssec
    DAQ_Counter = 0;
    DAQ_Send_Data();
  }

  //#ifdef PSR_
  if (UI_Command.StartStop) {
    digitalWrite(PSR_START, LOW);
  } else digitalWrite(PSR_START, HIGH);

  if (digitalRead(OLED)) PSR.Oled_State = 1;
  else PSR.Oled_State = 0;
  if (digitalRead(RLED)) PSR.Rled_State = 1;
  else PSR.Rled_State = 0;
  //#endif

 //  digitalWrite(TIME_DEBUG_OUT_PIN, OFF); // active time check in interrupt 
}


void setup() {
  MicroInit();  //

  Serial.begin(115200);

  VS_Port.begin(115200);

  // uint16_t temp = SPI_FULL_SPEED;
  //    Serial.print("SPI_FULL_SPEED");

  //    Serial.println(temp);
  IO_Settings();
#ifdef OLEDDISPLAY_EXISTS
  DisplaySetPowerIO();
#endif

  ResetCasePrint();

  Serial.print(F("Compiled: "));
  Serial.println(__DATE__ ", " __TIME__ ", " __VERSION__);
  // Serial.println( F("Compiled: ") __DATE__ ", " __TIME__ ", " __VERSION__);
  //Compiled: Jul 21 2020 15:55:39 7.3.0
  //  ShowSerialCode();


#ifdef OLEDDISPLAY_EXISTS
  DisplayInit();
  // #endif
#endif

#ifdef STM32_F407
  Serial.println(STM32_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = "));
  Serial.print(F_CPU / 1000000);
  Serial.println(F(" MHz"));
  // initialize timer1      // Interval in microsecs
  //if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 100, TimerHandler))
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler)) {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  } else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));
  ISR_Timer.setInterval(TIMER_INTERVAL_10_MS, STM32_IntVectorT1);
#endif

    static const char Disp_MENU1_SUB1[] PROGMEM = "LOG START            "; 

    String str = CopyFlashToRam(Disp_MENU1);
 //   str = (char*)CopyFlashArrayToRam(Load_Fan_TorqSpeed);  // (const char*)
 //   Serial.println(str);

   Serial.print("Load_Induction_Motor_TorqSpeed Adress :");
    Serial.println((long)&Load_Induction_Motor_TorqSpeed[0], HEX);
    Serial.print("Loads[0] content (holding Load_Induction_Motor_TorqSpeed Adress:");  
    Serial.println((long)Loads[0], HEX); 
    Serial.print("Load_Induction_Motor_TorqSpeed[0] content ");
    Serial.println(Load_Induction_Motor_TorqSpeed[0]);


    Serial.print("Load_Pump_TorqSpeed Adress :");
    Serial.println((long)&Load_Pump_TorqSpeed[0], HEX);
    Serial.print("Loads[1] content (holding Load_Pump_TorqSpeed Adress:");  
    Serial.println((long)Loads[1], HEX); 
    Serial.print("Load_Pump_TorqSpeed[0] content ");
    Serial.println(Load_Pump_TorqSpeed[0]);



   Serial.print("Load_Conveyor_TorqSpeed Adress :");
    Serial.println((long)&Load_Conveyor_TorqSpeed[0], HEX);
    Serial.print("Loads[2] content (Load_Conveyor_TorqSpeed Load_Fan_TorqSpeed Adress:");  
    Serial.println((long)Loads[2], HEX); 
    Serial.print("Load_Conveyor_TorqSpeed[0] content ");
    Serial.println(Load_Conveyor_TorqSpeed[0]);

   Serial.print("Load_Compressor_TorqSpeed Adress :");
    Serial.println((long)&Load_Compressor_TorqSpeed[0], HEX);
    Serial.print("Loads[3] content (holding Load_Compressor_TorqSpeed Adress:");  
    Serial.println((long)Loads[3], HEX); 
    Serial.print("Load_Compressor_TorqSpeed[0] content ");
    Serial.println(Load_Compressor_TorqSpeed[0]);

    Serial.print("Load_Crusher_TorqSpeed Adress :");
    Serial.println((long)&Load_Crusher_TorqSpeed[0], HEX);
    Serial.print("Loads[4] content (holding Load_Crusher_TorqSpeed Adress:");  
    Serial.println((long)Loads[4], HEX); 
    Serial.print("Load_Crusher_TorqSpeed[0] content ");
    Serial.println(Load_Crusher_TorqSpeed[0]); 

    Serial.print("Load_Fan_TorqSpeed Adress :");
    Serial.println((long)&Load_Fan_TorqSpeed[0], HEX);
    Serial.print("Loads[5] content (holding Load_Fan_TorqSpeed Adress:");  
    Serial.println((long)Loads[5], HEX); 
    Serial.print("Load_Fan_TorqSpeed[0] content ");
    Serial.println(Load_Fan_TorqSpeed[0]);



  //  Serial.println(Load_Fan_TorqSpeed[0]);

  //   Serial.println((long)&Loads[5], HEX);   
 //    Serial.println((long)Loads[5], HEX); 

//     Serial.println(Loads[5], HEX);   
  //  Serial.print(F("&Load_Induction_Motor_TorqSpeed "));Serial.println(Load_Induction_Motor_TorqSpeed)

}
// the loop function runs over and over again forever
void loop() {
  Common_Loop();
}
