
//#define OLED_128X128
#define OLED_128X64
//#include <RTClib.h>
//#include <Adafruit_Si7021.h>
#include <Wire.h>
#include <SPI.h>
#ifdef OLED_128X64
#include <Adafruit_SSD1306.h>
#endif
#ifdef OLED_128X128
#include <Adafruit_SSD1327.h>
#endif
#define ARM_MATH_CM0PLUS

#define ON 1
#define OFF 0
#define true 1
#define false 0

#define TIMER_INTERVAL_1_MS         1L
#define TIMER_INTERVAL_10_MS         10L
#define TIMER_INTERVAL_20_MS         20L
#define TIMER_INTERVAL_100MS         100L

#define HW_TIMER_INTERVAL_MS      50 //5msec 50*100= 5000  // 10 mSec
//#define HW_TIMER_INTERVAL_MS      5 //5msec 50*100= 5000  // 10 mSec
#define HW_TIMER_INTERVAL_US      50L // 50uSec
#define TIMER_INTERVAL_0_5S           500L
#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_1_5S           1500L


 #define LOGIC_ANALYSER_DEBUG 


//const uint16_t R_VoltIO = PE14;
//const uint16_t S_VoltIO = PE11;
//const uint16_t T_VoltIO= PE9;

   #define RAMPUP_MIN 70
   #define RAMPUP_MAX 969
 
   #define RAMPSTOP_MIN 90
   #define RAMPSTOP_MAX 985

   #define VOLTPERCENT_MIN 70
   #define VOLTPERCENT_MAX 934

#define HERTZ_50 
//#define HERTZ_60 
#if (defined HERTZ_50 && defined HERTZ_60)
    #error SELECT ONLY ONE MAINS FREQ !! // COMPILER ERROR
#endif

//const int PhaseFull = 400; // 10 ms 
//const int P_Shift = 133;//133; //13.33  6.66 ms (10/180)*120 = 

  // PSR Specific IOs
  #define OLED  PF11          // CN12
  #define RLED  PE0           // CN12
  #define ADC_STOP PF4        //  CN12
  #define ADC_VOLTPERCENT PF5 // CN12
  #define ADC_RAMPUP PF10     // CN12
  //#define PSR_START    PG12   //CN10 D7 ->PG12 on STM Board
  #define PSR_START    PG14  //CN10 D2 ->PG12 on STM Board

// PORTB &= ~(1 << 3);  set to 1  bit 3
// PORTB & ~(1 << 3): set to 0bit 3

  #define R_PSR_Thy_FireDet_IO PF3  //CN10  D8  R27 FB_N
  #define S_PSR_Thy_FireDet_IO PD15  //CN10  D9  R26 FA_N
  #define T_PSR_Thy_FireDet_IO PD14   //CN10  D9  R26 FA_N

  #define R_VOLTAGE_CREATE_PIN    PE14  //CN10 D4   ->PE14 on STM Board  FB_N
  #define S_VOLTAGE_CREATE_PIN    PE11  //CN10 D5   -> PE11 on STM Board      
  #define T_VOLTAGE_CREATE_PIN    PE9   //CN10 D6   -> PE9 on STM Board   


 // const uint16_t R_THYRISTOR_VOLTAGE_CREATE_PIN = PD4;
//const uint16_t S_THYRISTOR_VOLTAGE_CREATE_PIN = PC_3;
//const uint16_t T_THYRISTOR_VOLTAGE_CREATE_PIN = (PortC << 4) + 0x02;


 #define TIME_DEBUG_OUT_PIN            PG10 // CN11 Logic Analyser 9

  #define R_PSTX_VOLTAGE_CREATE_PIN    PG12  // CN11 Logic Analyser 0
  #define R_PSTX_DEBUG_THY_FIRING_CREATE_PIN  PG9 // CN11 Logic Analyser 1
  #define R_PSTX_CURRENT_CREATE_PIN    PE1  // CN11 Logic Analyser 2

  #define S_PSTX_VOLTAGE_CREATE_PIN     PG0 // CN11 Logic Analyser 3 
  #define S_PSTX_DEBUG_THY_FIRING_CREATE_PIN  PD0 // CN11 Logic Analyser 4 
        // GND                               // CN11 Logic Analyser GND
  #define S_PSTX_CURRENT_CREATE_PIN     PD1 // CN11 Logic Analyser 5  

  #define T_PSTX_VOLTAGE_CREATE_PIN     PF0 // CN11 Logic Analyser 6
  #define T_PSTX_DEBUG_THY_FIRING_CREATE_PIN  PF1 // CN11 Logic Analyser 7
  #define T_PSTX_CURRENT_CREATE_PIN     PE3 // CN11 Logic Analyser 8

  #define R_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN    PD7  // // CN11   Logic Analyser 10
  #define S_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN    PD6   // // CN11  Logic Analyser 11
  #define T_PSTX_THYRISTOR_VOLTAGE_CREATE_PIN    PD5   // // CN11  Logic Analyser 12

  #define R_PSR_CURRENT_CREATE_PIN    PD4   // // CN11  Logic Analyser 13
  #define S_PSR_CURRENT_CREATE_PIN    PA14   // // CN11   Logic Analyser 15
  #define T_PSR_CURRENT_CREATE_PIN    PH1  // // CN11   Logic Analyser 14

/*
  #define R_CURRENT_CREATE_PIN    PB7  // // CN11  Logic Analyser 13
  #define S_CURRENT_CREATE_PIN    PA15   // // CN11  Logic Analyser 14
  #define T_CURRENT_CREATE_PIN    PA14   // // CN11   Logic Analyser 15

  #define R_DEBUG_VOLTAGE_CREATE_PIN    PG0  // CN11 Logic Analyser 0
  #define R_DEBUG_THY_FIRING_CREATE_PIN PD0  // CN11 Logic Analyser 1
  #define R_DEBUG_CURRENT_CREATE_PIN    PD1  // CN11 Logic Analyser 2

  #define S_DEBUG_VOLTAGE_CREATE_PIN    PF0  // CN11 Logic Analyser 3 
  #define S_DEBUG_THY_FIRING_CREATE_PIN PF1  // CN11 Logic Analyser 4 
        // GND                               // CN11 Logic Analyser GND
  #define S_DEBUG_CURRENT_CREATE_PIN    PE3  // CN11 Logic Analyser 5  

  #define T_DEBUG_VOLTAGE_CREATE_PIN    PD7  // CN11 Logic Analyser 6
  #define T_DEBUG_THY_FIRING_CREATE_PIN PD6  // CN11 Logic Analyser 7
  #define T_DEBUG_CURRENT_CREATE_PIN    PD5  // CN11 Logic Analyser 8
*/






#define PHASE_OFF  0
#define IDLE  1
#define RAMPING_UP  2
#define TOPOF_RAMP 3


  void clearSerialInput(void);


  #define TASK_1MSEC     1
  #define TASK_5MSEC     2   
  #define TASK_10MSEC   4
  #define TASK_20MSEC   8
  #define TASK_50MSEC   16
  #define TASK_100MSEC 32
  #define TASK_250MSEC 64
  #define TASK_500MSEC 127
  #define TASK_1SEC   128
  
  #define TASK_2SEC   130
  #define TASK_5SEC   134
  #define TASK_10SEC 138
  #define TASK_20SEC 140
  #define TASK_60SEC 144

#define ACKNOWLEDGE  0XC3//26 
#define DATA_BYTE    0X55
#define PREAMBLE_BYTES  2 // preamble+length 4 byte + 2 byte
#define DATALENGTH_BYTES  2
#define CRC_BYTES 2
#define DEFAULT_PREAMBLE = 0XAAAA;
#define DEFAULT_CRC_INIT 0XAAAA; // preamble+length 4 byte + 4 byte
#define DEFAULT_TX_LENGTH  32//26 // The whole packet including preamble and CRC
#define DEFAULT_RX_LENGTH  16//////26 // The whole packet including preamble and CRC
 // #define DEBUG_MODE_COMM
// 001BC5067010E312
/*
typedef  byte               uint8;
typedef  signed char        int8;
typedef  unsigned char      uint8;
//typedef  signed short       int16;
//typedef  unsigned short     uint16;
typedef  signed int         int16;
typedef  unsigned int       uint16;
typedef  signed long        int32;
typedef  unsigned long      uint32;
typedef  signed long long   int64;
typedef  unsigned long long uint64

C:\Program Files (x86)\Arduino\libraries

*/
//#define FASTLOG_INIT_TIMER 10

#define DOWNROLL 0
#define UPROLL 1
//#define TEMP_HUM_ONBOARD_SENSOR_EXISTS
//#define TEMP_HUM_1_SENSOR_EXISTS
//#define TEMP_HUM_2_SENSOR_EXISTS
//#define TEMP_HUM_3_SENSOR_EXISTS  

//#define PM25_DUST_SENSOR_EXISTS
//#define SDCARD_EXISTS
#define OLEDDISPLAY_EXISTS
//#define ENERGYMETER_EXISTS 
//#define PROGRELAY_EXISTS 
//#define BATTERY_SCOOTER_EXISTS
//#define  SOLAR_PANEL_EXISTS
//#define LIGHT_SENSOR_EXISTS  
//#define BAR_PRES_SENSOR_EXISTS  
//#define ACCL_GYRO_SENSOR_EXISTS  
//
//#define  LIDAR_EXISTS
//#define WIND_SENSOR_EXISTS  
//#define LEM_CURRENT_EXISTS
//#define VOLTAGE_MEASURE_EXISTS

  #define ANALOG_RES_12BIT
  //#define ANALOG_RES_10BIT


//#define KEY_DIGITAL
#define KEY_ANALOG



#define ARM_MATH_CM0PLUS

/*

#ifdef FIRST_PROTOTYPE
  //  const int chipSelect = 10; // mega SS for SD Card  
  #define KEY_UP 5 // 12//4 //RED
  #define KEY_RIGHT 2//12//2 //
  NN
  #define LED_GREEN 3// 11//3 // GREEN
  #define LED_RED 4 // 12//4 //RED
  #define KEY_MID 5// 11//5 //
  #define KEY_LEFT 6//13//6 // ok
#endif

*/

#if defined (STM32_F407) 
/*
  #define TDI         A7 //9 Atmel ice JTAG  // 0 2 GND
  #define TDO         A6 //3 Atmel ice JTAG  // 4 Vdd -> 5V mega 3V3 Due
  #define TMS         A5 //5 Atmel ice JTAG  // 6 Reset
  #define TCK         A4 //1 Atmel ice JTAG
  */

  #define  KEY_ANALOG_IN   A0 
     
    #if  defined KEY_DIGITAL
  #define KEY_RIGHT       13  gg hh
  #define KEY_DOWN        12
  #define KEY_LEFT        11
  #define KEY_UP          12    
    #endif
   //   const int chipSelect = 10;
   //   const int SD_CS_PINOUT = 10;
      #define SD_CS_PINOUT    10  
#endif


     
  #define TX_OUTPUT_PIN          1 //ON BOARD PROGRAMMING & DEBUG RESERVED
  #define RX_INPUT_PIN           0  //ON BOARD PROGRAMMING & DEBUG RESERVED

  #define OUT_PINOUT 2 // Out pin of the sensor
  #define RV_PINOUT 1 // RV output of the sensor
  #define TMP_PINOUT 0 // analogRead(TMP_PINOUT);

#define MAX_DISPLAY_CHAR 21
#define  MAXSHOWLINE 6  // define how many lines for sensorts to show including fw info line 

#define DISPSHOWLINE4
#define DISPSHOWLINE3
#define DISPSHOWLINE2
#define DISPSHOWLINE1

#define DISPROLL_LINE0 0
#define DISPROLL_LINE1 1
#define DISPROLL_LINE2 2
#define DISPROLL_LINE3 3
#define DISPROLL_LINE4 4
#define DISPROLL_LINE5 5
#define DISPROLL_LINE6 6
#define DISPROLL_LINE7 7
#define DISPROLL_LINE8 8
#define DISPROLL_LINE9 9

#define MENU_NULL 0
#define MENU1   16
#define MENU2   32
#define MENU3   48
#define MENU4   64
#define MENU5   80
#define MENU6   96
#define MENU7   112
#define MENU8   128

#define MENU1_SUB1 17 // +=4  // Log Start
#define MENU1_SUB2 18    // / Log Stop
#define MENU1_SUB3 19 // +=4
#define MENU1_SUB4 20

#define MENU2_SUB01  33  // +=4
#define MENU2_SUB02  34
#define MENU2_SUB03  35
#define MENU2_SUB04  36
#define MENU2_SUB05  37
#define MENU2_SUB06  38
#define MENU2_SUB07  39
#define MENU2_SUB08  40
#define MENU2_SUB09  41
#define MENU2_SUB10  42
#define MENU2_SUB11  43
#define MENU2_SUB12  44
#define MENU2_SUB13  45
#define MENU2_SUB14  46

#define MENU3_SUB1  49 // +=4
#define MENU3_SUB2  50
#define MENU3_SUB3  51 // +=4
#define MENU3_SUB4  52

#define MENU4_SUB1 65
#define MENU4_SUB2 66
#define MENU4_SUB3 67
#define MENU4_SUB4 68

#define MENU5_SUB1 81
#define MENU5_SUB2 82
#define MENU5_SUB3 83
#define MENU5_SUB4 84
#define MENU5_SUB5 85
#define MENU5_SUB6 86
#define MENU5_SUB7 87
#define MENU5_SUB8 88

#define MENU6_SUB1 97
#define MENU6_SUB2 98
#define MENU6_SUB3 99
#define MENU6_SUB4 100
#define MENU6_SUB5 101
#define MENU6_SUB6 102
#define MENU6_SUB7 103

#define MENU7_SUB1 113
#define MENU7_SUB2 114
#define MENU7_SUB3 115
#define MENU7_SUB4 116
#define MENU7_SUB5 117
#define MENU7_SUB6 118
#define MENU7_SUB7 119
#define MENU7_SUB8 120
#define MENU7_SUB9 121

#define MENU8_SUB1 129
#define MENU8_SUB2 130
#define MENU8_SUB3 131

#define KEYDISP_TIMER 40

#define SD_NOT_Present 0
#define SD1_TYPE 1
#define SD2_TYPE 2
#define SDHC_TYPE 3
#define UNKNOWN_TYPE 4

#define LETTER_NUMBER 0
#define NUMBER_ONLY 1

/*

//  Log_Status Phases
#define LOG_OFF 0
#define LOG_START 1
#define LOG_BIN_CREATE 2
#define LOG_BIN_CREATE_POST 3
#define LOG_LOOP_IN_ACTION 4
#define LOG_LOOP_ENDED_SUCCESS 5
#define LOG_LOOP_ENDED_FAIL 8

#define LOG_BIN_CREATE_FAIL 16
#define LOG_RECORD_FAIL     17
#define LOG_2CSV_FAIL       18


 
#define LOG_2CSV_CREATE 6
#define LOG_2CSV_CREATE_POST 7

// Timers 
#define LOG_START_TIMER 3
#define LOG_LOOP_ENDED_SUCCESS_TIMER 3
#define LOG_2CSV_CREATE_POST_TIMER 3
#define LOG_BIN_CREATE_POST_TIMER 3
#define LOG_LOOP_ENDED_FAIL_TIMER 3
*/
/*
#if defined (ARDUINO_MEGA)  & defined (ARDUINO_DUE) 
    #error Select Only One Platform-> ARDUINO_MEGA or ARDUINO_DUE
#endif

 #if !(!defined (ARDUINO_MEGA) ^ !defined (ARDUINO_DUE)) 
    #error Select At Least One Platform -> ARDUINO_MEGA or ARDUINO_DUE
#endif
*/
#if defined (KEY_DIGITAL)  & defined (KEY_ANALOG) 
    #error Select Only One Type -> KEY_DIGITAL or KEY_ANALOG
#endif

 #if !(!defined (KEY_DIGITAL) ^ !defined (KEY_ANALOG)) 
    #error Select At Least One Type -> KEY_DIGITAL or KEY_ANALOG
#endif

// function prototypes
void Common_Loop(); 
void ResetCasePrint();
void IO_Settings();
void MicroInit(void);
void Display_ReInit_Start(uint8_t Timer);
void Display_ReInit_End(void);
/*
void SD_Card_Info(void);
void SD_Card_Init(void);
void SD_Card_Data_Preparation(void);
void SD_Card_Header_Preparation(void);

void RTC_Init();
void SensorInit_Si072(uint8_t);
void SensorAlt_Init();
void SensorLight_Init();
void SensorACccel_GyroInit();
void Sensor_LidarInit(void);
void Sensors_PeripInit();

void CurrentVolt_Read(void);
void AdcRead(void);
void WindSensorRead(void);
void SensorRead_Si072(unsigned char);
void SensorAlt_Read(void);
void SensorLight_Read(void);
void SensorAcccel_GyroRead(void);
void Sensor_LidarRead(void);
void SDS_DustSensor(void);
void UpdateSensorInfo(void);
*/
void UpdateInfoLine();
void UpdateDisplayMenu();
void UpdateSD_LogTime();
void UpdateFileSize();
void ConvertFileSize(uint32_t);// Line3  
void UpdateProperLine(uint8_t Index, uint8_t Line);

void  RTC_TimeClock(void);

void EscMenuKey(void);
void EnterMenuKey(void);
void DownMenuKey(void);
void UpMenuKey(void);
void SetSampling(uint16_t Time);
void DispEnable(bool Enable, uint8_t Timer);
void DispEnable_4SD_Prblm(bool Enable, uint8_t Timer);

void DispExtTimeout(void);
void DisplayMenu(void);
void KeyTimeOutCheck(void);
void SD_CardLogTask(void);
void SD_Log_File(void);
void SD_Info_Only(void);
void DisplayFullSensors(void);
void DisplayTestDevices(void);
void SerialPortRx(void);
void UpdateDispRoll(uint8_t);
void Log_Data_Write_SD(void);

void Parse_FileString(void);
void Relay_loop(void) ;
float GetValue(uint8_t Relay);
String LimitCopyDisplayStr(String str, uint8_t MaxNumber);
void EnergyMeterIC_Operation(void);
void I2_ACK_Reset(void);
/*
void SetResetLog(bool Enable);
void NVRam_Write_LogStatus(bool Mode);
uint8_t NVRam_Read(uint8_t Address);
void NVRam_Write(uint8_t Address, uint8_t Sample); // EE_SAMPLE
void NVRam_Read_Standbye(void);
void NVRam_Write_Standbye(bool Mode);
void NVRam_Read_MainsFreq(void);
void NVRam_Write_MainsFreq(bool Mode);
void NVRam_Write_MaxFileSize(uint8_t Size);
uint8_t NVRam_Read_MaxFileSize(void);

void NVRam_Read_SerNo(void);
void NVRam_Write_SerNo(char* p);
void NVRam_Read_QueNo(void);
void NVRam_Write_QueNo(char* p);
char NVRam_Check_Content(char Content, bool Number);

void UpdateLogFileId(void);
char* CopyFlashToRam(const char* );

void Due_Memory();
void Print_ARM_SPI_Regs(void);

void UpdateLogFileNo(void);
void UpdateLogFileId(void);
void Startup_NV_Ram(void);

void EEProm_Update_FileNo(void);
void EEProm_Update_DevId(void);
void EEProm_Update_Debug(bool);
*/
bool Log_Escape(void);

