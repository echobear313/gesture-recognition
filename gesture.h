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
        assert(!im.empty());
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
            if(max_index>=0){
                this->gesture_category = max_index;
//                std::cout<<max_index<<std::endl;
                // 计算凸包，得到指尖的位置
                std::vector<int> hull_int;
                std::vector<cv::Point> hull;
                cv::convexHull(cv::Mat(contours[index]), hull_int, true);
//                std::cout<<hull_int.size()<<std::endl;

                // 计算convexity defects, depth至少是20的点
                std::vector<cv::Vec4i> convexityDefects;
                cv::convexityDefects(cv::Mat(contours[index]), hull_int, convexityDefects);
//                std::cout<<convexityDefects.size()<<std::endl;
//                std::cout<<contours[index].size()<<std::endl;
                // 只保留终点
                for (auto x:convexityDefects) {
                    /*
                     * 这里的1000是保留的最大深度的凸缺陷的坐标的阈值，可能需要调整
                     */
                    if (x[3] > 1000) {
                        hull.push_back(contours[index][x[1]]);
//                        std::cout << contours[index][x[2]] << std::endl;
                    }
//                std::cout<<x<<std::endl;
                }
//                std::vector<std::vector<cv::Point> > hh;
//                hh.push_back(hull);
//                cv::drawContours(im, hh, -1, 255);
//                show(im);
                std::cout<<max_index<<std::endl;
                switch (max_index%2){
                    /**
                     * 八字
                     */
                    case 0: {
                        int finger_index1 = 0;
                        int finger_index2 = 0;
                        // 一个足够大的数
                        int min_d_1 = 10000000;
                        int min_d_2 = 10000000;
//                        std::cout<<im.size()<<std::endl;
                        cv::Point point1(0, 0);
                        cv::Point point2(im.size().width, 0);
//                        std::cout<<point2<<std::endl;
                        /*
                         * 左上角和右上角距离最小的点
                         */
                        for (int i = 0; i < hull.size(); i++) {
                            double d1 = powf((point1.x - hull[i].x),2) + powf((point1.y - hull[i].y),2);
                            double d2 = powf((point2.x - hull[i].x),2) + powf((point2.y - hull[i].y),2);
                            if (min_d_1 > d1){
                                finger_index1 = i;
                                min_d_1 = d1;
                            }
                            if (min_d_2 > d2){
                                finger_index2 = i;
                                min_d_2 = d2;
                            }
                            std::cout<<d1<<std::endl;
                            std::cout<<d2<<std::endl;
                            std::cout<<"-----"<<std::endl;
                        }
                        std::cout<<min_d_1<<std::endl;
                        finger_location.push_back(hull[finger_index1]);
                        finger_location.push_back(hull[finger_index2]);
//                        cv::circle(im, hull[finger_index1], 10, cv::Scalar(0, 0, 255));
//                        cv::circle(im, hull[finger_index2], 10, cv::Scalar(0, 0, 255));
//                        show(im);
                        break;
                    }
                    /**
                     * 只有食指
                     */
                    case 1: {
                        int finger_index = 0;
                        // 一个足够大的数
                        int min_y = 1000000;
                        for (int i = 0; i < hull.size(); i++) {
                            if (min_y > hull[i].y) {
                                finger_index = i;
                                min_y = hull[i].y;
                            }
                        }
                        finger_location.push_back(hull[finger_index]);
//                        cv::circle(im, hull[finger_index], 10, cv::Scalar(0, 0, 255));
//                        show(im);
                        break;
                    }
                }
            }
           else {
               std::cout<<"no gesture matched!"<<std::endl;
           }

        }
       else {
           std::cout<<"no hand has been found!"<<std::endl;
       }
    }

    bool hasHand(){
        return this->hasHand;
    }
    void getFingerLocation(std::vector<cv::Point>& v){
        assert(v.size()==0);
        v.assign(this->finger_location.begin(), this->finger_location.end());
    }

};

#endif //GESTURERECOGNITION_GESTURE_H
