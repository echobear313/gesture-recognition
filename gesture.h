//
// Created by Hzzone on 2017/11/16.
//

#ifndef GESTURERECOGNITION_GESTURE_H
#define GESTURERECOGNITION_GESTURE_H
#include "opencv2/opencv.hpp"
#include <assert.h>

extern std::vector<cv::Mat> gesture_templates;
void getMask(const cv::Mat& im, cv::Mat& mask);

int getMaxContours(const cv::Mat mask, std::vector<std::vector<cv::Point> >& contours);

void show(const cv::Mat& im);

cv::Scalar getMSSIM(const cv::Mat& i1, const cv::Mat& i2);

void load_template(std::vector<cv::Mat>& t);

int getMaxSimilarityIndex(const cv::Mat& im);

class Gesture {
private:

    /**
     * 是否右手，默认为false
     */
    bool hasHand = false;

    /**
     * 手的种类
     */
    int gesture_category;

    /**
     * 最大轮廓的面积的阈值，来判断手是否存在，可能需要调整
     */
    double handAreaThreshold = 9000;
    /**
     * 指尖的位置，两种手势，分别大小是1和2；
     */
    std::vector<cv::Point> finger_location;

public:
    Gesture(cv::Mat& im){
        cv::Mat mask, show_img;
        std::vector<std::vector<cv::Point> > contours;
        // 得到mask
        getMask(im, mask);
//        im.copyTo(show_img, mask);
//        show(show_img);
        mask.copyTo(show_img);
//        show(show_img);
        // 最大轮廓的位置
        int index = getMaxContours(mask, contours);

        // 判断是否有手
        if(cv::contourArea(cv::Mat(contours[index])) > this->handAreaThreshold)
            hasHand = true;

        /**
         * 有手的话先判断那种手势，再然后根据不同的手势提取需要的指尖位置
         */
        if(hasHand) {

            // 手的轮廓 zeroMatrix
            cv::Mat zeroMatrix(mask.size(), CV_8UC1, cv::Scalar(0));
            cv::drawContours(zeroMatrix, contours, index, 255, -1);
            // 获取最大相似度的模版
            int max_index = getMaxSimilarityIndex(zeroMatrix);
            /*
             * 手势存在与模版中
             */
            if(max_index){
                this->gesture_category = max_index;
//                std::cout<<max_index<<std::endl;
                // 计算凸包，得到指尖的位置
                std::vector<cv::Point> hull;
                cv::convexHull(cv::Mat(contours[index]), hull, true);
                std::cout<<hull<<std::endl;
//                std::vector<std::vector<cv::Point> > hh;
//                hh.push_back(hull);
//                cv::drawContours(show_img, hh, -1, 255);
//                show(show_img);
            }

        }
    }

};

#endif //GESTURERECOGNITION_GESTURE_H
