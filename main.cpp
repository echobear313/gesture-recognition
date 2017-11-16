#include "opencv2/opencv.hpp"
#include <assert.h>
#include "gesture.h"
#include <math.h>

void show(const cv::Mat& im){
    cv::imshow(" ", im);
    cv::waitKey(0);
}

int main(){
    cv::Mat im = cv::imread("/Users/HZzone/Desktop/2.jpg");
    cv::Mat mask, show_img;
    std::vector<std::vector<cv::Point> > contours;
    getMask(im, mask);
    im.copyTo(show_img, mask);
    auto index = getMaxContours(mask, contours);
    cv::Moments moment = cv::moments(mask, true);
    cv::Point center(moment.m10/moment.m00, moment.m01/moment.m00);
    cv::circle(show_img, center, 8 ,cv::Scalar(0, 0, 255), CV_FILLED);
    // 寻找指尖
    std::vector<cv::Point> couPoint = contours[index];
    std::vector<cv::Point> fingerTips;
    cv::Point tmp;
    int max = 0, count = 0, notice = 0;
    for (int i = 0; i < couPoint.size(); i++)
    {
        tmp = couPoint[i];
        int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
        if (dist > max)
        {
            max = dist;
            notice = i;
        }

        // 计算最大值保持的点数，如果大于40（这个值需要设置，本来想根据max值来设置，
        // 但是不成功，不知道为何），那么就认为这个是指尖
        if (dist != max)
        {
            count++;
            if (count > 40)
            {
                count = 0;
                max = 0;
                bool flag = false;
                // 低于手心的点不算
                if (center.y < couPoint[notice].y )
                    continue;
                // 离得太近的不算
                for (int j = 0; j < fingerTips.size(); j++)
                {
                    if (std::abs(couPoint[notice].x - fingerTips[j].x) < 20)
                    {
                        flag = true;
                        break;
                    }
                }
                if (flag) continue;
                fingerTips.push_back(couPoint[notice]);
                circle(show_img, couPoint[notice], 6 ,cv::Scalar(0, 255, 0), CV_FILLED);
                line(show_img, center, couPoint[notice], cv::Scalar(255, 0, 0), 2);
            }
        }
    }

    cv::imshow("show_img", show_img);
    cv::waitKey(0);

}
