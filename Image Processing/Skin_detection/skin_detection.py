#Human skin tone detection###
import cv2
from matplotlib import pyplot as plt
import numpy as np
import cv

img= cv2.imread("croped_skin.jpg",1)
cv2.imshow('s',img)
size= img.shape
b,g,r= cv2.split(img)
hsv=cv2.cvtColor(img, cv2.COLOR_BGR2YCR_CB)
h,s,v= cv2.split(hsv)
temp=np.zeros(size,img.dtype)
temp2=cv.CreateMat(size[0],size[1],cv.CV_32FC1)
"""for hsv
lower = np.array([0, 48, 80], dtype = "uint8")
upper = np.array([20, 255, 255], dtype = "uint8")
"""
lower = np.array([0,133, 77], dtype = "uint8")
upper = np.array([255, 173, 127], dtype = "uint8")

result= cv2.inRange(hsv,lower, upper)
#cv2.imshow('h',h)
#cv2.imshow('s',s)
#cv2.imshow('v',v)



print s
for i in range(0,size[0]):
    for j in range(0,size[1]):
        if(r[i,j]>95 and g[i,j]>40 and b[i,j]>20):
            if(  (max(r[i,j],g[i,j],b[i,j]) - min(r[i,j],g[i,j],b[i,j])) > 15  ):
                if ( (r[i,j]> g[i,j])  and  (r[i,j]> b[i,j]) and ((r[i,j]-g[i,j])>15) ):
                    temp[i,j]=255
        else:
            temp[i,j]=0
           
                   
##cv2.imshow('detect',result)

cv2.imshow('detect1',temp)
cv2.waitKey(0)
