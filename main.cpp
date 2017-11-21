#include "opencv2/opencv.hpp"
#include <assert.h>
#include "gesture.h"
#include <math.h>

std::vector<cv::Mat> gesture_templates;

int main(){
    /*
     * 因为全局变量，必须在最开始加载模版
     */
    load_template(gesture_templates);
    cv::Mat im = cv::imread("/Users/HZzone/Desktop/template_4.jpg");
    Gesture gesture(im);
}
