#define _CRT_SECURE_NO_WARNINGS
#include "GestureRecognition.h"
#include <iostream>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
int pictureOpenFlag = 0;

int main()
{
	// Mat image, imgRenew;
	// image = imread("../pictures/2.jpg");

	// ʶ��ģ��
	Mat imgSrc = imread("/Users/HZzone/Desktop/Gesture_map/GestureRecognition/Pictures/111.jpg");
	//string name[8] = {"0", "5", "1", "8_right","8_left","4", "3", "2"};
	/*string name[10] = {"8_right","8_right","5","0","1","8_left", "5", "8_left"};*/
	string name[4] = {"5","0","1","5"};
	std:cout<< imgSrc.size() <<endl;
	imshow("hh", imgSrc);
	Mat imb;
	cvtColor(imgSrc, imb, CV_BGR2GRAY);
	threshold(imb, imb, 100, 255, CV_THRESH_TOZERO);
	vector<Contour>imgSrc_contours;
	findContours(imb, imgSrc_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	//drawContours(imgSrc,imgSrc_contours,2,Scalar(255,0,0),4);
	//imshow("123",imgSrc);
	//waitKey(0);

	//��ͼ�任
	// Mapchange map;
	//string NotExist = "XXX";

	vector<Point> smallwindow_num;
	int windownum = 0;
	// ������Ƶ�豸�����Ϊ0��
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		std::cerr << "[����] ������Ƶ�豸ʧ��." << std::endl;
		return -1;
	}

	// ǰһ��ʶ��Ľ��
	int pre_idxGesture = -99;

	int OpCloNum[8] = {0};//�������Ƶĳ���֡��
	// ������Ƶ
	do
	{
		bool HasGesture[8] = {false};//��ǰ֡���ַ�������i
		// ��ȡ��ǰ֡
		Mat frame;
		cap >> frame;
		//Mat frame = imread("../Pictures/sample.jpg");
		if(frame.empty())
		{
			continue;
		}
		////ROI��С
		//cv::Rect roi(0, 0, 640, 380);
		//cv::Mat frame_roi = frame(roi);

		// �Ե�ǰ֡���洴��һ������ʶ�����
		Gesture GestureObj(frame);

		// ���ɹ���ȡ����ͼ��
		if (GestureObj.GetPalmImage() == true)
		{
			// ��imgSrcΪԴ����ʶ�𣬻�ö�ӦͼƬ�����±�
			vector<int> idxGesture = GestureObj.Recognize(imgSrc_contours);
			if(idxGesture.empty())
			{
				puts("ʶ��ʧ��");
				continue;
			}

			// ��ʾ���
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
			// �����Ʊ仯ʱ������ʶ����
			/*if (idxGesture != pre_idxGesture)
			{
				pre_idxGesture = idxGesture;
				if (idxGesture != -1)
				{
					std::cout << "[ʶ��] " << name[idxGesture] << std::endl;
				}
				else
				{
					std::cout << "[ʶ��] �޶�Ӧ���ơ�" << std::endl;
				}
			}*/

			// Ѱ��ָ��
			/*GestureObj.FindFingerTips();*/
			//��ͼ�任
			// GestureObj.mapChange(map,OpCloNum);
		}

		// ��ʾͼƬ
		GestureObj.Show();
		//waitKey();
	} while(waitKey(30) != 13);

	return 0;
}
