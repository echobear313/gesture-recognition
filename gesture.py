#!/usr/bin/env python
import cv2
import numpy as np
def getContours(img):
    kernel = np.ones((5,5),np.uint8)
    closed = cv2.morphologyEx(img, cv2.MORPH_OPEN, kernel)
    closed = cv2.morphologyEx(closed, cv2.MORPH_CLOSE, kernel)
    contours, h  = cv2.findContours(closed, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    validContours = [];
    max_area = 0
    i = 0
    for index, cont in enumerate(contours):
        if cv2.contourArea(cont) > max_area:
            max_area = cv2.contourArea(cont)
            i = index

    validContours.append(contours[i])
    return validContours
def HSVBin(img):
    hsv = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)

    lower_skin = np.array([100, 50, 0])
    upper_skin = np.array([125, 255, 255])

    mask = cv2.inRange(hsv, lower_skin, upper_skin)
    # res = cv2.bitwise_and(img, img, mask=mask)
    print mask.shape
    return mask
if __name__ == '__main__':
    im = cv2.imread("pic/2.JPG")
    mask = HSVBin(im)
    contours = getContours(mask)
    cv2.drawContours(im, contours, -1, (0, 255, 0), 2, 8)
    cv2.imshow('capture', im)
    cv2.imwrite("pic/5.jpg", im)
    # cv2.imshow("", mask)
    cv2.waitKey(0)
