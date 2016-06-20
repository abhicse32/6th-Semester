#!/usr/bin/env python
__author__ = 'abhishek'
import cv2
import numpy
import os,sys
import re

def detect(image_path,cropped_path):
    cascPath='haarcascade_frontalface_default.xml'
    image=cv2.imread(image_path)
    #image=cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)

    cv2.imshow("window",image)
    cv2.waitKey(5000)
    faceCascade= cv2.CascadeClassifier(cascPath)

    faces = faceCascade.detectMultiScale(
        image,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30),
        flags = cv2.cv.CV_HAAR_SCALE_IMAGE
    )

    for (x,y,w,h) in faces:
        cv2.rectangle(image,(x, y),(x+w, y+h),(0,255,0),2)
        image=image[y:y+h, x:x+w]
        cv2.imwrite(cropped_path,image)

if __name__=='__main__':
    path=sys.argv[1]

    for root, dirs, files in os.walk(path):
        for file in files:
             files_path=os.path.join(root, file)

        for subdir in dirs:
            sub_path=os.path.join(root, subdir)

            for files_ in sub_path:
                image_path=os.path.join(sub_path, files_)