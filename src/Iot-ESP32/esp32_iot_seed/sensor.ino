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
