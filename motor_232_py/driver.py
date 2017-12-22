import serial
import time
import timeit
import matplotlib.pyplot as plt
import threading
import random

total=1000
mvelo= [0] * total
mveli=[0]*total
mcur=[0]*total
time=[0]*total


def hex2dec(rawhex):
    if int(rawhex,16) >32767:
        ab =int(rawhex,16)-65535
    else:
        ab=int(rawhex,16)
    return ab


def writefd(fd,data):
    for i in data:
        fd.write(str(i))
        fd.write("\t")
    fd.write("\n")


def init():
    global motor
    motor = serial.Serial("/dev/ttyUSB1", 115200, timeout=0.5)
    motor.write(b"ANSW0\r")
    motor.write(b"EN\r")
    motor.write(b"BINSEND1")
    motor.write(b"\xc8\x00\xca\x04")


def control():
    motor.write(b"V" + bytes(str(mveli[i]), "ascii") + b"\r")
    motor.write(b"\xc9")
    mdata = motor.read(5)
    hdata = mdata.hex()
    mvelo[i] = hex2dec(hdata[2:4] + hdata[0:2])
    time[i] = timeit.default_timer() - time_start
    mcur[i] = hex2dec(hdata[6:8] + hdata[4:6]) * 4


def disp(ds):
    print("input:"+str(time[i-1]-time[i-2]))


def close():
    global motor
    motor.write(b"V0\r")
    motor.write(b"DI\r")
    motor.close()


threads=[]
init()

time_start = timeit.default_timer()
for i in range(total):
    mveli[i] = random.randint(-500,500)
    t1 = threading.Thread(target=control)
    t2 = threading.Thread(target=disp,args=(i,))
    t1.start()
    t2.start()
    t1.join()
    t2.join()
close()

fd=open('data.dat','w')
writefd(fd,time)
writefd(fd,mveli)
writefd(fd,mvelo)
writefd(fd,mcur)
fd.close()

plt.figure(1)
plt.plot(time, mvelo)
plt.plot(time,mveli)
plt.plot(time, mcur)
plt.show()

