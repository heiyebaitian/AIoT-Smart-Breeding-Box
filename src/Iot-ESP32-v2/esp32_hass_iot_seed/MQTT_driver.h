// MQTT_driver.h  
#ifndef MQTT_DRIVER_H  
#define MQTT_DRIVER_H 

#include <Arduino.h> 
#include <stdint.h>
#include <WiFi.h>
#include <PubSubClient.h>



extern const bool DEBUG_MODE;

/* WiFi相关配置信息 */
extern const char *wifi_ssid;
extern const char *wifi_password;
/* MQTT相关配置信息 */
extern const char *mqtt_broker_addr; // MQTT服务器地址
extern const uint16_t mqtt_broker_port; // MQTT服务器端口
extern const char *mqtt_username; // 账号（非必须）
extern const char *mqtt_password; // 密码（非必须）
extern const uint16_t mqtt_client_buff_size; // 客户端缓存大小（非必须）
extern String mqtt_client_id; // 客户端ID
extern const char *mqtt_willTopic; // MQTT连接遗嘱主题
extern const char *mqtt_topic_pub; // 需要发布到的主题
extern const char *mqtt_topic_sub; // 需要订阅的主题
/* 系统事件时间记录 */
extern unsigned long previousConnectMillis; // 毫秒时间记录
extern const long intervalConnectMillis; // 时间间隔
extern unsigned long previousPublishMillis; // 毫秒时间记录
extern const long intervalPublishMillis; // 时间间隔
extern unsigned long currentMillis; // 当前时间记录
/* 系统运行数据 */
extern long rssi;

extern WiFiClient tcpClient;
extern PubSubClient mqttClient;




void wifi_setup();
void mqtt_setup();
void mqtt_subscribe_setup();
void mqtt_callback(char *topic, byte *payload, unsigned int length);
void setup_iot_server();
void connect_check();
void hass_debug_log(char *log);



#endif //MQTT_DRIVER_H
