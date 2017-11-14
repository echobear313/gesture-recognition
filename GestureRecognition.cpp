#ifndef GESTURERECOGNITION_CPP
#define GESTURERECOGNITION_CPP
#include "GestureRecognition.h"
#include "FunctionTools.h"
#include "CompareImage.h"


// 构造函数
Gesture::Gesture(Mat& image)
{
	_imgRaw = image.clone();
	_contours = vector<Contour>();
}

// 显示原图
void Gesture::Show()
{
	imshow("原图", _imgRaw);
	
	//imshow("二值图", _imgBin);
}

// 将imgRaw转换为二值图，并存储在imgBin中
void Gesture::_GetBinaryImage(Mat& imgBin)
{

	cvtColor(_imgRaw, imgBin, CV_BGR2GRAY);
	//// 二值化
	threshold(imgBin, imgBin, 100, 255, 0);

	Mat ycrcb;
	Mat imb;

	//YCrCb
	cvtColor(_imgRaw,ycrcb,CV_BGR2YCrCb);
	vector<Mat> a;
	split(ycrcb,a);
	Mat y=a[0];
	Mat cr=a[1];
	Mat cb=a[2];
	inRange(cr,140,175,a[1]);
	inRange(cb,100,120,a[2]);
	bitwise_and(a[1],a[2],imb);

	////HSV 
	Mat hsv;
	Mat imb2;
	cvtColor(_imgRaw,hsv,CV_BGR2HSV);
	vector<Mat> b;
	split(hsv,b);
	Mat h = b[0];
	Mat s = b[1];
	Mat v = b[2];

	inRange(h,0,25,b[0]);
	inRange(s,26,200,b[1]);
	bitwise_and(b[0],b[1],imb2);

	bitwise_and(imb2,imb,_imgBin);
	bitwise_and(imgBin,_imgBin,_imgBin);
	dilate(imb, imb, Mat(5, 5, CV_8UC1), Point(-1, -1));
	// 腐蚀
	erode(imb, imb, Mat(5, 5, CV_8UC1), Point(-1, -1));
}

// 获取图中所有手的轮廓
void Gesture::_MaxContourIndex(vector<Contour>& contours)
{

	// 该轮廓必须非空
	assert(!contours.empty());

	for (int i = 0; i < (int)contours.size(); i++)
	{
		double thisArea = contourArea(contours[i]);
		if (thisArea > 8000)
		{
			
			_Contour.push_back(contours[i]);
		}
	}
	assert(_Contour.size() >= 0);
	assert(_Contour.size() <= (int)contours.size());
}

// 返回某个轮廓内最大圆的圆心P及半径R
void Gesture::_MaxCircleInContour(Mat& imgBin, const Contour& contour, Point& P, double& R)
{
	P = Point(-1, -1);
	R = -1;
	for (int i = 0; i < imgBin.rows; i += 40)
		for (int j = 0; j < imgBin.cols; j += 40)
			if (imgBin.at<uchar>(i, j) == 255)
			{
				double minDist = 1e66;
				for (int k = 0; k < (int)contour.size(); k += 4)
				{
					double thisDist = dist2(contour[k], Point(j, i));
					if (thisDist < minDist)
					{
						minDist = thisDist;
					}
				}
				if (R < minDist)
				{
					R = minDist;
					P = Point(j, i);
				}
			}
	if (R > 0) R = sqrtf(R);
}

// 返回轮廓是否与边界接触
bool Gesture::_TouchedWithBoundary(const Contour& contour, int maxRow, int maxCol)
{
	int fixed = 5;
	for(int i=0;i<(int)contour.size();i++)
	{
		const Point &p = contour[i];
		if(p.x < fixed || p.x > maxCol-fixed || p.y < fixed || p.y > maxRow-fixed)
			return 1;
	}
	return 0;
}

// 寻找手掌
bool Gesture::GetPalmImage()
{
	// 二值化
	_GetBinaryImage(_imgBin);

	// 在二值图中找出所有的轮廓
	Mat imgBin = _imgBin.clone();
	findContours(imgBin, _contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// 若没有轮廓，返回false
	if (_contours.empty())
	{
		std::cerr << "[错误] 未找到轮廓。" << std::endl;
		return false;
	}
	// 【蓝色】所有找到的轮廓
	drawContours(_imgRaw, _contours, -1, Scalar(255, 0, 0), 2, 8);

	// 找到图中手的轮廓
	_MaxContourIndex(_contours);
	// 【绿色】手的轮廓
	drawContours(_imgRaw, _Contour, -1, Scalar(0, 255, 0), 2, 8);

	// 在二值图中画出填充的手的轮廓
	_imgBin = Mat::zeros(_imgRaw.rows, _imgRaw.cols, CV_8UC1);
	drawContours(_imgBin, _Contour, -1, Scalar(255, 255, 255), -1, 8);

	//对每一个手的轮廓进行处理
	for(int i=0;i<(int)_Contour.size();i++)
	{	
		//取出一段手
		imgBin = Mat::zeros(_imgRaw.rows, _imgRaw.cols, CV_8UC1);
		drawContours(imgBin, _Contour, i, Scalar(255, 255, 255), -1, 8);

		// 找到手的轮廓内最大圆的圆心P及半径R
		Point P;
		double R;
		_MaxCircleInContour(imgBin, _Contour[i], P, R);
		if (R < 0)
		{
			std::cerr << "[错误] 未找到圆心及半径。" << std::endl;
			return false;
		}

		// 记录中心点及半径
		_Center[i] = P;
		_CenterRadius[i] = R;

		// 用背景色（黑色）填充手掌中的圆
		circle(imgBin, P, (int)R * 2, Scalar(0, 0, 0), -1);
		// imgBin 删去圆   _imgBin 完整手臂

		// 【紫色】最大圆及圆心
		circle(_imgRaw, P, (int)R * 2, Scalar(255, 0, 255), 4);
		circle(_imgRaw, P, 10, Scalar(255, 0, 255), -1);
	
		// 寻找此时中的轮廓
		vector<Contour> newContours;
		findContours(imgBin, newContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		if (newContours.empty())
		{
			std::cerr << "[错误] 填充最大圆后，未找到轮廓。" << std::endl;
			//return false;
		}
		else
		{
			// 删除所有与边界接触，且面积大于阈值的轮廓下标
			// 删除方式为，在内部二值图，用黑色填充
			double area_level = acos(-1) * R * R * 0.25;
			for(int j=0;j<(int)newContours.size();j++)
			{
				Contour &thisContour = newContours[j];

				if(_TouchedWithBoundary(thisContour, imgBin.rows, imgBin.cols))// && contourArea(thisContour) > area_level)
				{
					drawContours(_imgBin, newContours, j, Scalar(0, 0, 0), -1, 8);
				}
			}
		}
	}

	// 再次寻找手轮廓
	imgBin = _imgBin.clone();
	_Contour.clear();
	findContours(imgBin, _Contour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//if (_contours.size() != 1u)
	//{
	//	std::cerr << "[错误] 再次寻找手轮廓时出错，此时应该有且只有一个轮廓。" << std::endl;
	//	return false;
	//}
	//if (_contours[0].empty())
	//{
	//	std::cerr << "[错误] 再次寻找手轮廓时出错，找到唯一一个轮廓但是为空。" << std::endl;
	//	return false;
	//}

	// 【红色】手掌轮廓
	drawContours(_imgRaw, _Contour, -1, Scalar(0, 0, 255), 4, 8);
	return true;
}

// 计算不同图片间相似度
int Gesture::_Test_Result(vector<Contour> imgSource, vector<Contour> imghand, double** Results, int* ObjectNumber)
{

    LANDMARKS landmarks1, landmarks2;

    allContData1.number = imgSource.size();
    allContData1.cont_data = new CONTOUR_DATA [allContData1.number];


    landmarks1.LandmarksNumb = new int [allContData1.number];
    landmarks1.ContNumb = allContData1.number;
    landmarks1.Landmarks = new cv::Point *[allContData1.number];

    landmarks1.MatchingPnts = new cv::Point *[allContData1.number];


    allContData2.number = imghand.size();
    allContData2.cont_data = new CONTOUR_DATA [allContData2.number];

    landmarks2.LandmarksNumb = new int [allContData2.number];
    landmarks2.ContNumb = allContData2.number;
    landmarks2.Landmarks = new cv::Point *[allContData2.number];

    for (size_t k = 0; k < imgSource.size(); k++)
    {

        allContData1.cont_data[k].PntsNumb = imgSource[k].size();
        allContData1.cont_data[k].point = new cv::Point[allContData1.cont_data[k].PntsNumb];

        obtainInfoFrmOneCont(imgSource[k], k, allContData1);

        landmarks1.LandmarksNumb[k] = Landmarks_Numb;
        landmarks1.Landmarks[k] = new cv::Point[allContData1.cont_data[k].LandmarksNumb];

        //landmarks1.MatchingPnts[k] = new cv::Point [allContData2.cont_data[k].LandmarksNumb];

        for (int i = 0; i < landmarks1.LandmarksNumb[k]; i++)
        {
            landmarks1.Landmarks[k][i].x = allContData1.cont_data[k].point[allContData1.cont_data[k].indexLandmarks[i]].x;
            landmarks1.Landmarks[k][i].y = allContData1.cont_data[k].point[allContData1.cont_data[k].indexLandmarks[i]].y;
        }
    }



    for (size_t k = 0; k < imghand.size(); k++)
    {

        allContData2.cont_data[k].PntsNumb = imghand[k].size();
        allContData2.cont_data[k].point = new cv::Point[allContData2.cont_data[k].PntsNumb];

        obtainInfoFrmOneCont(imghand[k], k, allContData2);
        landmarks2.LandmarksNumb[k] = Landmarks_Numb;
        landmarks2.Landmarks[k] = new cv::Point[allContData2.cont_data[k].LandmarksNumb];


        for (int i = 0; i < landmarks2.LandmarksNumb[k]; i++)
        {
            landmarks2.Landmarks[k][i].x = allContData2.cont_data[k].point[allContData2.cont_data[k].indexLandmarks[i]].x;
            landmarks2.Landmarks[k][i].y = allContData2.cont_data[k].point[allContData2.cont_data[k].indexLandmarks[i]].y;
        }
    }

    //matching
    *Results = new double[landmarks1.ContNumb];


    MatchShapes(landmarks1, landmarks2, Results);
    *ObjectNumber = landmarks1.ContNumb;

    //Mat outImg;
    //
    //DrawLinesInOneImage(srcImg1, srcImg2, landmarks1, landmarks2, outImg);


    //imshow("test",outImg);

    /*waitKey(0);*/

    for (size_t k = 0; k < imghand.size(); k++)
    {
        delete [] landmarks2.Landmarks[k];
        delete [] allContData2.cont_data[k].point;
        delete [] allContData2.cont_data[k].indexLandmarks;
    }
    delete [] landmarks2.Landmarks;
    delete [] landmarks2.LandmarksNumb;
    delete [] allContData2.cont_data;


    for (size_t k = 0; k < imgSource.size(); k++)
    {
        delete [] landmarks1.Landmarks[k];
        delete [] allContData1.cont_data[k].point;
        delete [] allContData1.cont_data[k].indexLandmarks;
        delete [] landmarks1.MatchingPnts[k];
    }
    delete [] landmarks1.Landmarks;
    delete [] landmarks1.LandmarksNumb;
    delete [] allContData1.cont_data;
    delete [] landmarks1.MatchingPnts;
    return 0;
}

// 识别
vector<int> Gesture::Recognize(vector<Contour>& imgSource)
{
	double *results;
	int ObjectNumber;
	for(int i=0;i<(int)_Contour.size();i++)
	{	
		//取出一段手
		vector<Contour>imghand;
		imghand.push_back(_Contour[i]);
		_Test_Result(imgSource, imghand, &results, &ObjectNumber);
		// 找出最接近的图片下标
		double minDist = 1e99;
		int idxMin = -1;
		for (int j = 0; j < ObjectNumber; j++)
		{
			//printf("Object[%d] %.10f\n", i, results[i]);
			if (results[j] < minDist)
			{
				minDist = results[j];
				idxMin = j;
			}
		}
		assert(idxMin >=0);
		handname.push_back(idxMin);
		if(idxMin == 3)
		{
			zero_gesture.push_back(i);
		}
		if(idxMin == 5 || idxMin == 7)
		{
			eight_left.push_back(i);
		}
		if(idxMin == 1 || idxMin == 2)
		{
			eight_right.push_back(i);
		}

	}
	//cout<<"3"<<endl;
	//matchGesture8();
	//cout<<"4"<<endl;
	return handname;
}

// 寻找指尖
void Gesture::FindFingerTips()
{


	// 寻找指尖
	assert(_Contour.size() < 100);
	for(int index=0;index<(int)_Contour.size();index++)
	{
		Contour &contourPoints = _Contour[index];
		_FingerTips[index].clear();

		// 寻找每一个角度较大的点
		vector<int> idxs;
		int pStep = 20;
		for (int i = 0; i < (int)contourPoints.size(); i++)
		{
			// 选取一定间隔的三个点
			int j = (i + pStep) % contourPoints.size();
			int k = (i + pStep * 2) % contourPoints.size();
			Point &p1 = contourPoints[i];
			Point &p2 = contourPoints[j];
			Point &p3 = contourPoints[k];
			// 构成两个向量
			Point vec_p1p2 = Point(p2.x - p1.x, p2.y - p1.y);
			Point vec_p2p3 = Point(p3.x - p2.x, p3.y - p2.y);
			// 计算夹角
			double dot = vec_p1p2.x * vec_p2p3.x + vec_p1p2.y * vec_p2p3.y;
			double xmult = vec_p1p2.x * vec_p2p3.y - vec_p1p2.y * vec_p2p3.x;
			double len_vec_p1p2 = sqrtf(dist2(p1, p2));
			double len_vec_p2p3 = sqrtf(dist2(p2, p3));
			double cos_val = 1. * dot / len_vec_p1p2 / len_vec_p2p3;
			//double angle = acos(cos_val) / acos(-1) * 180;
			// 符合要求则将p2的下标存入idxs
			// 余弦值小于0.3 叉积方向为逆时针 位于最大圆外侧
			if (cos_val < 0.3 && xmult < 0 && sqrtf(dist2(_Center[index], p2)) > _CenterRadius[index] * 2)
			{
				// idxs为空时直接放入
				if(idxs.empty())
				{
					idxs.push_back(j);
				}
				// 否则判断与上一点的下标是否变化过大
				else
				{
					
					int pre_idx = *idxs.rbegin();
					// 过大 则判定当前idxs为一个指尖集合
					if(j - pre_idx > pStep)
					{
						int idx_middle = idxs[idxs.size() / 2];
						_FingerTips[index].push_back(contourPoints[idx_middle]);
						idxs.clear();
					}
					// 否则继续添加
					else
					{
						idxs.push_back(j);
					}
				}
			}
		}
		// 处理idxs非空的情况
		if(!idxs.empty())
		{
			int sz_idxs = idxs.size();
			int idx_middle = idxs[sz_idxs / 2];
			_FingerTips[index].push_back(contourPoints[idx_middle]);
			idxs.clear();
		}
		//  【绿色】画出所有的指尖
		for (int j = 0; j < (int)_FingerTips[index].size(); j++)
		{
			circle(_imgRaw, _FingerTips[index][j], 10, Scalar(0, 255, 0), -1);
		}
	}	


}

//输出图片识别结果
void Gesture::SetText(string& txt,int& i)
{
	putText(_imgRaw, txt, _Center[i],CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255) );
}
// 返回中心点
Point* Gesture::GetCenterPoint()
{
	return _Center;
}

// 返回指尖点
vector<Point>* Gesture::GetFingerTipPoints()
{
	return _FingerTips;
}

//地图变换
// bool Gesture::mapChange(Mapchange& map,int* CloseNum)
// {
// 	/*cout<<"mapchange"<<endl;*/

// 	if(handname.empty()||_Contour.size()!=handname.size())
// 	{
// 		cout<<"handname!=countours"<<endl;
// 		return 0;
// 	}
// 	//if( CloseNum[5] >= 5 && pictureOpenFlag == 0) //手势为5，打开地图
// 	//{
// 	//		openPicture(image,Row,Col,_Center[0]);
// 	//		pictureOpenFlag = 1;		
// 	//}

// 	//if(/*_Contour.size()==1 && handname[0] == 0 */CloseNum[0] >= 5 && pictureOpenFlag == 1) //手势为0，关闭地图
// 	//{
// 	//	cout<<"close"<<endl;
// 	//	closePicture();
// 	//	pictureOpenFlag = 0;
// 	//}

// 	/*if(eight_left.size() == eight_right.size() && CloseNum[6] >= 5 && CloseNum[7] >= 5)*/ //双手为8，地图整体放大缩小
// 	if(_Contour.size()==2 && handname[0] == 1 && handname[1] == 1 && CloseNum[0] >=5)
// 	{
// 		int dis = abs(_Center[0].x - _Center[1].x);
// 		map.pre_dis = dis;
// 	}
// 	if(_Contour.size()==2 && handname[0] == 2 && handname[1] == 2 && CloseNum[1] >= 5)//双手为1，地图整体放大缩小
// 	{
// 		/*cout<<"changesize"<<endl;*/
// 		int dis = abs(_Center[0].x - _Center[1].x);
// 		if(map.pre_dis == -1.0)
// 			map.pre_dis = dis;
// 		else if(map.pre_dis < dis)//放大
// 		{
// 			map.point.x = dis/2.0/640*3200;
// 			map.point.y =  abs(_Center[0].y - _Center[1].y)/2.0/480*1800;
// 			map.MapEnlarge();
// 			map.pre_dis = dis;
// 		}
// 		else if(map.pre_dis > dis)//缩小
// 		{
// 			map.point.x = dis/2.0/640*3200;
// 			map.point.y =  abs(_Center[0].y - _Center[1].y)/2.0/480*1800;
// 			map.MapNarrow();
// 			map.pre_dis = dis;
// 		}

// 		/*cout<<"changesize2"<<endl;*/
// 	}
// 	if(_Contour.size()==1 && handname[0] == 1 && CloseNum[0] >= 5) //手势0，地图不变化
// 	{
// 		map.pre_y = _Center[0].y;
// 		map.pre_x = _Center[0].x;
// 	}
// 	if(_Contour.size()==1 && handname[0] == 2 && CloseNum[1] >= 5) //手势1，地图上下移动
// 	{
// 		/*cout<<"updown"<<endl;*/

// 		if(map.pre_y== -1) 
// 			map.pre_y = _Center[0].y;
// 		else if(map.pre_y > _Center[0].y)//向下
// 		{
// 			map.MapMoveup();
// 			map.pre_y = _Center[0].y;
// 		}
// 		else if(map.pre_y < _Center[0].y)//向上
// 		{
// 			map.MapMovewdown();
// 			map.pre_y = _Center[0].y;
// 		}
				
// 		//if(map.pre_x == -1)
// 		//	map.pre_x = _Center[0].x;
// 		//else if(map.pre_x < _Center[0].x)//向右
// 		//{
// 		//	map.MapMoveright();
// 		//	map.pre_x = _Center[0].x;
// 		//}
// 		//else if(map.pre_x > _Center[0].x)//向左
// 		//{
// 		//	map.MapMoveleft();
// 		//	map.pre_x = _Center[0].x;
// 		//}

// 		/*cout<<"updown2"<<endl;*/
		
// 	}
// 	if(_Contour.size()==1 && (handname[0] == 0 || handname[0] == 3) && CloseNum[5] >= 5) //手势5，地图左右移动
// 	{
// 		/*cout<<"leftright"<<endl;*/
		
// 		if(map.pre_x == -1)
// 			map.pre_x = _Center[0].x;
// 		else if(map.pre_x < _Center[0].x)//向右
// 		{
// 			map.MapMoveright();
// 			map.pre_x = _Center[0].x;
// 		}
// 		else if(map.pre_x > _Center[0].x)//向左
// 		{
// 			map.MapMoveleft();
// 			map.pre_x = _Center[0].x;
// 		}

// 		/*cout<<"leftright2"<<endl;*/
		
// 	}
// 	//cout<<"change2"<<endl;
// 	return 0;
// }
// 析构函数
Gesture::~Gesture()
{
	_imgRaw.release();
	_imgBin.release();
}

#endif