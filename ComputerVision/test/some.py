'''
Created on Nov 9, 2015

@author: serred
'''

import cv2
import numpy 

capture = cv2.VideoCapture(0) 

ret, frame = capture.read()

cv2.imshow('image',frame)

capture.release()

cv2.waitKey(0)

cv2.destroyAllWindows()
