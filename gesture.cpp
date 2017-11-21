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
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20)));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20)));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)));
}



int getMaxContours(const cv::Mat mask, std::vector<std::vector<cv::Point> >& contours) {
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
    return index;
}

cv::Scalar getMSSIM(const cv::Mat& i1, const cv::Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d     = CV_32F;

    cv::Mat I1, I2;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    cv::Mat I2_2   = I2.mul(I2);        // I2^2
    cv::Mat I1_2   = I1.mul(I1);        // I1^2
    cv::Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    cv::Mat mu1, mu2;   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

    cv::Mat mu1_2   =   mu1.mul(mu1);
    cv::Mat mu2_2   =   mu2.mul(mu2);
    cv::Mat mu1_mu2 =   mu1.mul(mu2);

    cv::Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    ///////////////////////////////// FORMULA ////////////////////////////////
    cv::Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    cv::Mat ssim_map;
    divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

    cv::Scalar mssim = mean( ssim_map ); // mssim = average of ssim map
    return mssim;
}

/**
 * 加载模版
 * @param t
 */
void load_template(std::vector<cv::Mat>& t){
    auto im1 = cv::imread("/Users/HZzone/Desktop/gesture-recognition/pic/templates/1.jpg");
    auto im2 = cv::imread("/Users/HZzone/Desktop/gesture-recognition/pic/templates/2.jpg");
    auto im3 = cv::imread("/Users/HZzone/Desktop/gesture-recognition/pic/templates/3.jpg");
    auto im4 = cv::imread("/Users/HZzone/Desktop/gesture-recognition/pic/templates/4.jpg");
    cv::cvtColor(im1, im1, cv::COLOR_RGB2GRAY);
    cv::cvtColor(im2, im2, cv::COLOR_RGB2GRAY);
    cv::cvtColor(im3, im3, cv::COLOR_RGB2GRAY);
    cv::cvtColor(im4, im4, cv::COLOR_RGB2GRAY);
    t.push_back(im1);
    t.push_back(im2);
    t.push_back(im3);
    t.push_back(im4);
}

/*
 * 如果相似度小于0.8，返回-1,否则返回模版的下标
 */
int getMaxSimilarityIndex(const cv::Mat& im){
    double max_similarity = 0;
    int max_index = -1;
    for(int i=0; i<gesture_templates.size();i++) {
        cv::Mat new_image;
        cv::resize(gesture_templates[i], new_image, im.size());
        auto x = getMSSIM(new_image, im);
        std::cout<<x[0]<<std::endl;
        if(max_similarity < x[0]){
            max_similarity = x[0];
            max_index = i;
        }
    }
    std::cout<<"max_similarity: "<<max_similarity<<std::endl;
    if(max_similarity<0.8)
        max_index = -1;
    return max_index;
}
void show(const cv::Mat& im){
    cv::imshow(" ", im);
    cv::waitKey(0);
}


