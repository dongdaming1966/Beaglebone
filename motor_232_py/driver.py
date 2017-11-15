import serial  
import timeit

motor=serial.Serial("/dev/ttyUSB0",115200,timeout=0.5)
motor.write(b"ANSW0\r")
motor.write(b"EN\r")
motor.write(b"V100\r")
motor.write(b"BINSEND1")
motor.write(b"\xc8\x00\xca\x04")
for i in range(100):
    motor.write(b"\xc9")
    time_start = timeit.default_timer()
    mdata=motor.read(5)
    period=timeit.default_timer()-time_start
    hdata=mdata.hex()
    print("Actual celocity:"+hdata[2:4]+hdata[0:2])
    print("Motor current:"+hdata[6:8]+hdata[4:6])
    print("Raw:"+hdata)
    print(period)
motor.close()
