// sensor_driver.h
#ifndef SENSOR_DRIVER_H  
#define SENSOR_DRIVER_H

#include <Arduino.h> 
#include <stdint.h>

extern const bool DEBUG_MODE;

/* I/O引脚配置 */
extern const int sh_normalization_Pin; // 常态化培养区土壤湿度传感器
extern const int sh_differentiation_Pin; // 差异化培养区土壤湿度传感器
extern const int fan_normalization_Pin; // 常态化培养区通风系统
extern const int light_normalization_Pin; // 常态化培养区光照系统
extern const int refrigeration_normalization_Pin; // 常态化培养区制冷系统
extern const int heating_normalization_Pin; // 常态化培养区制热系统
extern const int fan_differentiation_Pin; // 差异化培养区通风系统
extern const int light_differentiation_Pin; // 差异化培养区光照系统
extern const int refrigeration_differentiation_Pin; // 差异化培养区制冷系统
extern const int heating_differentiation_Pin; // 差异化培养区制热系统
extern const int water_liquid_level_Pin; // 液位传感器

/* 系统运行数据 */
extern long rssi;

/* 系统标志位 */
extern bool enable_Iot_data_upload;

/* 传感器数据记录 */

// 常态化培养区
extern double temperature_normalization; // 常态化培养区温度
extern double humidity_normalization; // 常态化培养区湿度
extern uint16_t sh_normalization; // 常态化培养区土壤湿度
extern uint16_t co2_normalization; // 常态化培养区二氧化碳
extern uint16_t ch2o_normalization; // 常态化培养区甲醛
extern uint16_t tvoc_normalization; // 常态化培养区TVOC
extern bool fan_state_normalization; // 常态化培养区通风系统状态
extern bool light_state_normalization; // 常态化培养区光照系统状态
extern bool refrigeration_state_normalization; // 常态化培养区制冷系统状态
extern bool heating_state_normalization; // 常态化培养区制热系统状态

// 差异化培养区
extern double temperature_differentiation; // 差异化培养区温度
extern double humidity_differentiation; // 差异化培养区湿度
extern uint16_t sh_differentiation; // 差异化培养区土壤湿度
extern uint16_t co2_differentiation; // 差异化培养区二氧化碳
extern uint16_t ch2o_differentiation; // 差异化培养区甲醛
extern uint16_t tvoc_differentiation; // 差异化培养区TVOC
extern bool fan_state_differentiation; // 差异化培养区通风系统状态
extern bool light_state_differentiation; // 差异化培养区光照系统状态
extern bool refrigeration_state_differentiation; // 差异化培养区制冷系统状态
extern bool heating_state_differentiation; // 差异化培养区制热系统状态



/* 机械臂数据 */
extern bool waterpump_state; //  滴灌系统状态
extern uint16_t water_liquid_level;  //  水箱液位


void sh_sernor_read();
void air_senor_read();
void water_liquid_level_read();
void actuator_write();
uint16_t merge_high_low_bytes(uint8_t high_byte, uint8_t low_byte);
bool bit7_analysis_set(char c);
char clear_bit7(char c);



#endif // SENSOR_DRIVER_H  
