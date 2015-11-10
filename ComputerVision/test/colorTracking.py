import cv2 
import numpy as np 

cap = cv2.VideoCapture(0)

while(1):

    _,frame = cap.read()

    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)

    lower_blue = np.array([110,50,50])
    upper_blue = np.array([130,255,255])

    mask = cv2.inRange(hsv,lower_blue, upper_blue)
