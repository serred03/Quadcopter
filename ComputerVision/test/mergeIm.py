import cv2 
import numpy as np 

im1 = cv2.imread('Messi.jpg')
im2 = cv2.imread('opencv.png')

rows,cols,channels = im2.shape
roi = im1[0:rows,0:cols]

im2gray = cv2.cvtColor(im2,cv2.COLOR_BGR2GRAY)
ret, mask = cv2.threshold(im2gray,10,255,cv2.THRESH_BINARY)
mask_inv = cv2.bitwise_not(mask)

img1_bg = cv2.bitwise_and(roi,roi,mask=mask_inv)

cv2.imshow('img1_bg',img1_bg)

im2_fg = cv2.bitwise_and(im2,im2,mask=mask)

cv2.imshow('im2_fg',im2_fg)

dst = cv2.add(img1_bg, im2_fg)

im1[0:rows,0:cols] = dst 

cv2.imshow('res',im1)
cv2.waitKey(0)
cv2.destroyAllWindows()
