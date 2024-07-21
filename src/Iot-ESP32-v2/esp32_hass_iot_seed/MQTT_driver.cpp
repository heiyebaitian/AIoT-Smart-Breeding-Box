// MQTT_driver.cpp  
#include "MQTT_driver.h "  
#include "task_app.h"



/* WIFI初始化函数 */
void wifi_setup(){
  if(DEBUG_MODE) Serial.printf("\nConnecting to %s", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    if(DEBUG_MODE) Serial.print(".");
  }
  if(DEBUG_MODE){
    Serial.println("[DEBUG]WIFI connect is ok.");
    Serial.print("[DEBUG]IP address: ");
    Serial.println(WiFi.localIP());
  }
  hass_debug_log("[SYS]WIFI连接成功！");
}



/* MQTT初始化函数 */
void mqtt_setup(){
  // 设置MQTT客户端
  mqttClient.setClient(tcpClient);
  mqttClient.setServer(mqtt_broker_addr, mqtt_broker_port);
  //mqttClient.setBufferSize(mqtt_client_buff_size);
  mqttClient.setCallback(mqtt_callback);
  mqtt_client_id += String(WiFi.macAddress()); // 每个客户端需要有唯一的ID，不然上线时会把其他相同ID的客户端踢下线
  if(DEBUG_MODE)Serial.println("[DEBUG]MQTT正在连接...");
  if (mqttClient.connect(mqtt_client_id.c_str(), mqtt_username, mqtt_password, mqtt_willTopic, false, 2, "offline"))
  {
    mqttClient.publish("esp32/state", "online"); // 连接成功后发布状态
    if(DEBUG_MODE)Serial.println("[DEBUG]MQTT连接成功！");
    hass_debug_log("[SYS]MQTT连接成功!");
    mqtt_subscribe_setup();
    if(DEBUG_MODE)Serial.println("[DEBUG]MQTT订阅列表添加成功！");
    hass_debug_log("[SYS]MQTTMQTT订阅列表添加成功！");
  }
}



/* Iot服务初始化函数 */
void setup_iot_server(){
  if(DEBUG_MODE) Serial.printf("\n[DEBUG]WIFI Connecting to %s", wifi_ssid);
  wifi_setup();
  rssi = WiFi.RSSI(); // 获取网络连接质量
  mqtt_setup();
}




// MQTT消息回调函数
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    /* char类型topic转String类型 */  
    int topic_length = strlen(topic); // 确定topic字符串的长度
    char topic_char[topic_length + 1]; // 创建一个足够长的字符数组来存储 topic，并添加一个空字符
    strncpy(topic_char, topic, topic_length); // 将 topic 复制到新的字符数组中，并确保以 null 结尾
    topic_char[topic_length] = '\0'; // 添加字符串结束符
    String str_topic = String(topic_char); // 将 char 数组转换为 String
    
    /* byte类型payload转String类型 */  
    char payload_char[length + 1];
    for (unsigned int i = 0; i < length; i++) {
        payload_char[i] = (char)payload[i];
    }
    payload_char[length] = '\0'; // 添加字符串结束符
    String str_payload = String(payload_char);

    
    if(str_topic != "" && str_payload != ""){
      enable_Iot_data_upload = false; //关闭数据上传 等待状态刷新
       /* 常态化培养区执行器数据接收 */
      // 常态化培养区通风系统
      if(str_topic == "esp32/normalization/fans"){
        if(str_payload == "ON") fan_state_normalization = true;
        else if(str_payload == "OFF") fan_state_normalization = false;
      }
      
      // 常态化培养区制冷系统
      if(str_topic == "esp32/normalization/refrigeration"){
        if(str_payload == "ON") refrigeration_state_normalization = true;
        else if(str_payload == "OFF") refrigeration_state_normalization = false;
      }

      // 常态化培养区制热系统
      if(str_topic == "esp32/normalization/heating"){
        if(str_payload == "ON") heating_state_normalization = true;
        else if(str_payload == "OFF") heating_state_normalization = false;
      }

      // 常态化培养区光照系统
      if(str_topic == "esp32/normalization/light"){
        if(str_payload == "ON") light_state_normalization = true;
        else if(str_payload == "OFF") light_state_normalization = false;
      }



       /* 差异化培养区执行器数据接收 */
      // 差异化培养区通风系统
      if(str_topic == "esp32/differentiation/fans"){
        if(str_payload == "ON") fan_state_differentiation = true;
        else if(str_payload == "OFF") fan_state_differentiation = false;
      }

      // 差异化培养区制冷系统
      if(str_topic == "esp32/differentiation/refrigeration"){
        if(str_payload == "ON") refrigeration_state_differentiation = true;
        else if(str_payload == "OFF") refrigeration_state_differentiation = false;
      }

      // 差异化培养区制热系统
      if(str_topic == "esp32/differentiation/heating"){
        if(str_payload == "ON") heating_state_differentiation = true;
        else if(str_payload == "OFF") heating_state_differentiation = false;
      }

      // 差异化培养区光照系统
      if(str_topic == "esp32/differentiation/light"){
        if(str_payload == "ON") light_state_differentiation = true;
        else if(str_payload == "OFF") light_state_differentiation = false;
      }



      if(DEBUG_MODE){
      Serial.printf("\n[DEBUG]Message arrived in topic[%s], length [%d]\n", str_topic.c_str(), length);
      Serial.printf("[DEBUG]Message:%s",str_payload.c_str());
      }
      /*for (int i = 0; i < length; i++)
      {
          if(DEBUG_MODE)Serial.print((char)payload[i]);
      }*/
      if(DEBUG_MODE)Serial.println("\n[DEBUG]----------------END----------------");
      enable_Iot_data_upload = true; // 启用数据上传
     }
}



void mqtt_subscribe_setup(){
  mqttClient.subscribe("esp32/normalization/fans"); // 常态化培养区通风系统
  mqttClient.subscribe("esp32/normalization/refrigeration");  // 常态化培养区制冷系统
  mqttClient.subscribe("esp32/normalization/heating");  // 常态化培养区制热系统
  mqttClient.subscribe("esp32/normalization/light");  // 常态化培养区光照系统

  mqttClient.subscribe("esp32/differentiation/fans"); // 差异化培养区通风系统
  mqttClient.subscribe("esp32/differentiation/refrigeration");  // 差异化培养区制冷系统
  mqttClient.subscribe("esp32/differentiation/heating");  // 差异化培养区制热系统
  mqttClient.subscribe("esp32/differentiation/light");  // 差异化培养区光照系统
  
  
}


/* 连接状态检查及重连函数 */
void connect_check(){
  if(WiFi.status() != WL_CONNECTED){  // 如果WIFI未连接
    wifi_setup();
  }
  rssi = WiFi.RSSI(); // 获取网络连接质量
  if (!mqttClient.connected()) // 如果MQTT未连接
    {
            mqtt_client_id += String(WiFi.macAddress()); // 每个客户端需要有唯一的ID，不然上线时会把其他相同ID的客户端踢下线
            if(DEBUG_MODE)Serial.println("[DEBUG]MQTT正在连接...");
            if (mqttClient.connect(mqtt_client_id.c_str(), mqtt_username, mqtt_password, mqtt_willTopic, false, 2, "offline"))// 尝试连接MQTT服务器
            {
                mqttClient.publish("esp32/state", "online"); // 连接成功后发布状态
                if(DEBUG_MODE)Serial.println("[DEBUG]MQTT连接成功！");
                hass_debug_log("[SYS]MQTT连接成功!");
                mqtt_subscribe_setup();
                if(DEBUG_MODE)Serial.println("[DEBUG]MQTT订阅列表添加成功！");
                hass_debug_log("[SYS]MQTTMQTT订阅列表添加成功！");
            }
    }
}


/* HASS debug日志传输函数 */
void hass_debug_log(char *log){
    mqttClient.publish("esp32/system/debug", log); 
}
