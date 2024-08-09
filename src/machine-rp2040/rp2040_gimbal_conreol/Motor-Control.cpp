// gimbal_control.cpp
#include "gimbal_control.h"
#include <Servo.h>
#include "Motor-Control.h"


extern Servo myservo;

// 滴灌命令
extern uint16_t water_status;

//抓取命令
extern uint16_t crawl_status;

//坐标指令标志
extern uint16_t Normal_Crawl[2];
extern uint16_t Defridence[2];

int x_last = 2200;                          //x轴基于上一次需要移动的位置
int y_last = 2050;                          //y轴基于上一次需要移动的位置
int drip_timg = 500;                        //滴灌时间
int nomal_status = 0;                       //常态滴灌状态
int different_status = 0;                   //异层滴灌状态
int hand_on = 30;                     //机械手开
int hand_off = 70;                     //机械手关
int crawl_First_hight[2] = {900, 3000};      //第一层机械手抓取和运动的安全高度
int crawl_Second_hight[2] = {9700, 11000};      //第二层机械手抓取和运动的安全高度
int First_flag = 0;
int Second_flag = 0;

int Middle_aisle[4] = {200, 9000};                                      //仅记住中间点的X,Y坐标，Z的坐标需要根据到时候在那一层来取反。
int First_normal[7] = { 1800, 1800, 4000, 3850};             //第一层常态化区域的其中两盆需要抓取的坐标。x,y.x,y,z为常规高度
int Second_normal[7] = {1200, 1000, 4000, 3850};                  //第二层常态化区域的其中两盆需要抓取的坐标。x,y.x,y,z
int First_different[5] = {2100, 13750, 4200, 15750};            //第一层异层区域的其中两盆需要抓取的坐标。x,y.x,y,z
int Second_different[5] = {1700, 14200, 3000, 14000};           //第二层异层区域的其中两盆需要抓取的坐标。x,y.x,y,z
int first_crawl_flag[4] = {1, 1, 0, 0};                                 //第一层抓取标志,前两位为常态化区域的抓取标志，后两位为异层区域的抓取标志
int Second_crawl_flag[4] = {1, 1, 0, 0};                                //第二层抓取标志，前两位为常态化区域的抓取标志，后两位为异层区域的抓取标志
//箱子状态,FF 抓取状态 滴灌状态 FE
uint8_t Motor_Status[4] = {0XFF, 00, 00 , 0XFE};


void crawl_drip_control()
{
  int step = 0;
    if(!water_status && crawl_status)
    {
        Motor_Status[1] = 0x01;
        crawl_status = 0x00;
        Serial1.write(Motor_Status, 4);
        stepperZ.runToNewPosition(crawl_First_hight[1]);          //z轴上升到安全运动高度
        Serial.print("z轴上升到安全运动高度");

            if(Normal_Crawl[0])
            {
                Serial.print("正在抓取常态化第一盆");
                stepperX.runToNewPosition(First_normal[0]);        //x轴运动到对应位置
                Serial.print("y轴运动到对应位置");
                stepperY.runToNewPosition(First_normal[1]);        //y轴运动到对应位置
                myservo.write(hand_on);                            //机械手开
                Serial.print("z轴下降到安全抓取高度，开始抓取第一盆");
                stepperZ.runToNewPosition(crawl_First_hight[0]);   //z轴下降到安全抓取高度
                delay(500);
                myservo.write(hand_off);                           //机械手关
                delay(500);
                stepperZ.runToNewPosition(crawl_First_hight[1]);   //z轴上升到安全运动高度

                Serial.print("正在进入第一层差异化区域第一坐标");
                stepperY.runToNewPosition(First_different[1]);     //镜像关系，x轴不动，y轴移动即可
                stepperZ.runToNewPosition(crawl_First_hight[0]);   //z轴下降到安全抓取高度
                delay(1000);
                myservo.write(hand_on);                            //机械手开
                stepperZ.runToNewPosition(crawl_First_hight[1]);   //z轴上升到安全运动高度
                myservo.write(hand_off);                           //机械手关
                Serial.print("常态化第一坐标抓取完毕");
                first_crawl_flag[2] = 0x01;                        //常态化第一花盆已经抓取到差异化第一位置
                first_crawl_flag[0] = 0x00;
                Normal_Crawl[0] = 0x00;                             //清楚
                step ++;
            }
             if(Normal_Crawl[1])
            {
              Normal_Crawl[1] = 0x00;
                Serial.print("正在抓取常态化第二盆");
                stepperY.runToNewPosition(First_normal[3]);  //Y轴去到第二盆常态化花盆
                stepperX.runToNewPosition(First_normal[2]);  //X轴去到第二盆常态化花盆
                 Serial.print("X轴去到第二盆常态化花盆");
                delay(400);
                myservo.write(hand_on);                         //机械手开
                delay(1000);
                stepperZ.runToNewPosition(crawl_First_hight[0]);          //z轴下降到安全抓取高度
                Serial.print("z轴下降到安全抓取高度");
                delay(2000);
                myservo.write(hand_off);                        //机械手关
                delay(1000);
                stepperZ.runToNewPosition(crawl_First_hight[1]);          //z轴上升到安全运动高度
                stepperY.runToNewPosition(First_different[3]);  //Y轴去到第二盆异层花盆
                stepperX.runToNewPosition(First_different[2]);  //X轴去到第二盆异层花盆
                stepperZ.runToNewPosition(crawl_First_hight[0]);//z轴下降到安全抓取高度
                myservo.write(hand_on);
                stepperZ.runToNewPosition(crawl_First_hight[1]);//z轴上升到安全运动高度
                myservo.write(hand_off);                        //机械手关

                first_crawl_flag[3] = 0x01;                     //常态化第二花盆已经抓取到差异化第二位置
                step ++;
            }


              

             if(Defridence[0])
            {
                Defridence[0] = 0x00;
                Serial.print("正在抓二层常态化第一盆");
                  //进入二层
                stepperX.runToNewPosition(Middle_aisle[0]);     //x轴运动到对应位置
                stepperY.runToNewPosition(Middle_aisle[1]);     //y轴移动到中间过道
                stepperZ.runToNewPosition(crawl_Second_hight[0]);//进入二层，z轴上升到二层安全高度抓取

                //二层抓取
                stepperX.runToNewPosition(Second_normal[0]);  //x轴运动到对应位置
                stepperY.runToNewPosition(Second_normal[1]);  //y轴运动到对应位置
                delay(1000);
                myservo.write(hand_on);                         //机械手开
                delay(1000);
                stepperZ.runToNewPosition(crawl_Second_hight[0]);//z轴下降到安全抓取
                delay(1000);
                myservo.write(hand_off);                         //机械手关
                delay(1000);
                stepperZ.runToNewPosition(crawl_Second_hight[1]);//z轴上升到安全运动位置
                stepperX.runToNewPosition(Second_different[0]);    //x轴运动到对应位置
                stepperY.runToNewPosition(Second_different[1]);    //y轴运动到对应位置
                delay(1000);
                myservo.write(hand_on);                             //机械手开
                delay(1000);
                stepperZ.runToNewPosition(crawl_Second_hight[0]);   //z轴下降到安全抓取
                delay(1000);
                myservo.write(hand_off);                            //机械手关
                delay(1000);
                stepperZ.runToNewPosition(crawl_Second_hight[1]);   //z轴上升到安全运动位置
                // stepperX.runToNewPosition(Second_different[0]);     //x轴运动到二层差异区对应位置
                // stepperY.runToNewPosition(Second_different[1]);     //y轴运动到二层差异区对应位置
                // stepperZ.runToNewPosition(crawl_Second_hight[0]);   //z轴下降到安全抓取
                // myservo.write(hand_on);                             //机械手开
                // stepperZ.runToNewPosition(crawl_Second_hight[1]);   //z轴上升到安全运动位置
                // myservo.write(hand_off);                            //机械手关

                Second_crawl_flag[2] = 0x01;                        //常态化二层第一个花盆已经抓到差异化第一位置
            }
            /*++++++++++++++++开始抓取二层常态化第二盆+++++++++++++++++++*/
             if(Defridence[1])
            {

              Defridence[1] == 0x00;
                Serial.print("正在抓取二层常态化第二盆");
                stepperX.runToNewPosition(Second_normal[2]);     //x轴运动到对应位置
                Serial.print("x轴运动到对应位置");
                stepperY.runToNewPosition(Second_normal[3]);     //y轴运动到对应位置 
                Serial.println("y轴运动到对应位置");
                myservo.write(hand_on);                          //机械手开
                delay(1000);
                stepperZ.runToNewPosition(crawl_Second_hight[0]);//z轴下降到安全抓取位置
                Serial.println("z轴下降到安全抓取位置");
                myservo.write(hand_off);
                delay(1000);
                stepperZ.runToNewPosition(crawl_Second_hight[1]);//z轴上升到安全运动位置
                stepperX.runToNewPosition(Second_different[2]);
                stepperY.runToNewPosition(Second_different[3]);
                stepperZ.runToNewPosition(crawl_Second_hight[0]);//z轴下降到安全抓取位置
                myservo.write(hand_on);
                stepperZ.runToNewPosition(crawl_Second_hight[1]);//z轴上升到安全运动位置
                myservo.write(hand_off);

                

                 Second_crawl_flag[3] = 0x01;                    //二层第一盆已经抓取到差异化第二位置

               

            }
               /*++++++++++++++++回中间过道位置+++++++++++++++++++++*/
               
               
                stepperX.runToNewPosition(Middle_aisle[0]);     //x轴运动到对应位置
                Serial.println("x轴移动到中间过道");
                stepperY.runToNewPosition(Middle_aisle[1]);     //y轴移动到中间过道
                Serial.println("y轴移动到中间过道");
                stepperZ.runToNewPosition(crawl_First_hight[1]);//进入一层，z轴下降到一层安全高度
                /*++++++++++++++++回到初始位置+++++++++++++++++++*/
                stepperX.runToNewPosition(1000);
                Serial.println("x轴回到初始位置"); 
                stepperY.runToNewPosition(800);
                Serial.println("y轴回到初始位置"); 
               
                 
                Serial.println("抓取完成");
                /*上位机交互，抓取完成*/
                Motor_Status[1] = 0x00;
                Serial1.write(Motor_Status,4);
               
    }

/**************************************************************************************************** */
/***************************+++++++++++滴灌++++++++++++++++****************************************** */
/*************************************************************************************************** */
    if(water_status && !crawl_status)
    {
        water_status = 0x00;
        step = 0 ;                                                      //区域位清零，用于检测机械臂所在区域
        Serial.println("开始滴灌");
        Motor_Status[2] = 0x01;
        Serial1.write(Motor_Status, 4);
        stepperZ.runToNewPosition(crawl_First_hight[1]);                //z轴上升到安全运动高度
        Serial.println("z轴上升到安全运动高度");
         int lastY = -1000;
         int lastX = -1000;

        /******************常态化第一层滴灌************************* */
        for(int i = 0; i < 3; i++)                                      //遍历y轴，每次走固定坐标
        {
            lastY += y_last;
            stepperY.runToNewPosition(First_normal[1] + lastY);          //基于上一盆坐标y轴移动到下一盆y值
                                               //预设值，调整更改，y值之间的距离
            Serial.printf("i = %d\n", i);
            nomal_status ++;            
            for(int j = 0;j < 4;j++)                                    //遍历x轴，每次走固定坐标
            {
              if(first_crawl_flag[0] == 0x00 || first_crawl_flag[1] == 0x00)                         //如果常态化第一盆被抓取
                {
                    lastX = lastX + pow(-1,i) * 1000;                                      //预设值，调整更改，x值之间的距离
                }
                else
                {
                    Motor_Water(1);
                    delay(drip_timg);                                         //滴灌时间
                    Motor_Water(0);
                    lastX = lastX + pow(-1,i) * 1000;
                }
                
               Serial.printf("j = %d\n", j);
               stepperX.runToNewPosition(First_normal[0] +  lastX );    //基于上一盆坐标x轴移动到下一盆x值
               Serial.printf("基于上一盆坐标x轴移动到下一盆x值%d\n",(First_normal[0] + lastX));
               Serial.printf("lastX=%d\n",lastX);
                
            }
             if(nomal_status == 3)                                       //判断常态化滴灌是否完成，数值可能是3！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
                {
                      lastY = 0;
                      lastX = 0;
                    Serial.printf("nomal_status = %d\n",nomal_status);
                    
                    break;                                                   //跳出上层循环
                }    

           
        }
               Serial.printf("条件是否满足nomal_status = %d\n",nomal_status);

            /*************常态化滴灌完成，进行差异化滴灌**************** */
         if(nomal_status == 3)                                       //判断常态化滴灌是否完成
            {
               Serial.println("常态化滴灌完成，进行差异化滴灌");
                nomal_status = 0;
                   
                for(int j = 0; j < 2; j++)
                {
                   
                    for(int i = 0; i <= 1; i++)                                       //常态化滴灌状态位重置
                    {  
                       
                       if(j == 0 && i == 0)
                       {
                          stepperX.runToNewPosition(First_different[0]);    //基于上一盆坐标x轴移动到下一盆x值
                          stepperY.runToNewPosition(First_different[1]);

                           Serial.printf("x = 0, y = 0\n");
                       }
                       else if(j == 0 && i == 1)
                       {
                          stepperX.runToNewPosition(Second_different[0]);    //基于上一盆坐标x轴移动到下一盆x值
                          stepperY.runToNewPosition(Second_different[1] + y_last);

                           Serial.printf("x = 1, y = 0\n");
                       }
                       else if(j == 1 && i == 0)
                       {
                          stepperX.runToNewPosition(Second_different[0] + x_last);
                          stepperY.runToNewPosition(Second_different[1]);

                           Serial.printf("x = 0, y = 1\n");
                       }
                       else if(j == 1 && i == 1)
                       {
                           stepperX.runToNewPosition(2);
                           stepperY.runToNewPosition(3);
                           Serial.printf("x = 1, y = 1\n");
                       }
                        
                        if(first_crawl_flag[2] == 0x01 || first_crawl_flag[3] == 0x01)
                        {
                            Motor_Water(1);
                            delay(drip_timg);                                         //滴灌时间
                            Motor_Water(0);
                        }
                       if((j == 0 && i == 1) || (j == 1 && i == 0))
                       {
                             Motor_Water(1);
                            delay(drip_timg);                                         //滴灌时间
                            Motor_Water(0);
                       }
                    }
                }
               
            }
            /*++++++++++++++++回中间过道位置+++++++++++++++++++++*/
            Serial.println("回中间过道位置");
            stepperX.runToNewPosition(Middle_aisle[0]);
            stepperY.runToNewPosition(Middle_aisle[1]);
            stepperZ.runToNewPosition(crawl_Second_hight[1]);               //进入二层

            //重新赋值
            lastY = -1000;
            lastX = -1000;

           /******************常态化第二层滴灌************************* */
        for(int i = 0; i < 3; i++)                                      //遍历y轴，每次走固定坐标
        {
          Serial.println("常态化第二层滴灌开始");
            lastY += y_last;                                                //预设值，调整更改，y值之间的距离
            stepperY.runToNewPosition(Second_normal[1] + lastY);          //基于上一盆坐标y轴移动到下一盆y值
                                                           
            Serial.printf("Second_normal[1] + lastY = %d\n", Second_normal[1] + lastY);
            Serial.printf("i = %d\n", i);
            different_status ++;                        
            for(int j = 0;j < 4;j++)                                    //遍历x轴，每次走固定坐标
            {
                if(Second_crawl_flag[0] == 0x00 || Second_crawl_flag[1] == 0x00) //如果常态化第一盆被抓取
                {
                    lastX = lastX + pow(-1,i) * x_last;                                        //预设值，调整更改，x值之间的距离
                }
                else
                {
                    Motor_Water(1);
                    delay(drip_timg);                                         //滴灌时间
                    Motor_Water(0);
                    lastX = lastX + pow(-1,i) * x_last;  
                }

                stepperX.runToNewPosition(Second_normal[0] + (pow(-1,i) * lastX) );    //基于上一盆坐标x轴移动到下一盆x值
                Serial.printf("Second_normal[0] + (pow(-1,i) * lastX) = %d\n", Second_normal[0] + (pow(-1,i) * lastX));
               
            }
             /******************************结束常态化第二层滴灌***************************************************/
              if(different_status == 3)                                       //判断常态化滴灌是否完成，数值可能是3！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
                {
                    lastY = 0;
                    lastX = 0;
                    step ++;                                               //机械臂结束在二层常态化动作
                    break;                                                   //跳出上层循环
                }
           
      
           
        }
            /*************第二层常态化滴灌完成，进行差异化滴灌**************** */
         if(different_status == 3)                                       //判断常态化滴灌是否完成
            {
                step = 0;
                Serial.println("二层常态化滴灌完成，进行差异化滴灌");
                different_status = 0;
                for(int j = 0; j < 2; j++)
                {  
                   
                    for(int i = 0; i <= 1; i++)                                       //常态化滴灌状态位重置
                    {  
                         if(j == 0 && i == 0)
                       {
                          stepperX.runToNewPosition(Second_different[0]);    //基于上一盆坐标x轴移动到下一盆x值
                          stepperY.runToNewPosition(Second_different[1]);

                           Serial.printf("x = 0, y = 0\n");
                       }
                       else if(j == 0 && i == 1)
                       {
                          stepperX.runToNewPosition(Second_different[0]);    //基于上一盆坐标x轴移动到下一盆x值
                          stepperY.runToNewPosition(Second_different[1] + y_last);

                           Serial.printf("x = 1, y = 0\n");
                       }
                       else if(j == 1 && i == 0)
                       {
                          stepperX.runToNewPosition(Second_different[0] + x_last);
                          stepperY.runToNewPosition(Second_different[1]);

                           Serial.printf("x = 0, y = 1\n");
                       }
                       else if(j == 1 && i == 1)
                       {
                           stepperX.runToNewPosition(2);
                           stepperY.runToNewPosition(3);
                           Serial.printf("x = 1, y = 1\n");
                       }
                        
                        if(first_crawl_flag[2] == 0x01 || first_crawl_flag[3] == 0x01)
                        {
                            Motor_Water(1);
                            delay(drip_timg);                                         //滴灌时间
                            Motor_Water(0);
                        }
                       if((j == 0 && i == 1) || (j == 1 && i == 0))
                       {
                             Motor_Water(1);
                            delay(drip_timg);                                         //滴灌时间
                            Motor_Water(0);
                       }

                       if(i && j )
                        {
                             Serial.println("二层差异化滴灌完，跳出上层循环");
                            step ++;                                               //机械臂结束在二层差异化动作
                            break;
                        }
                    }
                    if(step)
                    {
                        step = 20;                   //滴灌完成，清楚机械臂装态
                        
                        /**********************交互上位机，滴灌完成****************************** */
                             water_status = 0x00;                     //命令清零   
                             Motor_Status[2] = 0x00;             //滴灌完成
                             Serial1.write(Motor_Status, 4);
                         /*++++++++++++++++回中间过道位置+++++++++++++++++++++*/
                        Serial.println("回中间过道位置");
                        stepperX.runToNewPosition(Middle_aisle[0]);
                        stepperY.runToNewPosition(Middle_aisle[1]);
                        stepperZ.runToNewPosition(crawl_First_hight[1]);               //进入一层
                        /*++++++++++++++++回一层常态化第一坐标+++++++++++++++++++++*/
                        stepperX.runToNewPosition(First_normal[0]);        //x轴运动到对应位置
                        stepperY.runToNewPosition(First_normal[1]);        //x轴运动到对应位置
                        break;
                    }

                    if(step == 20)
                    {
                      break;
                    }
                }
               
            } 
         
    }
}
