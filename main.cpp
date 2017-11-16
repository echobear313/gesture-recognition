#include "opencv2/opencv.hpp"

void show(const cv::Mat& im){
    cv::imshow(" ", im);
    cv::waitKey(0);
}
int main(){
    cv::Mat im = cv::imread("pic/2.JPG");
    cv::Mat hsv, mask;
    cv::medianBlur(im, im, 5);
    cv::cvtColor(im, hsv, cv::COLOR_RGB2HSV);
    cv::inRange(hsv, cv::Scalar(100, 50, 0), cv::Scalar(125, 255, 255), mask);
    std::vector<std::vector<cv::Point> > contours;
//    std::vector<cv::Vec4i> hierarchy;

    //寻找轮廓
    findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    std::cout<<contours.size()<<std::endl;
    // 找到最大的轮廓
    int index;
    double maxArea = 0;
    for (int i=0; i < contours.size(); i++)
    {
        double area = contourArea(cv::Mat(contours[i]));
        if (area > maxArea)
        {
            maxArea = area;
            index = i;
        }
    }
    cv::drawContours(im, contours, index, cv::Scalar(0, 0, 255), 2, 8);
    show(im);
    cv::imwrite("pic/6.jpg", im);
    cv::waitKey(0);
}
