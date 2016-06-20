#!/usr/bin/env python
import os,subprocess
import random

N=[random.randint(1,100) for n in range(100)]
M=[random.randint(1,100) for n in range(100)]
p=[random.randint(1,100)/100.0 for n in range(100)]
b=[random.randint(1,100)/100.0 for n in range(100)]
for i in range(100):
	os.system("make;./lab7 -N "+str(N[i])+" -M "+str(M[i])+" -b "+str(b[i])+" -p "+str(p[i]))