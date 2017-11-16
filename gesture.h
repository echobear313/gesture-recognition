//
// Created by Hzzone on 2017/11/16.
//

#ifndef GESTURERECOGNITION_GESTURE_H
#define GESTURERECOGNITION_GESTURE_H
#include "opencv2/opencv.hpp"
#include <assert.h>

void getMask(const cv::Mat& im, cv::Mat& mask);

int getMaxContours(const cv::Mat& mask, std::vector<std::vector<cv::Point> >& contours);


#endif //GESTURERECOGNITION_GESTURE_H
