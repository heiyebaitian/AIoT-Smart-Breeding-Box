


//  Blinker任务程序
void Blinker_app(){
  Blinker.run();  
}





//  串口任务程序
void Serial_app(){
  Serial_analysis();  // 串口1命令解析
  Serial1_analysis(); //  串口2命令解析
  Serial2_analysis(); //  串口3命令解析
}





//  信息自动上报任务程序
void Report_app(){
  error_flag = 0; //  重置错误标记
  Number_Sensor_Temperature();
  Report_app_task.delay(DELAY_TS);
  Number_Sensor_Humidity();
  Report_app_task.delay(DELAY_TS);
  Number_Sensor_Sh();
  Report_app_task.delay(DELAY_TS);
  Number_Sensor_CO2();
  Report_app_task.delay(DELAY_TS);
  Text_Text_info(); //  为了能正确获取错误信息，请确保此函数最后执行
  Debug();
  Report_app_task.delay(DELAY_TS);
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
    default:Text_info.print("ERROR：系统发生未知错误，请检查！");
            break;
            
  }
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
    Sensor_Humidity.print(Humidity[0]);
  }
  else{
    Sensor_Humidity.color("#1E90FF"); //蓝色
    Sensor_Humidity.print(Humidity[0]);
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



// 串口1解析器
void Serial_analysis(){
  String inString ="" ; //串口接收命令存放区
  while(Serial.available()>0){ //检查缓冲区是否存在数据
    inString += char(Serial.read()); //读取缓冲区
    delay(10);      // 延时函数用于等待字符完全进入缓冲区
  }


  
}



// 串口2解析器
void Serial1_analysis(){
  String inString ="" ; //串口接收命令存放区
  while(Serial1.available()>0){ //检查缓冲区是否存在数据
    inString += char(Serial1.read()); //读取缓冲区
    delay(10);      // 延时函数用于等待字符完全进入缓冲区
  }


  
}



// 串口3解析器
int Serial2_analysis(){
  char cash[50]; //串口接收命令存放区
  int length = 0;
  while(Serial2.available()>0){ //检查缓冲区是否存在数据
    cash[length++] += char(Serial2.read()); //读取缓冲区
    delay(10);      // 延时函数用于等待字符完全进入缓冲区
  }

  if(length<=3) return -1; //如果接收的数据长度不足或等于3，则一定不包含末尾3个0xFF结束标记，直接返回错误-1
  if(cash[length-1]!=0xFF || cash[length-2]!=0xFF || cash[length-3]!=0xFF) return -1; //如果接收的数据末尾三个字节不是0xFF，则一定不是正确的命令，直接返回错误-1

  if(length>0){
    /* 指令长度4 */
    if(length == 4){
      /* 系统启动报告 */
      if(cash[0]==0x88){

      }

      /* BOOT就绪报告 */
      if(cash[0]==0x00){
        Serial2.print("mcu_status=1");  //  将状态置1完成启动
      }
    }
    /* 指令长度5 */
    if(length == 5){
      /* 页面ID报告 */
      if(cash[0]==0x01){

      }
    }
  }
}
