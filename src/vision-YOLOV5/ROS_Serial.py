import serial # 引入串口库


COM_port = 'COM10' # input('请输入设备的端口（如：COM1）：')
Baud_rate = 115200 # input('请输入设备的波特率（如：9600）：')
ser = serial.Serial(COM_port,Baud_rate,8,'N',timeout=1) # 初始化串口对象

def Serial_printf(A_string):    # 打印字符串(无换行)
    ser.write(A_string.encode('utf-8'))
def Serial_println(A_string):   # 打印字符串(自动换行)
    A_string += '\n'
    ser.write(A_string.encode('utf-8'))

def Serial_coordinate(p1_x,p1_y,p2_x,p2_y,display_x,display_y):
    # 坐标变量类型转换
    fp1_x = int(p1_x)
    fp2_x = int(p2_x)
    fp1_y = int(p1_y)
    fp2_y = int(p2_y)
    display_x = int(display_x)
    display_y = int(display_y)

    # 坐标重映射

    if(fp1_x > fp2_x):  # 保证x坐标的顺序正确 p1_x始终位于p2_x左侧
        Serial_println('x ERROR')
        return
    if(fp1_y > fp2_y):  # 保证y坐标的顺序正确 p1_y始终位于p2_y上方
        Serial_println('y ERROR')
        return

    if  fp1_x >= 0 and fp1_x <= display_x :   # 如果p1_x在屏幕内
        fp1_x = fp1_x - display_x/2
    else:
        Serial_println('p1x ERROR')
        return
        
    if  fp2_x >= 0 and fp2_x <= display_x :   # 如果p2_x在屏幕内
        fp2_x = fp2_x - display_x/2
    else:
        Serial_println('p2x ERROR')
        return

    if  fp1_y >= 0 and fp1_y <= display_y :   # 如果p1_y在屏幕内
        if fp1_y < display_y/2:  # 如果p1_y在屏幕的上半部分
            fp1_y = display_y/2 - fp1_y
        elif fp1_y > display_y/2:  # 如果p1_y在屏幕的下半部分
            fp1_y = -(fp1_y - display_y/2)
        elif fp1_y == display_y/2:  # 如果p1_y在屏幕的中间
            fp1_y = 0
    else:
        Serial_println('p1y ERROR')
        return
    if  fp2_y >= 0 and fp2_y <= display_y :   # 如果p2_y在屏幕内
        if fp2_y < display_y/2:  # 如果p2_y在屏幕的上半部分
            fp2_y = display_y/2 - fp2_y
        elif fp2_y > display_y/2:  # 如果p2_y在屏幕的下半部分
            fp2_y = -(fp2_y - display_y/2)
        elif fp2_y == display_y/2:  # 如果p2_y在屏幕的中间
            fp2_y = 0
    else:
        Serial_println('p2y ERROR')
        return
    

    # 结果处理

    center_x = (fp1_x + fp2_x)/2  # 计算两点的中心点
    center_y = (fp1_y + fp2_y)/2
    Astring = '(' + str(int(center_x)) + ',' + str(int(center_y)) + ')'
    Serial_println(Astring)
    print('(' + str(int(center_x)) + ',' + str(int(center_y)) + ')')

'''
# 测试代码
while True:
    p1_x = input('请输入第一个点的x坐标：')
    p1_y = input('请输入第一个点的y坐标：')
    p2_x = input('请输入第二个点的x坐标：')
    p2_y = input('请输入第二个点的y坐标：')
    display_x = 640
    display_y = 640
    Serial_coordinate(p1_x,p1_y,p2_x,p2_y,display_x,display_y)
'''
