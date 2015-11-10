import time 
import cv2

img = cv2.imread("Messi.jpg")

t1 = time.time()

res = cv2.medianBlur(img,49)

t2 = time.time()

tenabled = t2 - t1

cv2.setUseOptimized(False) 

t1 = time.time() 

res = cv2.medianBlur(img,49)

t2 = time.time()

tdisabled = t2 -t1

print "tenabled = " + str(tenabled) + "\n tdisabled = " + str(tdisabled)
