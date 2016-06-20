#!/usr/bin/env python
__author__ = 'abhishek'
import os
import sys
import numpy

def func(path):
    for root, dirs , files in os.walk(path):
        for file in files:
            path=os.path.join(root,file)
            print(path)

        for subdirs in dirs:
            sub_path=os.path.join(root,subdirs)
            for file in sub_path:
                path_=os.path.join(sub_path,file)
                print(path_)

if __name__=='__main__':
    path=sys.argv[1]
    func(path)