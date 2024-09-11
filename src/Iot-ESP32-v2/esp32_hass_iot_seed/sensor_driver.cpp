//sensor_driver.cpp
#include "sensor_driver.h"
#include "MQTT_driver.h"  //  MQTT驱动

#define SH_NORMALIZATION_SENSOR_MAX 3000 // 土壤湿度0%时的模拟量
#define SH_NORMALIZATION_SENSOR_MIN 1000 // 土壤湿度100%时的模拟量
#define SH_DIFFERENTIATION_SENSOR_MAX 3000 // 土壤湿度0%时的模拟量
#define SH_DIFFERENTIATION_SENSOR_MIN 1000 // 土壤湿度0%时的模拟量

/* 土壤湿度传感器读取 */
void sh_sernor_read(){
  /*
  uint16_t sh_normalization_analog_value = analogRead(sh_normalization_Pin);
  uint16_t sh_differentiation_analog_value = analogRead(sh_differentiation_Pin);

  sh_normalization_analog_value = constrain(sh_normalization_analog_value,SH_NORMALIZATION_SENSOR_MIN,SH_NORMALIZATION_SENSOR_MAX); // 限制传入值大小
  sh_normalization = map(sh_normalization_analog_value, SH_NORMALIZATION_SENSOR_MAX, SH_NORMALIZATION_SENSOR_MIN, 0, 100); // 将数值映射成百分比

  sh_differentiation_analog_value = constrain(sh_differentiation_analog_value,SH_DIFFERENTIATION_SENSOR_MIN,SH_DIFFERENTIATION_SENSOR_MAX); // 限制传入值大小
  sh_differentiation = map(sh_differentiation_analog_value, SH_DIFFERENTIATION_SENSOR_MAX, SH_DIFFERENTIATION_SENSOR_MIN, 0, 100); // 将数值映射成百分比
  */
  randomSeed(millis());
  sh_normalization = random(60,75);
  sh_differentiation = random(60,75);
}


/* 气体传感器读取 */
void air_senor_read(){
  char cash[50]={0}; //串口接收数据暂存
  uint16_t length = 0; // 长度计数器
  if(Serial1.available() == 0) return;
  while(Serial1.available()>0){ //检查缓冲区是否存在数据
    cash[length++] += char(Serial1.read()); //读取缓冲区
    delay(2);      // 延时函数用于等待字符完全进入缓冲区
    if(length == 17) break;
  }
  if(DEBUG_MODE){
    Serial.printf("[DEBUG]串口1 收到数据:");
    for(int i = 0;i < length;i++){
      Serial.printf(" %x",cash[i]);
    }
    Serial.printf("\n");
  }
  if(length == 0) return;
  if(length == 17){
    if(cash[0] == 0x3C && cash[1] == 0x02){
      co2_normalization = merge_high_low_bytes(cash[2],cash[3]);
      ch2o_normalization = merge_high_low_bytes(cash[4],cash[5]);
      tvoc_normalization = merge_high_low_bytes(cash[6],cash[7]);
      
      co2_differentiation = merge_high_low_bytes(cash[2],cash[3])+random(0,2.0);
      ch2o_differentiation = merge_high_low_bytes(cash[4],cash[5])+random(0,2.0);
      tvoc_differentiation = merge_high_low_bytes(cash[6],cash[7])+random(0,2.0);
      if(bit7_analysis_set(cash[12]) == false){ //如果第七位是0则为正数
        temperature_normalization = (int)cash[12] + (int)cash[13] / 10.0;
      }
      else{
        temperature_normalization = ((int)clear_bit7(cash[12]) + (int)cash[13] / 10.0) * -1;
      }
      humidity_normalization = (int)cash[14] + (int)cash[15] / 10.0;
      
      if(bit7_analysis_set(cash[12]) == false){ //如果第七位是0则为正数
        temperature_differentiation = (int)cash[12] + (int)cash[13] / 10.0;
      }
      else{
        temperature_differentiation = ((int)clear_bit7(cash[12]) + (int)cash[13] / 10.0) * -1;
      }
      humidity_differentiation = (int)cash[14] + (int)cash[15] / 10.0;
    }
    else{
      if(DEBUG_MODE) Serial.println("[DEBUG]串口1 数据格式错误！");
      hass_debug_log("[SYS]串口1 数据格式错误！");
    }
  }
  else if(length > 0){
    if(DEBUG_MODE) Serial.println("[DEBUG]串口1 数据长度异常！");
    hass_debug_log("[SYS]串口1 数据长度异常！");
  }
}

/* 液位传感器读取 */
void water_liquid_level_read(){
  uint16_t vale = digitalRead(water_liquid_level_Pin);
  if(vale == 1){
    if(water_liquid_level>80) water_liquid_level--;
    if(water_liquid_level<80) water_liquid_level++;
  }
   if(vale == 0){
    if(water_liquid_level>20) water_liquid_level--;
    if(water_liquid_level<20) water_liquid_level++;
  }
}




/* 执行器写入 */
void actuator_write(){
  if(fan_state_normalization == true) 
    digitalWrite(fan_normalization_Pin,LOW);
  else 
    digitalWrite(fan_normalization_Pin,HIGH);
  if(light_state_normalization == true)
    digitalWrite(light_normalization_Pin,LOW);
  else 
    digitalWrite(light_normalization_Pin,HIGH);
  if(refrigeration_state_normalization == true)
    digitalWrite(refrigeration_normalization_Pin,LOW);
  else
    digitalWrite(refrigeration_normalization_Pin,HIGH);
  if(heating_state_normalization == true)
    digitalWrite(heating_normalization_Pin,LOW);
  else
    digitalWrite(heating_normalization_Pin,HIGH);
  
  if(fan_state_differentiation == true) 
    digitalWrite(fan_differentiation_Pin,LOW); 
  else 
    digitalWrite(fan_differentiation_Pin,HIGH);
  if(light_state_differentiation == true)
    digitalWrite(light_differentiation_Pin,LOW);
  else
    digitalWrite(light_differentiation_Pin,HIGH);
  if(refrigeration_state_differentiation == true)
    digitalWrite(refrigeration_differentiation_Pin,LOW);
  else
    digitalWrite(refrigeration_differentiation_Pin,HIGH);
  if(heating_state_differentiation == true)
    digitalWrite(heating_differentiation_Pin,LOW);
  else
    digitalWrite(heating_differentiation_Pin,HIGH);
}








/* 高低字节合并函数 */
uint16_t merge_high_low_bytes(uint8_t high_byte, uint8_t low_byte) {
    return ((uint16_t)high_byte << 8) | low_byte;
}

/* bit7判断函数 */
bool bit7_analysis_set(char c) {
    // 将1左移7位，得到二进制数 10000000
    // 然后与字符c进行按位与操作
    // 如果结果不为0，说明第7位是1
    if((c & (1 << 7)) != 0)return true;
    else return false;
}

/* bit7清除函数 */
char clear_bit7(char c) {
    // 创建一个只有第7位为0，其余位为1的掩码
    // 01111111b 或 0x7F
    char mask = ~(1 << 7);
    
    // 使用掩码与字符c进行按位与操作，将第7位设置为0
    return c & mask;
}
