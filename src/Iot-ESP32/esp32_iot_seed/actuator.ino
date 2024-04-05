//  注册回调函数
void Blinker_actuator_setup(){
    Button_WaterPump.attach(Button_WaterPump_callback);//  水泵开关
    Button_Light.attach(Button_Light_callback);//  灯开关
    Button_Fan.attach(Button_Fan_callback);//  风扇开关
}

void Button_WaterPump_callback(const String & state){
    waterpump_goal = !waterpump_status;
    Serial_tx();
}

void Button_Light_callback(const String & state){
    light_goal = !light_status;
    Serial_tx();
}

void Button_Fan_callback(const String & state){
    fan_goal = !fan_status;
    Serial_tx();
}


// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
}

/* 按下按键即会执行该函数
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
*/
