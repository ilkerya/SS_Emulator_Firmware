



// prototypes for Grid & Thyristor Fring & Chopped Current Functions
bool Generate_Grid_Voltage(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift); // R-S-T Grid Voltage 
void Detect_Thyristor_Firing_Angle(struct Grid_Thy_I_Chopped_Var *p, const uint16_t IO_ThyristorFire); // 
void Generate_Simulated_Thyristor_Firing_Angle(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift); // Set from UI
bool Generate_Chopped_Current(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift);
void Generate_Simulated_Chopped_Current(struct Grid_Thy_I_Chopped_Var *p, uint16_t PhaseShift);
