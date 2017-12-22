import matplotlib.pyplot as plt


fd=open('data.dat','r')
data=fd.read()
fd.close()
data=data.split("\n")
time=data[0].split("\t")
mveli=data[1].split("\t")
mvelo=data[2].split("\t")
mcur=data[3].split("\t")

plt.figure(1)
plt.plot(time, mvelo)
plt.plot(time,mveli)
plt.plot(time, mcur)
plt.show()