#include "gesture.h"

void getMask(const cv::gpu::GpuMat& im, cv::gpu::GpuMat& mask){
    assert(!im.empty());
    cv::gpu::GpuMat hsv;
    cv::medianBlur(im, im, 5);
    cv::cvtColor(im, hsv, cv::COLOR_RGB2HSV);
    cv::inRange(hsv, cv::Scalar(100, 50, 0), cv::Scalar(125, 255, 255), mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20)));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20)));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)));
}