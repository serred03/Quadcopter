'''
Created on Nov 9, 2015

@author: serred
'''
import cv2
import numpy as np 
import time

img1 = cv2.imread('messi5.jpg')

e1 = cv2.getTickCount()
t1 = time.time()
for i in xrange(5,49,2):
    img1 = cv2.medianBlur(img1,i)
e2 = cv2.getTickCount()
t = (e2 - e1)/cv2.getTickFrequency()
t2 = time.time()
time = t2 - t1
print "t = " + str(t) + "\n" + "time = " + str(time)
