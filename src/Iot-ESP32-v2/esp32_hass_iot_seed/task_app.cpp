// task_app.cpp
#include "task_app.h"
#include "MQTT_driver.h"  //  MQTT驱动



void State_check_app(){
    connect_check();  //  连接状态检查
    // 定期发送消息
    if (mqttClient.connected())
    {
            previousPublishMillis = currentMillis;
            mqttClient.publish("esp32/test", "state:online!");
    }
    Serial.println("OK");
}

void MQTT_event_app(){
    mqttClient.loop(); // 处理MQTT事务
}
