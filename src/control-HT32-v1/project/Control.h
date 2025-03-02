#ifndef __CONTROL_H
#define __CONTROL_H

extern u8  Box_Configuration[32];	

 void Send_Box_Configuration(void);
 void Soil_Control(void);
 void Liqiud_State(void);
 void Water_Switch(void);
 void Fan_Switch(void);
 void Light_Switch(void);
#endif