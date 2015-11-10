'''
Created on Nov 8, 2015

@author: serred
'''

import cv2  
import numpy


cap = cv2.VideoCapture(0) #0 indicates the capture interface

while(True):
    #Capture frame by frame
    ret,frame = cap.read()
    
    #Our operations on the frame come from here-
    gray = cv2.cvtColor(frame,cv2.COLOR_BGR2LAB)
    
    cv2.imshow('frame',gray)
    
    if cv2.waitKey(1) & 0XFF == ord('q'):
        break
    
cap.release()
cv2.destroyAllWindows() 



