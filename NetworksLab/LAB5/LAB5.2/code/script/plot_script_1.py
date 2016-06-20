__author__ = 'abhishek'
import matplotlib.pyplot as plt
import sys

filename1=sys.argv[1]
file1=open(filename1,"r")
lst_a,lst_b=[],[]

for lines in file1:
    str_=[float(val) for val in lines.split()]
    lst_a.append(str_[0])
    lst_b.append(str_[2])
file1.close()
plt.figure()
plt.plot(lst_a,lst_b)
plt.title("TcpLab")
plt.xlabel("time")
plt.ylabel("cwnd")
plt.show()
