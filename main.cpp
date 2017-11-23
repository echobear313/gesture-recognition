#include "opencv2/opencv.hpp"
#include <assert.h>
//#include "gesture.h"
#include <math.h>
#include "gestures_gpu.h"

std::vector<cv::Mat> gesture_templates;

int main(){
    /*
     * 因为全局变量，必须在最开始加载模版, 不要重复加载，从磁盘加载花费很多时间
     */
//    load_template(gesture_templates);
//    cv::Mat im = cv::imread("/Users/HZzone/Desktop/template_3.jpg");
//    Gesture gesture(im);

    /*
     * gpu version
     */
//    cv::Mat im = cv::imread("/home/hzzone/gesture-recognition/template_4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//    assert(!im.empty());
    cv::gpu::setDevice(0);
    assert(cv::gpu::getCudaEnabledDeviceCount()!=0);
    try
    {
        cv::Mat src_host = cv::imread("/home/hzzone/gesture-recognition/template_4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
        assert(!src_host.empty());
        cv::gpu::GpuMat dst, src;
        src.upload(src_host);
        assert(!src.empty());

        cv::gpu::cvtColor(src, dst, cv::COLOR_RGB2HSV);
//        dst = src.clone();

        cv::Mat result_host;
        dst.download(result_host);

        cv::imshow("Result", result_host);
        cv::waitKey();
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
//    cv::Mat result_host;
//    dst.download(result_host);

//    cv::imshow("Result", result_host);
//    cv::waitKey();


}
