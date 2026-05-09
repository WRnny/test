#ifndef _TRACK_H_
#define _TRACK_H_
#include "ti_msp_dl_config.h"

extern volatile float coord;
extern volatile int Flag_coord;
extern int pin_number[7];
extern volatile float qty;
extern volatile int Flag_Black;
extern volatile int Flag_RigjtAngle;
void track();

//void Read_Pin();
//void Track_PID();
//void track();

#endif  /* #ifndef _INTERRUPT_H_ */