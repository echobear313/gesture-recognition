#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
// #include "MapChange.h"
#include <iostream>
using namespace cv;

typedef vector<Point> Contour;


// 手势类
// 下划线开头的变量/函数为私有
class Gesture {
protected:
	// 图像原图，二值图
	Mat _imgRaw, _imgBin;
	// 图像中所有的轮廓
	vector<Contour> _contours;
private:
	// 中心点 半径
	Point _Center[100];
	double _CenterRadius[100];
	vector<int> handname;
	//存储图中手的轮廓
	vector<Contour> _Contour;
	// 指尖点
	vector<Point> _FingerTips[100];
	//存储8_left,8_right的轮廓序号
	vector<int> eight_left;
	vector<int> eight_right;
	//存储手势0的轮廓序号
	vector<int> zero_gesture;
	//匹配上的手势8
	vector<Point> matchGesture;
	// imgRaw转换为二值图，存储在imgBin中
	void _GetBinaryImage(Mat& imgBin);
	// 返回某个轮廓集合中，面积最大的轮廓的下标
	void _MaxContourIndex(vector<Contour>& contours);
	// 返回某个轮廓内最大圆的圆心及半径
	void _MaxCircleInContour(Mat& imgBin, const Contour& contour, Point& P, double& R);
	// 计算不同图片间相似度
	int _Test_Result(vector<Contour> imgSource, vector<Contour> imghand, double** Results, int* ObjectNumber);
	// 返回轮廓是否与边界接触
	bool _TouchedWithBoundary(const Contour& contour, int maxRow, int maxCol);
	//打开图片
	void openPicture(Mat image,double Row,double Col,Point P,const string& winname);
    //关闭图片
	void closePicture(const string& winname);
    //整体放大
	void resizeDemo(Mat& image,Point P1,Point P2,const string& winname);
	//局部放大，这个目前不能再原图上放大，但是可以新开窗口局部放大
	void magnifyPart(Mat image,Point P,int dis_x,int dis_y,string& smallwinname);
	//移动图片
	void movePicture(Mat image,Point pt,const string& winname,int pictureOpenFlag);
	//对当前的左右手进行匹配
	void matchGesture8();
public:
	// 构造函数
	Gesture(Mat& image);
	// 显示原图
	void Show();
	// 寻找手掌
	bool GetPalmImage();
	// 识别
	vector<int> Recognize(vector<Contour>& imgSource);
	// 寻找指尖
	void FindFingerTips();
	//输出识别结果
	void SetText(string& txt, int& i);
	// 返回中心点
	Point* GetCenterPoint();
	// 返回指尖
	vector<Point>* GetFingerTipPoints();
	//图片变化
	bool pictureChange(Mat image,int pictureOpenFlag,double Row,double Col,int* CloseNum,vector<Point>& smallwindow_num,int& windownum);
	//地图变化
	// bool mapChange(Mapchange& map,int* CloseNum);
	// 析构函数
	~Gesture();
};
