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
            mqttClient.publish("esp32/normalization/state", "online");
            mqttClient.publish("esp32/differentiation/state", "online");
            mqttClient.publish("esp32/mechanical_arm/state", "online");
    }
    if(DEBUG_MODE)Serial.println("[DEBUG]Connect state OK!");
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
  if(DEBUG_MODE) Serial.printf("[DEBUG]%s\n",sensor1_normalization_string.c_str());
  mqttClient.publish("esp32/normalization/sensor1",sensor1_normalization_string.c_str());

  /* 差异化培养区传感器数据上传 */
  String sensor1_differentiation_string = "{\"temperature\":" + String(temperature_differentiation) + 
                                        ",\"humidity\":" + String(humidity_differentiation) +
                                        ",\"CO2\":" + String(co2_differentiation) +
                                        ",\"CH2O\":" + String(ch2o_differentiation) +
                                        ",\"TVOC\":" + String(tvoc_differentiation) +
                                        ",\"sh\":" + String(sh_differentiation) +
                                        "}";
  if(DEBUG_MODE) Serial.printf("[DEBUG]%s\n",sensor1_differentiation_string.c_str());
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
  
  
  if(DEBUG_MODE) Serial.println("[DEBUG]Iot data upload finish！");
}
