#!/usr/bin/env python
import sys
import os,subprocess
infile=sys.argv[1]
outfile="out1.txt"
node_id=sys.argv[2]
hello_interval=sys.argv[3]
lsa_interval=sys.argv[4]
spf_interval=sys.argv[5]

os.system("make;./ospf -i "+node_id+ " -f "+infile+" -o " + outfile +" -h "+ hello_interval 
	               + " -a " + lsa_interval +" -s "+ spf_interval)