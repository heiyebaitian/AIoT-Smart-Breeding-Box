/* 全局变量 */
extern int pageid = 0;




//  Blinker任务程序
void Blinker_app(){
  Blinker.run();  
}





//  串口任务程序
void Serial_app(){
  Serial_analysis();  // 串口1命令解析
  Serial_tx();  //  串口1数据发送
  Serial1_analysis(); //  串口2命令解析
  Waterpump_status_get(); //  水泵状态获取
  Serial2_analysis(); //  串口3命令解析
  Serial2_tx(); //  串口3数据发送
}





//  信息自动上报任务程序
void Report_app(){
  Number_Sensor_Temperature();
  Report_app_task.delay(DELAY_TS);
  Number_Sensor_Humidity();
  Report_app_task.delay(DELAY_TS);
  Number_Sensor_Sh();
  Report_app_task.delay(DELAY_TS);
  Number_Sensor_CO2();
  Report_app_task.delay(DELAY_TS);
  Status_WaterPump();
  Report_app_task.delay(DELAY_TS);
  Status_Light();
  Report_app_task.delay(DELAY_TS);
  Status_Fan();
  Report_app_task.delay(DELAY_TS);
  Status_Water();
  Report_app_task.delay(DELAY_TS);
  Text_Text_info(); //  为了能正确获取错误信息，请确保此函数最后执行发送数据
  Debug();
  Report_app_task.delay(DELAY_TS);
}


/* 自动任务程序 */
void Autotask_app(){
  if(autotask_flag == 0){
    light_goal = 1;
    Serial_tx();
    Serial2.print("tasking.n0.val=25\xff\xff\xff");
    autotask_flag++;
    return;
  }
  else light_goal = 0;

  if(autotask_flag == 1){
    fan_goal = 1;
    Serial_tx();
    Serial2.print("tasking.n0.val=50\xff\xff\xff");
    autotask_flag++;
    return;
  }
  else fan_goal = 0;

  if(autotask_flag == 2){
    waterpump_goal = 1;
    Serial_tx();
    Waterpump_control_do();
    Serial2.print("tasking.n0.val=75\xff\xff\xff");
    autotask_flag++;
    return;
  }
  else waterpump_goal = 0;

  if(autotask_flag >= 3){
    Serial_tx();
    Serial2.print("tasking.gm0.aph=0\xff\xff\xff");
    Serial2.print("tasking.t0.txt=\"Mission Complete\"\xff\xff\xff");
    Serial2.print("tasking.b0.txt=\"HOME\"\xff\xff\xff");
    Serial2.print("tasking.n0.val=100\xff\xff\xff");
    Auto_app_task.disable();
    return;
  }
}


/* 提示信息回传函数（需要最后执行） */
void Text_Text_info(){
  if(error_flag == 0){
    Text_info.color("#000000");
    Text_info.print("系统状态正常...");
  }
  else Text_info.color("#DC143C");
  switch(error_flag)
  {
    case 0: break;
    case -1:Text_info.print("ERROR：多个传感器异常，请检查！");
            break;
    case 1:Text_info.print("ERROR：温度传感器异常，请检查！");
            break;
    case 2:Text_info.print("ERROR：湿度传感器异常，请检查！");
            break;
    case 3:Text_info.print("ERROR：土壤湿度传感器异常，请检查！");
            break;
    case 4:Text_info.print("ERROR；CO2传感器异常，请检查！");
            break;
    case 5:Text_info.print("ERROR：HT32串口传输长度异常");
            break;
    case 6:Text_info.print("ERROR：HT32串口标记异常");
            break;
    case 7:Text_info.print("ERROR：HT32-气体传感器数据异常");
            break;
    default:Text_info.print("ERROR：系统发生未知错误，请检查！");
            break;
            
  }
  error_flag = 0; //  重置错误标记
}



/* 历史数据上报缓存函数 */
void dataStorage()
{
    Blinker.dataStorage("temp", Temperature[0]);
    Blinker.dataStorage("humi", Humidity[0]);
}

/* 温度传感器回传函数 */
void Number_Sensor_Temperature(){
  Sensor_Temperature.unit("℃"); //  设置单位
  //Sensor_Temperature.icon("thermometer-three-quarters");  //  设置控件图标(已弃用)
  Sensor_Temperature.text("温度");  // 设置提示文本
  if(Temperature[0]>100 || Temperature[0]<-50){
    if(error_flag == 0) error_flag = 1;
    else error_flag =-1;
    BLINKER_LOG("ERROR：非法的温度读数,请检查回传数据是否正确！");
  }
  if(Temperature[0]<Temperature[1] || Temperature[0]>Temperature[2]){ //  如果超目标范围则红色输出，否则蓝色色输出
    Sensor_Temperature.color("#DC143C");
    Sensor_Temperature.print(Temperature[0]);
  }
  else{
    Sensor_Temperature.color("#1E90FF");
    Sensor_Temperature.print(Temperature[0]);
  }
}



/* 湿度度传感器回传函数 */
void Number_Sensor_Humidity(){
  Sensor_Humidity.unit("%"); //  设置单位
  Sensor_Humidity.text("湿度");  // 设置提示文本
  if(Humidity[0]>100 || Humidity[0]<0){
    if(error_flag == 0) error_flag = 2;
    else error_flag =-1;
    BLINKER_LOG("ERROR：非法的湿度读数,请检查回传数据是否正确！");
  }
  if(Humidity[0]<Humidity[1] || Humidity[0]>Humidity[2]){ //  如果超目标范围则红色输出，否则蓝色色输出
    Sensor_Humidity.color("#DC143C"); //红色
    Sensor_Humidity.print((int)Humidity[0]);
  }
  else{
    Sensor_Humidity.color("#1E90FF"); //蓝色
    Sensor_Humidity.print((int)Humidity[0]);
  }
}




/* 土壤湿度度传感器回传函数 */
void Number_Sensor_Sh(){
  Sensor_Humidity.unit("%"); //  设置单位
  Sensor_Humidity.text("土壤湿度");  // 设置提示文本
  if(Sh[0]>100 || Sh[0]<0){
    if(error_flag == 0) error_flag = 3;
    else error_flag =-1;
    BLINKER_LOG("ERROR：非法的土壤湿度读数,请检查回传数据是否正确！");
  }
  if(Sh[0]<Sh[1] || Sh[0]>Sh[2]){ //  如果超目标范围则红色输出，否则蓝色色输出
    Sensor_Sh.color("#DC143C"); //红色
    Sensor_Sh.print(Sh[0]);
  }
  else{
    Sensor_Sh.color("#1E90FF"); //蓝色
    Sensor_Sh.print(Sh[0]);
  }
}






/* CO2度传感器回传函数 */
void Number_Sensor_CO2(){
  Sensor_CO2.unit("ppm"); //  设置单位
  Sensor_CO2.text("二氧化碳");  // 设置提示文本
  if(CO2[0]>1000 || CO2[0]<0){
    if(error_flag == 0) error_flag = 4;
    else error_flag =-1;
    BLINKER_LOG("ERROR：非法的CO2读数,请检查回传数据是否正确！");
  }
  if(CO2[0]<CO2[1] || CO2[0]>CO2[2]){ //  如果超目标范围则红色输出，否则蓝色色输出
    Sensor_CO2.color("#DC143C");  //红色
    Sensor_CO2.print(CO2[0]);
  }
  else{
    Sensor_CO2.color("#1E90FF");  //蓝色
    Sensor_CO2.print(CO2[0]);
  }
}


/* 水泵状态回传函数 */
void Status_WaterPump(){
  if(waterpump_status == 0){ 
    Button_WaterPump.color("#A9A9A9");  //灰色
    Button_WaterPump.print();
  }
  else{
    Button_WaterPump.color("#1E90FF");  //蓝色
    Button_WaterPump.print();
  }
}

/* 生长灯状态回传函数 */
void Status_Light(){
  if(light_status == 0){ 
    Button_Light.color("#A9A9A9");  //灰色
    Button_Light.print();
  }
  else{
    Button_Light.color("#1E90FF");  //蓝色
    Button_Light.print();
  }
}


/* 生长灯状态回传函数 */
void Status_Fan(){
  if(fan_status == 0){ 
    Button_Fan.color("#A9A9A9");  //灰色
    Button_Fan.print();
  }
  else{
    Button_Fan.color("#1E90FF");  //蓝色
    Button_Fan.print();
  }
}

/* 液位状态回传函数 */
void Status_Water(){
  if(water_status == 0){ 
    Button_water.color("#DC143C");  //红色
    Button_water.print();
  }
  else{
    Button_water.color("#1E90FF");  //蓝色
    Button_water.print();
  }
}



/* Debug函数用于自动调整变量数值以供测试 */
void Debug(){
  if(DEBUG_MODE){
    if(Temperature[0]<50) Temperature[0]+=10;
    else Temperature[0]=-10;
    if(Humidity[0]<100) Humidity[0]+=1;
    else Humidity[0]=0;
    if(Sh[0]<100) Sh[0]+=1;
    else Sh[0]=0;
    if(CO2[0]<400) CO2[0]+=100;
    else CO2[0]=0;
  }
}

/* 高低字节合并函数 */
uint16_t merge_high_low_bytes(uint8_t high_byte, uint8_t low_byte) {
    return ((uint16_t)high_byte << 8) | low_byte;
}


// 串口1解析器
void Serial_analysis(){
  char cash[50]={0}; //串口接收命令存放区
  uint16_t length = 0;
  while(Serial.available()>0){ //检查缓冲区是否存在数据
    cash[length++] += char(Serial.read()); //读取缓冲区
    delay(1);      // 延时函数用于等待字符完全进入缓冲区
    if(length == 27) break;
  }
  if(length == 27){
    if(cash[0] == 0xFF && cash[1] == 0xFE){
        water_status = cash[2]; //  液位传感器状态
        Sh[0] = cash[3];  //  土壤湿度传感器
        //waterpump_status = cash[4]; //  水泵状态（已弃用——不再从HT32获取）
        fan_status = cash[5]; //  风扇状态
        light_status = cash[6]; //  生长灯状态
        if(cash[7] == 0x00 && cash[8] == 0x02){
          CO2[0] = merge_high_low_bytes(cash[9],cash[10]);  //  CO2数值
          CH2O[0] = merge_high_low_bytes(cash[11],cash[12]);  //  甲醛数值
          TVOC[0] = merge_high_low_bytes(cash[13],cash[14]);  //  TVOC数值
          Temperature[0] = cash[19];
          Humidity[0] = cash[21];
        }
        else error_flag = 7; // 错误标记抛出异常:气体传感器数据异常
    }
    else error_flag = 6; // 错误标记抛出异常:数据格式错误
  }
  else if(length > 0) error_flag = 5;  // 错误标记抛出异常：数据长度错误
}



// 串口2解析器
void Serial1_analysis(){
  String inString ="" ; //串口接收命令存放区
  while(Serial1.available()>0){ //检查缓冲区是否存在数据
    inString += char(Serial1.read()); //读取缓冲区
    delay(1);      // 延时函数用于等待字符完全进入缓冲区
  }


  
}



// 串口3解析器
void Serial2_analysis(){
  char cash[50]={0}; //串口接收命令存放区
  uint16_t length = 0;
  while(Serial2.available()>0){ //检查缓冲区是否存在数据
    cash[length++] += char(Serial2.read()); //读取缓冲区
    delay(1);      // 延时函数用于等待字符完全进入缓冲区
    if(length == 50) break;
  }

  if(length<=3) return;
  if(cash[length-1]!=0xFF || cash[length-2]!=0xFF || cash[length-3]!=0xFF) return;

  if(length>0){

    /* 指令长度4 */
    if(length == 4){
      /* 系统启动报告 */
      if(cash[0] == 0x88){

      }


    }

    /* 指令长度5 */
    if(length == 5){
      /* 页面ID报告 */
      if(cash[0]==0x01){
        pageid = cash[1]; //  将页面ID赋值
        Serial2.print("mcu_status=1\xff\xff\xff");
      }
    }

    /* 指令长度6 */
    if(length == 6){
      /* 遇到错误自动重启屏幕 */
      if(cash[0]==0xEE && cash[1]==0xEE && cash[2]==0xEE){
        Serial2.print("rest\xff\xff\xff");
      }

      /* BOOT就绪报告 */
      if(cash[0] == 0x00 && cash[1] == 0x00 && cash[2] == 0x00){
        Serial2.print("mcu_status=1\xff\xff\xff");  //  将状态置1完成启动
      }

      /* 自动任务触发报告 */
        if(cash[0] == 0x05 && cash[1] == 0x00 && cash[2] == 0x01){
          autotask_flag = 0;
          Serial2.print("mcu_status=1\xff\xff\xff");
          Auto_app_task.enable();
        }
        if(cash[0] == 0x05 && cash[1] == 0x00 && cash[2] == 0x00){
          Serial2.print("mcu_status=1\xff\xff\xff");
          Auto_app_task.disable();
        }
      
    }

    /* 指令长度7 */
    if(length == 7){
      /* 一般按钮触发报告 */
      if(cash[0] == 0x02 && cash[1] == pageid){

      }

      /* 双态按钮触发报告 */
      if(cash[0] == 0x03 && cash[1] == pageid){
        if(cash[2] == 0x00){
          if(cash[3] == 0x00) fan_goal = 0;
          else if(cash[3] == 0x01) fan_goal = 1;
        }
        if(cash[2] == 0x01){
          if(cash[3] == 0x00) light_goal = 0;
          else if(cash[3] == 0x01) light_goal = 1;
        }
        if(cash[2] == 0x02){
          if(cash[3] == 0x00) waterpump_goal = 0;
          else if(cash[3] == 0x01) Waterpump_control_do();
        }
        Serial_tx();
      }

      /* 滑动条报告 */
      if(cash[0] == 0x04 && cash[1] == pageid){
      }

    }
  }
}

/* 水泵状态获取函数 */
void Waterpump_status_get(){
  waterpump_status = !digitalRead(statusPin);
}

void Waterpump_control_do(){
  if(waterpump_status == 0){
    digitalWrite(controlPin,LOW);
    delay(500);
    digitalWrite(controlPin,HIGH);
  }
}

/* 串口1指令发送函数 */
void Serial_tx(){
  Serial.printf("%c",0xFF);
  Serial.printf("%c",0x00);
  if(waterpump_status != waterpump_goal) Serial.printf("%c",waterpump_goal);
  else Serial.printf("%c",waterpump_status);
  if(fan_status != fan_goal) Serial.printf("%c",fan_goal);
  else Serial.printf("%c",fan_status);
  if(light_status != light_goal) Serial.printf("%c",light_goal);
  else Serial.printf("%c",light_status);
}


/* 串口2指令发送函数 */
void  Serial2_tx(){

  /* 如果页面ID为6 则为env_mon页面
  *  发送传感器数据给屏幕
  */
  if(pageid == 0x06){
    Serial2.print("n0.val=");
    Serial2.print(Temperature[0]);  // 温度
    Serial2.print("\xff\xff\xff");
    Serial2.print("n1.val=");
    Serial2.print(Humidity[0]); // 湿度
    Serial2.print("\xff\xff\xff");
    Serial2.print("n2.val=");
    Serial2.print(Sh[0]); // 土壤湿度
    Serial2.print("\xff\xff\xff");
    Serial2.print("n3.val=");
    Serial2.print(CO2[0]);  // 二氧化碳
    Serial2.print("\xff\xff\xff");
  }

  /* 如果页面ID为7 则为fun_con页面
  *  发送执行器状态给屏幕
  */
  if(pageid == 0x07){
    Serial2.print("bt0.val=");
    Serial2.print(fan_status);
    Serial2.print("\xff\xff\xff");
    Serial2.print("bt1.val=");
    Serial2.print(light_status);
    Serial2.print("\xff\xff\xff");
    Serial2.print("bt2.val=");
    Serial2.print(waterpump_status);
    Serial2.print("\xff\xff\xff");
    Serial2.print("bt3.val=0\xff\xff\xff");
    Serial2.print("bt4.val=0\xff\xff\xff");
  }
}
