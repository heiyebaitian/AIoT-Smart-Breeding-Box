//  Blinker任务程序
void Blinker_app(){
  Blinker.run();  
}





//  串口任务程序
void Serial_app(){
  
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



/* Debug函数用于自动调整变量数值以供测试 */
void Debug(){
  if(DEBUG_MODE){
    if(Temperature[0]<150) Temperature[0]+=10;
    else Temperature[0]=-100;
    if(Humidity[0]<100) Humidity[0]+=1;
    else Humidity[0]=-10;
    if(Sh[0]<100) Sh[0]+=1;
    else Sh[0]=-10;
    if(CO2[0]<1500) CO2[0]+=100;
    else CO2[0]=-10;
  }
}
