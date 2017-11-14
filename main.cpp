#define _CRT_SECURE_NO_WARNINGS
#include "GestureRecognition.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
int pictureOpenFlag = 0;

int main()
{
	// Mat image, imgRenew;
	// image = imread("../pictures/2.jpg");

	// 识别模板
	Mat imgSrc = imread("Pictures/111.jpg");
	//string name[8] = {"0", "5", "1", "8_right","8_left","4", "3", "2"};
	/*string name[10] = {"8_right","8_right","5","0","1","8_left", "5", "8_left"};*/
	string name[4] = {"5","0","1","5"};


	Mat imb;
	cvtColor(imgSrc, imb, CV_BGR2GRAY);
	threshold(imb, imb, 100, 255, CV_THRESH_TOZERO);
	vector<Contour>imgSrc_contours;
	findContours(imb, imgSrc_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	//drawContours(imgSrc,imgSrc_contours,2,Scalar(255,0,0),4);
	//imshow("123",imgSrc);
	//waitKey(0);

	//地图变换
	// Mapchange map;
	//string NotExist = "XXX";

	vector<Point> smallwindow_num;
	int windownum = 0;
	// 连接视频设备（编号为0）
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		std::cerr << "[错误] 连接视频设备失败." << std::endl;
		return -1;
	}

	// 前一次识别的结果
	int pre_idxGesture = -99;

	int OpCloNum[8] = {0};//各个手势的持续帧数
	// 处理视频
	do
	{
		bool HasGesture[8] = {false};//当前帧中手否有手势i
		// 获取当前帧
		Mat frame;
		cap >> frame;
		//Mat frame = imread("../Pictures/sample.jpg");
		if(frame.empty())
		{
			continue;
		}
		////ROI大小
		//cv::Rect roi(0, 0, 640, 380);
		//cv::Mat frame_roi = frame(roi);

		// 对当前帧画面创建一个手势识别对象
		Gesture GestureObj(frame);

		// 若成功获取手掌图像
		if (GestureObj.GetPalmImage() == true)
		{
			// 以imgSrc为源进行识别，获得对应图片所在下标
			vector<int> idxGesture = GestureObj.Recognize(imgSrc_contours);
			if(idxGesture.empty())
			{
				puts("识别失败");
				continue;
			}

			// 显示结果
			for(int i=0;i<idxGesture.size();i++) {
				assert(idxGesture[i] >= 0);
				if(name[idxGesture[i]] == "8_left")
				{
					HasGesture[6] = true;
				}
				else if(name[idxGesture[i]] == "8_right")
				{
					HasGesture[7] = true;
				}
				else if(name[idxGesture[i]] == "0")
				{
					HasGesture[0] = true;
				}
				else if(name[idxGesture[i]] == "1")
				{
					HasGesture[1] = true;
				}
				else if(name[idxGesture[i]] == "5")
				{
					HasGesture[5] = true;
				}
				GestureObj.SetText(name[idxGesture[i]],i);
			}
			for(int i=0;i<8;i++)
			{
				
				if(HasGesture[i] == false)
				{
					OpCloNum[i] = 0;
				}
				else
				{
					OpCloNum[i]++;
				}
			}
			
////	/*		else
//			{
//				GestureObj.SetText(NotExist);
//			}
//*/
			// 当手势变化时，更新识别结果
			/*if (idxGesture != pre_idxGesture)
			{
				pre_idxGesture = idxGesture;
				if (idxGesture != -1)
				{
					std::cout << "[识别] " << name[idxGesture] << std::endl;
				}
				else
				{
					std::cout << "[识别] 无对应手势。" << std::endl;
				}
			}*/

			// 寻找指尖
			/*GestureObj.FindFingerTips();*/
			//地图变换
			// GestureObj.mapChange(map,OpCloNum);
		}

		// 显示图片
		GestureObj.Show();
		//waitKey();
	} while(waitKey(30) != 13);

	return 0;
}
