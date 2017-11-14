#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
// #include "MapChange.h"
#include <iostream>
using namespace cv;

typedef vector<Point> Contour;


// ������
// �»��߿�ͷ�ı���/����Ϊ˽��
class Gesture {
protected:
	// ͼ��ԭͼ����ֵͼ
	Mat _imgRaw, _imgBin;
	// ͼ�������е�����
	vector<Contour> _contours;
private:
	// ���ĵ� �뾶
	Point _Center[100];
	double _CenterRadius[100];
	vector<int> handname;
	//�洢ͼ���ֵ�����
	vector<Contour> _Contour;
	// ָ���
	vector<Point> _FingerTips[100];
	//�洢8_left,8_right���������
	vector<int> eight_left;
	vector<int> eight_right;
	//�洢����0���������
	vector<int> zero_gesture;
	//ƥ���ϵ�����8
	vector<Point> matchGesture;
	// imgRawת��Ϊ��ֵͼ���洢��imgBin��
	void _GetBinaryImage(Mat& imgBin);
	// ����ĳ�����������У���������������±�
	void _MaxContourIndex(vector<Contour>& contours);
	// ����ĳ�����������Բ��Բ�ļ��뾶
	void _MaxCircleInContour(Mat& imgBin, const Contour& contour, Point& P, double& R);
	// ���㲻ͬͼƬ�����ƶ�
	int _Test_Result(vector<Contour> imgSource, vector<Contour> imghand, double** Results, int* ObjectNumber);
	// ���������Ƿ���߽�Ӵ�
	bool _TouchedWithBoundary(const Contour& contour, int maxRow, int maxCol);
	//��ͼƬ
	void openPicture(Mat image,double Row,double Col,Point P,const string& winname);
    //�ر�ͼƬ
	void closePicture(const string& winname);
    //����Ŵ�
	void resizeDemo(Mat& image,Point P1,Point P2,const string& winname);
	//�ֲ��Ŵ����Ŀǰ������ԭͼ�ϷŴ󣬵��ǿ����¿����ھֲ��Ŵ�
	void magnifyPart(Mat image,Point P,int dis_x,int dis_y,string& smallwinname);
	//�ƶ�ͼƬ
	void movePicture(Mat image,Point pt,const string& winname,int pictureOpenFlag);
	//�Ե�ǰ�������ֽ���ƥ��
	void matchGesture8();
public:
	// ���캯��
	Gesture(Mat& image);
	// ��ʾԭͼ
	void Show();
	// Ѱ������
	bool GetPalmImage();
	// ʶ��
	vector<int> Recognize(vector<Contour>& imgSource);
	// Ѱ��ָ��
	void FindFingerTips();
	//���ʶ����
	void SetText(string& txt, int& i);
	// �������ĵ�
	Point* GetCenterPoint();
	// ����ָ��
	vector<Point>* GetFingerTipPoints();
	//ͼƬ�仯
	bool pictureChange(Mat image,int pictureOpenFlag,double Row,double Col,int* CloseNum,vector<Point>& smallwindow_num,int& windownum);
	//��ͼ�仯
	// bool mapChange(Mapchange& map,int* CloseNum);
	// ��������
	~Gesture();
};
