#include "opencv2/opencv.hpp"
#include <assert.h>
#include "gesture.h"
#include <math.h>

std::vector<cv::Mat> gesture_templates;

int main(){
    /*
     * 因为全局变量，必须在最开始加载模版, 不要重复加载，从磁盘加载花费很多时间
     */
    load_template(gesture_templates);
    cv::Mat im = cv::imread("/Users/HZzone/Desktop/template_3.jpg");
    Gesture gesture(im);
}
