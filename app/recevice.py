import serial
import gzip

serial = serial.Serial('COM7',115200,timeout=2)
if serial.isOpen():
    print ('串口已打开')
 
    while True:
        data = serial.read(6)    #串口读20位数据
        if data == "\0":
            print("\n")
        else:
            print(data)
    print ('receive data is :',data) 

else:
    print ('串口未打开')
 
 
 
#关闭串口
serial.close()

if serial.isOpen():
    print ('串口未关闭')
else:
    print ('串口已关闭')