#!/usr/bin/env python
__author__ = 'abhishek'
import os
import sys

def calculate_image(path,more_than7):
    for root, dirs, files in os.walk(path):
        for subdirs in dirs:
            sub_path=os.path.join(root,subdirs)
            total_images= len(os.listdir(sub_path))
            if total_images>7:
                os.system("cp -R "+sub_path+" "+more_than7)

if __name__=='__main__':
    path=sys.argv[1]
    more_than_6="more_than6"
    if not os._exists(more_than_6):
        os.system("mkdir "+more_than_6)
    calculate_image(path,more_than_6)