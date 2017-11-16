//
// Created by Hzzone on 2017/11/16.
//

#include "gesture.h"

void getMask(const cv::Mat& im, cv::Mat& mask){
    assert(!im.empty());
    cv::Mat hsv;
    cv::medianBlur(im, im, 5);
    cv::cvtColor(im, hsv, cv::COLOR_RGB2HSV);
    cv::inRange(hsv, cv::Scalar(100, 50, 0), cv::Scalar(125, 255, 255), mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
}



int getMaxContours(const cv::Mat& mask, std::vector<std::vector<cv::Point> >& contours) {
    assert(!mask.empty());
    //寻找轮廓
    findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    // 找到最大的轮廓
    int index;
    double maxArea = 0;
    for (int i = 0; i < contours.size(); i++) {
        double area = contourArea(cv::Mat(contours[i]));
        if (area > maxArea) {
            maxArea = area;
            index = i;
        }
    }
    // 显示轮廓
//    cv::drawContours(im, contours, index, cv::Scalar(0, 0, 255), 2, 8);
//    show(im);
    return index;
}
