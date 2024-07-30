#ifndef __CONTROL_H
#define __CONTROL_H

extern uint16_t Box_Configuration[];
extern uint16_t Gas_information[];

void Level_Information(void);
void Soil_Control(void);
void Water_Switch(void);
void Fan_Switch(void);

void Light_Switch(void);
void Gas_sensor(void);

void Send_Box_Configuration(void);

#endif


