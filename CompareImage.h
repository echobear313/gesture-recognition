#pragma once
/*
 Test file for shape space liberary only including Distance.
 Written by Han Yuexing 20160330
*/
#include <stdio.h>
#include <string>
#include <iostream>

#include <math.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

typedef struct  LANDMARKS
{
	cv::Point** Landmarks;           //Landmarks
	int ContNumb;					 //Number of contours 

	int*  LandmarksNumb;		     //Number of landmarks

	cv::Point** MatchingPnts;        //Matching points(other images), corresponding to "Landmarks"
	int MatchingNumb;               //The number of Matching points

}LANDMARKS;

typedef struct  CONTOUR_DATA
{
	double longestLine;              //The longest line in on the contour
	int PntsNumb;					 //Number of contour points
	cv::Point* point;		         //Contour points
	int  indexEndPoint[2];			 //Indexs of two points with longest line in a coutour
	int* indexLandmarks;             //Index of landmarks
	int  LandmarksNumb;				 //Number of landmarks
}CONTOUR_DATA;

typedef struct  ALL_CONTOUR_DATA{
	CONTOUR_DATA* cont_data;
	int number;						 // Number of all contours
}ALL_CONTOUR_DATA;

extern ALL_CONTOUR_DATA allContData1, allContData2;

#define Landmarks_Numb 30;

int CombineTwoImages_Top_Down1(Mat img1, Mat img2, Mat& img);
int CombineTwoImages_Left_Right(Mat img1, Mat img2, Mat& img);
int DrawLinesInOneImage(Mat img1, Mat img2, LANDMARKS landmarks1, LANDMARKS landmarks2, Mat& outImg);
int MatchShapes(LANDMARKS& landmarks1, LANDMARKS&  landmarks2, double** result);
int obtainInfoFrmOneCont(const  cv::vector<cv::Point> contours, int index, ALL_CONTOUR_DATA& allContData);
int Test_result(string InputImage1, string InputImage2, double** Results, int* ObjectNumber);
