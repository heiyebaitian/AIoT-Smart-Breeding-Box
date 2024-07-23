// task_app.cpp
#include "task_app.h"
#include "MQTT_driver.h"  //  MQTT驱动


/* 连接状态检查任务程序 */
void State_check_app(){
    connect_check();  //  连接状态检查
    // 定期发送消息
    if (mqttClient.connected())
    {
            previousPublishMillis = currentMillis;
            mqttClient.publish("esp32/state", "online");
            //mqttClient.publish("esp32/normalization/state", "online");
            //mqttClient.publish("esp32/differentiation/state", "online");
            //mqttClient.publish("esp32/mechanical_arm/state", "online");
    }
    if(DEBUG_MODE)Serial.println("[DEBUG]连接检查正常!");
}


/* MQTT事物任务程序 */
void MQTT_event_app(){
    mqttClient.loop(); // 处理MQTT事务
}


/* Iot数据上传任务程序 */
void Iot_data_upload_app(){
  if(enable_Iot_data_upload == false) return;
  // WIFI状态 RSSI值
  String rssi_string = "{\"RSSI\":" + String(rssi) + "}";
  mqttClient.publish("esp32/system/wifi",rssi_string.c_str());

  /* 常态化培养区传感器数据上传 */
  String sensor1_normalization_string = "{\"temperature\":" + String(temperature_normalization) + 
                                        ",\"humidity\":" + String(humidity_normalization) +
                                        ",\"CO2\":" + String(co2_normalization) +
                                        ",\"CH2O\":" + String(ch2o_normalization) +
                                        ",\"TVOC\":" + String(tvoc_normalization) +
                                        ",\"sh\":" + String(sh_normalization) +
                                        "}";
  if(DEBUG_MODE) Serial.printf("[DEBUG]MQTT发送：%s\n",sensor1_normalization_string.c_str());
  mqttClient.publish("esp32/normalization/sensor1",sensor1_normalization_string.c_str());

  /* 差异化培养区传感器数据上传 */
  String sensor1_differentiation_string = "{\"temperature\":" + String(temperature_differentiation) + 
                                        ",\"humidity\":" + String(humidity_differentiation) +
                                        ",\"CO2\":" + String(co2_differentiation) +
                                        ",\"CH2O\":" + String(ch2o_differentiation) +
                                        ",\"TVOC\":" + String(tvoc_differentiation) +
                                        ",\"sh\":" + String(sh_differentiation) +
                                        "}";
  if(DEBUG_MODE) Serial.printf("[DEBUG]MQTT发送：%s\n",sensor1_differentiation_string.c_str());
  mqttClient.publish("esp32/differentiation/sensor1",sensor1_differentiation_string.c_str());

  /* 常态化培养区执行器数据上传 */
  // 常态化培养区通风系统状态
  if(fan_state_normalization) mqttClient.publish("esp32/normalization/fans","ON");
  else mqttClient.publish("esp32/normalization/fans","OFF");
  // 常态化培养区光照系统状态
  if(light_state_normalization) mqttClient.publish("esp32/normalization/light","ON");
  else mqttClient.publish("esp32/normalization/light","OFF");
  // 常态化培养区制冷系统状态
  if(refrigeration_state_normalization) mqttClient.publish("esp32/normalization/refrigeration","ON");
  else mqttClient.publish("esp32/normalization/refrigeration","OFF");
  // 常态化培养区制热系统状态
  if(heating_state_normalization) mqttClient.publish("esp32/normalization/heating","ON");
  else mqttClient.publish("esp32/normalization/heating","OFF");

  /* 差异化培养区执行器数据上传 */
  // 差异化培养区通风系统状态
  if(fan_state_differentiation) mqttClient.publish("esp32/differentiation/fans","ON");
  else mqttClient.publish("esp32/differentiation/fans","OFF");
  // 差异化培养区光照系统状态
  if(light_state_differentiation) mqttClient.publish("esp32/differentiation/light","ON");
  else mqttClient.publish("esp32/differentiation/light","OFF");
  // 差异化培养区制冷系统状态
  if(refrigeration_state_differentiation) mqttClient.publish("esp32/differentiation/refrigeration","ON");
  else mqttClient.publish("esp32/differentiation/refrigeration","OFF");
  // 差异化培养区制热系统状态
  if(heating_state_differentiation) mqttClient.publish("esp32/differentiation/heating","ON");
  else mqttClient.publish("esp32/differentiation/heating","OFF");

  /* 机械臂数据上传 */
  // 滴灌系统状态
  if(waterpump_state) mqttClient.publish("esp32/mechanical_arm/pump","ON");
  else mqttClient.publish("esp32/mechanical_arm/pump","OFF");
  // 水箱液位
  String water_liquid_level_string = "{\"water\":" + String(water_liquid_level) + "}";
  mqttClient.publish("esp32/mechanical_arm/water",water_liquid_level_string.c_str());
  
  
  if(DEBUG_MODE) Serial.println("[DEBUG]Iot数据上传完成！");
}



/* 串口1处理任务程序 */
void Serial1_analysis_app(){
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
      if(bit7_analysis_set(cash[12]) == false){ //如果第七位是0则为正数
        temperature_normalization = (int)cash[12] + (int)cash[13] / 10.0;
      }
      else{
        temperature_normalization = ((int)clear_bit7(cash[12]) + (int)cash[13] / 10.0) * -1;
      }
      humidity_normalization = (int)cash[14] + (int)cash[15] / 10.0;
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
