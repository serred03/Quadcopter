import cv2
import numpy as np 

img = cv2.imread('Messi.jpg')

ball = img[280:340, 330:390]
img[273:333, 100:160] = ball 

cv2.imshow('image',img)

cv2.waitKey(0)
