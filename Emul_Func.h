

// Create  Grid Voltages based on the provided phase shift and Voltage duty_cycle 
//  Voltage Duty Cycle is used for a variable Grid Voltage "Vrms",
// Create the required Thyristor Zero Cross signal for the next function  

bool Generate_Grid_Voltage(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift){
   if(!(p->GridEnable_UI)){
    p->IO_Voltage = OFF;
    return OFF;
   }

 uint8_t Grid_Time_DutyCycle = (p->Grid_DutyCycle*2) ; // value ,ultiplied by 5 * 2 = 10  %100 200 %80 -> 
 if(Grid_Time_DutyCycle < 10) Grid_Time_DutyCycle= 10; // min %10  
  uint16_t Grid_Init = PhaseShift ;
  uint16_t Grid_MaxPhase = Half_Cycle_Time + PhaseShift ; // Max time 10 Msec // cut before phase cross
  uint16_t Grid_Active = Grid_Time_DutyCycle + PhaseShift ;

  p->Grid_Counter++;
  if(p->Grid_Counter  <= Grid_Active){ // in the phase counting takingg into account of duty cycle.
        if(p->IO_Voltage  == 0) { // Voltage zero cross check, if IO State was low, zero cross should occured since we set it to high.
          p->V_ZeroCross = ON; // 0->1 low 2 high state //Voltage Zero Cross Occured  Thyristor Trig Enabled.
          p->Thy_Det_Fire_Angle_Count = 0; // / this must 0 since it syncronizes with the SoftStarter Firing in that case working with Offset values
          p->Thy_Sim_Fire_Angle_Count = PhaseShift; // this must Phaseshift since Sim counter is based on discrete values 
        }
       p->IO_Voltage  = ON;   
  }
  else{   // checking if full duty cycle on timeout and cycle end timeout occured 
      if(p->Grid_Counter >= Grid_MaxPhase) p->Grid_Counter  = Grid_Init;// Full Period Timeout Start Again %100
      p->IO_Voltage = OFF;      
  }
  if(p->IO_Voltage) p->IO_Grid_Voltage_Debug = ON; // for debugging purpose only
  else p->IO_Grid_Voltage_Debug = OFF;                      // when thy fired an IO follows in respect
  return p->IO_Voltage ;
}
  // Find out final Thyristor Firing  Angle // timing values in this function are relative not based on Grid or current phase shifts
 
 void  Generate_Simulated_Thyristor_Firing_Angle(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift){

 // p->Thyristor_Firing_Angle = p->Thyristor_Sim_Firing_Angle;
  uint16_t Fire_Init = PhaseShift ;
  uint16_t Fire_MaxPhase = Half_Cycle_Time + PhaseShift - 10 ; // Max time 10 Msec // cut before phase cross
  uint16_t Fire_Passive = p->Thyristor_Sim_Firing_Angle +  PhaseShift ;

  p->Thy_Sim_Fire_Angle_Count++;
  if(p->Thy_Sim_Fire_Angle_Count >= Fire_MaxPhase ){ 
    p->Thy_Sim_Fire_Angle_Count  = Fire_Init;// Full Period Timeout Start Again %100
    p->IO_Thyristor_Firing_Debug = OFF; 

   // if(p->Fire_Drop_Enable_UI)  p->Thy_Fire_Drop_Shift = !p->Thy_Fire_Drop_Shift;
   // else p->Thy_Fire_Drop_Shift = OFF;
  }
  else {
    if(p->Thy_Sim_Fire_Angle_Count <=  Fire_Passive){          
        p->IO_Thyristor_Firing_Debug = OFF;
        p->Thyristor_Firing_Duration = 0;
      }
    else //if(!p->Thy_Fire_Drop_Shift) {
        p->Thyristor_Firing_Duration++;
        if(p->Thyristor_Firing_Duration< 50){ // create ac signal for thyristor gate 
         // if((p->Thyristor_Firing_Duration % 2) && !p->Thy_Fire_Drop_Shift) p->IO_Thyristor_Firing_Debug = ON; //  p->Thyristor_Sim_Firing_Angle       
           if(p->Thyristor_Firing_Duration % 2) p->IO_Thyristor_Firing_Debug = ON; //  p->Thyristor_Sim_Firing_Angle  
           /* p->Thyristor_Firing_Frequency++;
            if(p->Thyristor_Firing_Frequency > 5){
              p->Thyristor_Firing_Frequency = 0;
              p->IO_Thyristor_Firing_Debug = !p->IO_Thyristor_Firing_Debug;
            }
            */
            else p->IO_Thyristor_Firing_Debug = OFF;           
        }
        else p->IO_Thyristor_Firing_Debug = OFF; 
    }
 }
  void  Detect_Thyristor_Firing_Angle(struct Grid_Thy_I_Chopped_Var *p, const uint16_t IO_ThyristorFire ){            
  if(p->V_ZeroCross){ // 
    p->Thy_Det_Fire_Angle_Count++;
    if(p->Thy_Det_Fire_Angle_Count >= Half_Cycle_Time ){ // for debugging 90 radian trigger  100 5 ms can be assigned 
         p->V_ZeroCross = OFF;
          p->Thyristor_Firing_Angle = Half_Cycle_Time; // timeout for fire angle cycle ended 
        }
        if(digitalRead(IO_ThyristorFire) && (p->Thy_Prev_IO_State == 0)){ // fired calculate time,  check if Low 2 High Occured
          p->V_ZeroCross = OFF;
          p->Thyristor_Firing_Angle = Half_Cycle_Time -  p->Thy_Det_Fire_Angle_Count ; // save the firing angle time
      }   
    }
    p->Thy_Prev_IO_State = digitalRead(IO_ThyristorFire);
  }

// Create Grid Currents based on the provided phase shift and cosFi delay, phase shifts should be same as Grid Shifts.
// Afterwards Create Chopped Currents based on the Actual Grid Current, Thyristor Firing angle and Current Duty Cycle.
// Current Duty Cycle is used to create variable current "Irms" to emulate variable currents or loads.

void Generate_Simulated_Chopped_Current(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift){
  uint8_t I_Chop_DutyCycle = (uint8_t)(((Half_Cycle_Time - p->Thyristor_Sim_Firing_Angle) *p->I_Chopped_DutyCycle) / 100);
   if(I_Chop_DutyCycle < 4) I_Chop_DutyCycle = 4; // min %4 

  // CosFi 0/90 as radyan 0 - 1,0 1->on phase    0->90radyan->5mSec->100 Δ
 //  PhaseShift += (100-CoSφ); // Cosφ added as a shift 100->cosφ 1 // 0->cosφ 0 //100 variable value stands for 90° radian // 5 mSec 
  uint8_t ΔtCoSφ  = 100 - MotorCoSφ; 
  uint16_t I_Init = PhaseShift + ΔtCoSφ;
  uint16_t I_MaxPhase =  Half_Cycle_Time + PhaseShift + ΔtCoSφ; // Max time 10 Msec
  uint16_t I_Passive = p->Thyristor_Sim_Firing_Angle + PhaseShift + ΔtCoSφ ;
  uint16_t I_Active = p->Thyristor_Sim_Firing_Angle + I_Chop_DutyCycle + PhaseShift + ΔtCoSφ ;
  bool IO_Status = 0;
/*
  if(p->Thy_Fire_Drop_Shift){
    p->IO_Chopped_Current_Debug = OFF; 
    p->IO_Thyristor_Voltage_Debug = OFF;  
    return;
  }
  */
  p->I_Sim_Chopped_Counter++;    //!!!!  p->I_Chopped_DutyCycle cant be bigger than p->Thyristor_Firing_Angle 
  if(p->I_Sim_Chopped_Counter >=  I_MaxPhase) {
    p->I_Sim_Chopped_Counter  = I_Init;// Full Period Timeout Start Again %100
    IO_Status = OFF;
  }
  else if(p->I_Sim_Chopped_Counter <= I_Passive) // reference full bypass current trajectory r
      IO_Status = OFF;
  else if(p->I_Sim_Chopped_Counter <= I_Active)   
    IO_Status = ON;
  else IO_Status = OFF;

  if(IO_Status){
    p->IO_Chopped_Current_Debug = ON;
    p->IO_Thyristor_Voltage_Debug = ON;
  }
  else {
    p->IO_Chopped_Current_Debug = OFF; 
    p->IO_Thyristor_Voltage_Debug = OFF;    
  }

}

bool Generate_Chopped_Current(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift){
  uint8_t I_Chop_DutyCycle = (uint8_t)(((Half_Cycle_Time - p->Thyristor_Sim_Firing_Angle) *p->I_Chopped_DutyCycle) / 100);
   if(I_Chop_DutyCycle < 4) I_Chop_DutyCycle = 4; // min %4 

  // CosFi 0/90 as radyan 0 - 1,0 1->on phase    0->90radyan->5mSec->100 Δ
 //  PhaseShift += (100-CoSφ); // Cosφ added as a shift 100->cosφ 1 // 0->cosφ 0 //100 variable value stands for 90° radian // 5 mSec 
  uint8_t ΔtCoSφ  = 100 - MotorCoSφ; 
  uint16_t I_Init = PhaseShift + ΔtCoSφ;
  uint16_t I_MaxPhase =  Half_Cycle_Time + PhaseShift + ΔtCoSφ; // Max time 10 Msec
  uint16_t I_Passive = p->Thyristor_Firing_Angle + PhaseShift + ΔtCoSφ ;
  uint16_t I_Active = p->Thyristor_Firing_Angle + I_Chop_DutyCycle + PhaseShift + ΔtCoSφ ;

  p->I_Chopped_Counter++;    //!!!!  p->I_Chopped_DutyCycle cant be bigger than p->Thyristor_Firing_Angle 
  if(p->I_Chopped_Counter >=  I_MaxPhase) {
    p->I_Chopped_Counter  = I_Init;// Full Period Timeout Start Again %100
    p->IO_Current  = OFF;   
  }
  else if(p->I_Chopped_Counter <= I_Passive) // reference full bypass current trajectory r
      p->IO_Current  = OFF;  
  else if(p->I_Chopped_Counter <= I_Active)   
    p->IO_Current  = ON;  
  else p->IO_Current  = OFF;  

  return p->IO_Current; 
}

