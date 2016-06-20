#!/usr/bin/env python
__author__ = 'abhishek'
import cv2
import numpy
import os,sys
import re

cascPath='haarcascade_frontalface_default.xml'
eye_path='haarcascade_eye.xml'
def detect(image_path,cropped_path):
    image=cv2.imread(image_path)
    #image=cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)

    faceCascade= cv2.CascadeClassifier(cascPath)
    faces = faceCascade.detectMultiScale(
        image,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30),
        flags = cv2.cv.CV_HAAR_SCALE_IMAGE
    )
    try:
        for (x,y,w,h) in faces:
            cv2.rectangle(image,(x, y),(x+w, y+h),(0,255,0),2)
            image=image[y:y+h, x:x+w]
            cv2.imwrite(cropped_path,image)
    except ValueError:
        pass
    else:
        pass

if __name__=='__main__':
    path=sys.argv[1]
    cropped_path= sys.argv[2]
    if not os.path.exists(cropped_path):
        os.system("mkdir "+cropped_path)

    for root, dirs, files in os.walk(path):
        for file_ in files:
            files_path=os.path.join(root, file_)
            image_initials=re.match(r"[0-9]*",file_).group()
            cropped_image_dir= os.path.join(cropped_path, image_initials)
            if not os.path.exists(cropped_image_dir):
                os.system("mkdir "+cropped_image_dir)
            cropped_image_path= os.path.join(cropped_image_dir, file_)
            temp=detect(files_path,cropped_image_path)

        for subdirs in dirs:
            sub_path=os.path.join(root, subdirs)