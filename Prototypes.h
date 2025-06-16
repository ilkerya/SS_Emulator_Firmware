



// prototypes for Grid & Thyristor Fring & Chopped Current Functions
bool Create_Grid_Voltage(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift); // R-S-T Grid Voltage 
void Detect_Thyristor_Firing_Angle(struct Grid_Thy_I_Chopped_Var *p, const uint16_t IO_ThyristorFire); // 
void Create_Simulated_Thyristor_Firing_Angle(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift); // Set from UI
bool Create_Chopped_Current(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift);
void Create_Simulated_Chopped_Current(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift);
