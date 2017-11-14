#pragma once
#include "CompareImage.h"
#include "ShapeSpaceLib_Dist.h"
ALL_CONTOUR_DATA allContData1, allContData2;
int CombineTwoImages_Top_Down1(Mat img1, Mat img2, Mat& img)
{
	IplImage ImgIpl1;
	IplImage ImgIpl2;
	ImgIpl1 = IplImage(img1);
	ImgIpl2 = IplImage(img2);
	Mat temp;

	IplImage* expanded = cvCreateImage( cvSize( MAX((&ImgIpl1)->width, (&ImgIpl2)->width), (&ImgIpl1)->height + (&ImgIpl2)->height ),IPL_DEPTH_8U, 3 );  
 
	cvZero( expanded );  
    cvSetImageROI( expanded, cvRect( 0, 0, (&ImgIpl1)->width, (&ImgIpl1)->height ) );  
    cvAdd( (&ImgIpl1), expanded, expanded, NULL );  
    cvSetImageROI( expanded, cvRect(0, (&ImgIpl1)->height, (&ImgIpl2)->width, (&ImgIpl2)->height) );  
    cvAdd( (&ImgIpl2), expanded, expanded, NULL );  
    cvResetImageROI( expanded );  
	
	temp=Mat(expanded,0);

	img = temp.clone();

	cvReleaseImage(&expanded);
  
   	return 0;
}

int CombineTwoImages_Left_Right(Mat img1, Mat img2, Mat& img)
{
	IplImage ImgIpl1;
	IplImage ImgIpl2;
	ImgIpl1 = IplImage(img1);
	ImgIpl2 = IplImage(img2);

	Mat temp;

	IplImage* expanded = cvCreateImage( cvSize((&ImgIpl1)->width + (&ImgIpl2)->width,  MAX((&ImgIpl1)->height, (&ImgIpl2)->height)),IPL_DEPTH_32F, 3 );  
  
    cvZero( expanded );  

    cvSetImageROI( expanded, cvRect( 0, 0, (&ImgIpl1)->width, (&ImgIpl1)->height ) );  
    cvAdd( (&ImgIpl1), expanded, expanded, NULL );  
    cvSetImageROI( expanded, cvRect((&ImgIpl1)->width, 0, (&ImgIpl2)->width, (&ImgIpl2)->height) );  
	cout<<"1"<<endl;
    cvAdd( (&ImgIpl2), expanded, expanded, NULL );  
    cvResetImageROI( expanded );  
	temp=Mat(expanded,0);

	img=temp.clone();

	cvReleaseImage(&expanded);

	  
   	return 0;
}

int DrawLinesInOneImage(Mat img1, Mat img2, LANDMARKS landmarks1, LANDMARKS landmarks2, Mat& outImg)

//int DrawLinesInOneImage(IplImage* img1, IplImage* img2, char* OutImage, int* number)
{


	int index1, index2;
	int indexa, indexb;
	
	Point pt1, pt2;
	
	CombineTwoImages_Left_Right( img1, img2, outImg );

	Mat temp = outImg.clone();
	
	

	//draw lines
	for(int i=0;i<landmarks1.ContNumb;i++)
	{
		for(int j=0;j<landmarks1.LandmarksNumb[i];j++)
		{
			pt1.x = landmarks1.Landmarks[i][j].x;
			pt1.y = landmarks1.Landmarks[i][j].y;

			pt2.x = landmarks1.MatchingPnts[i][j].x;
			pt2.y = landmarks1.MatchingPnts[i][j].y;     //numbers of landmarks of two matching shapes are same
			
			pt2.x += img1.cols;

			line( temp, pt1, pt2, (5,5,5), 3, 8, 0 );   //((int)((25*i)%255),(int)((30*i)%255),(int)((255+15*i)%255))
			
		}  
	}

	outImg = temp.clone();
	
	
	return 0;
}

/*
Match two shapes with the shape space method
Input:
	landmarks1: Landmarks from one images, including all contours.
	landmarks2: Landmarks from one images, including all contours.
Output:
	result: The result of matching two shapes.
Return:
	0: OK
*/
int MatchShapes(LANDMARKS& landmarks1,LANDMARKS&  landmarks2, double** result)	//matching 
{
	CShapeSpace ss;
	double tempDist, MinTempDist;
	int tempLandmarksnumber = Landmarks_Numb; 
	CvPoint* L1, *L2, *L3;
	L1 = new CvPoint[tempLandmarksnumber];
	L2 = new CvPoint[tempLandmarksnumber];
	L3 = new CvPoint[tempLandmarksnumber];

	for(int i=0;i<landmarks1.ContNumb;i++)
	{
		landmarks1.MatchingPnts[i] = new cv::Point [tempLandmarksnumber];

		for(int j=0;j<landmarks1.LandmarksNumb[i];j++)
		{
			L1[j].x=landmarks1.Landmarks[i][j].x;
			L1[j].y=landmarks1.Landmarks[i][j].y;			
		}

		MinTempDist = 1000.0;
		for(int h=0;h<landmarks2.ContNumb;h++)
		{			
			for(int k=0;k<landmarks2.LandmarksNumb[h];k++)
			{				
				L2[k].x=landmarks2.Landmarks[h][k].x;
				L2[k].y=landmarks2.Landmarks[h][k].y;
			} 		

			for (int l=0;l<tempLandmarksnumber;l++)       //tempLandmarksnumber==landmarks2.LandmarksNumb[h]
			{
				for(int n=0;n<tempLandmarksnumber;n++)
				{
					L3[(n+l)%tempLandmarksnumber].x = L2[n].x;
					L3[(n+l)%tempLandmarksnumber].y = L2[n].y;							
				}
				ss.GetShapeSpaceDistanceInComplex(L1,L3,tempLandmarksnumber,&tempDist);

				if(tempDist<MinTempDist)
				{
					MinTempDist = tempDist;
					for(int n=0;n<tempLandmarksnumber;n++)
					{
						landmarks1.MatchingPnts[i][n].x = L3[n].x;
						landmarks1.MatchingPnts[i][n].y = L3[n].y;
					}
				}

			}
			
		}	
		(*result)[i] = MinTempDist;

		
	}

	delete [] L1;
	delete [] L2;
	delete [] L3;

	return 0;
}


int obtainInfoFrmOneCont(const  cv::vector<cv::Point> contours, int index, ALL_CONTOUR_DATA& allContData)
{
	
	cv::vector<cv::Point>::const_iterator itp = contours.begin();
	
	int indexP1 = 0;
	while(itp != contours.end())
	{
		//Obtain all contours points
		allContData.cont_data[index].point[indexP1].x = itp->x;
		allContData.cont_data[index].point[indexP1++].y = itp->y;
		
		itp++;
	}

	allContData.cont_data[index].LandmarksNumb = Landmarks_Numb;
	allContData.cont_data[index].indexLandmarks = new int [allContData.cont_data[index].LandmarksNumb];

	int numb = allContData.cont_data[index].PntsNumb;
	
	for(int i=0;i<allContData.cont_data[index].LandmarksNumb;i++)
	{
		allContData.cont_data[index].indexLandmarks[i] = (int) i*numb/allContData.cont_data[index].LandmarksNumb;
	}	
	
	return 0;
}

/*
Test function
Input:
	InputImage1: The first test image
	InputImage2: The second test image
Output:
	Results: The procrustean distance between InputImage1 and InputImage2
	ObjectNumber: shapes' number in InputImage1

Return:
    0: OK

Written by Han Yuexing 20160406
*/
int Test_result( string InputImage1,string InputImage2,  double** Results, int* ObjectNumber )
{
	double proDist;   //procrustean distance
	cv::Mat grayImage1,BinaryImage1,grayImage2,BinaryImage2;
	Mat srcImg1, srcImg2;

	cv::Mat tmpImg1, tmpImg2;
	cv::vector<cv::Vec4i> hierarchy1,hierarchy2;
	cv::vector<cv::vector<cv::Point> > contours1,contours2;

	LANDMARKS landmarks1, landmarks2;

	//a.Close();
	srcImg1 = imread(InputImage1);
	if(srcImg1.empty()) 
	{
		std::cerr<<"Can't Load Image\n";
		return 1;
	}
	cvtColor(srcImg1,grayImage1,CV_BGR2GRAY);
	cv::threshold(grayImage1,BinaryImage1,100,255,CV_THRESH_TOZERO); 

	srcImg2 = imread(InputImage2);
	if(srcImg2.empty()) 
	{
		std::cerr<<"Can't Load Image\n";
		return 1;
	}
	cvtColor(srcImg2,grayImage2,CV_BGR2GRAY);
	cv::threshold(grayImage2,BinaryImage2,100,255,CV_THRESH_TOZERO); 


	tmpImg1 = BinaryImage1.clone();
		
	cv::findContours(tmpImg1, contours1, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);    //CV_CHAIN_APPROX_SIMPLE

	tmpImg2 = BinaryImage2.clone();
		
	cv::findContours(tmpImg2, contours2, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	allContData1.number = contours1.size();
	allContData1.cont_data = new CONTOUR_DATA [allContData1.number];

		
	landmarks1.LandmarksNumb = new int [allContData1.number];
	landmarks1.ContNumb = allContData1.number;
	landmarks1.Landmarks = new cv::Point *[allContData1.number];

	landmarks1.MatchingPnts = new cv::Point *[allContData1.number];

	
	allContData2.number = contours2.size();
	allContData2.cont_data = new CONTOUR_DATA [allContData2.number];

	landmarks2.LandmarksNumb = new int [allContData2.number];
	landmarks2.ContNumb = allContData2.number;
	landmarks2.Landmarks = new cv::Point *[allContData2.number];

	for(size_t k=0;k<contours1.size();k++)
	{

		allContData1.cont_data[k].PntsNumb = contours1[k].size();
		allContData1.cont_data[k].point = new cv::Point[allContData1.cont_data[k].PntsNumb];

		obtainInfoFrmOneCont(contours1[k],k,allContData1);	
		
		landmarks1.LandmarksNumb[k] = Landmarks_Numb;		
		landmarks1.Landmarks[k] = new cv::Point[allContData1.cont_data[k].LandmarksNumb];

		//landmarks1.MatchingPnts[k] = new cv::Point [allContData2.cont_data[k].LandmarksNumb];

		for (int i=0;i<landmarks1.LandmarksNumb[k];i++)
		{
			landmarks1.Landmarks[k][i].x = allContData1.cont_data[k].point[allContData1.cont_data[k].indexLandmarks[i]].x;
			landmarks1.Landmarks[k][i].y = allContData1.cont_data[k].point[allContData1.cont_data[k].indexLandmarks[i]].y;
		}
	}

	

	for(size_t k=0;k<contours2.size();k++)
	{

		allContData2.cont_data[k].PntsNumb = contours2[k].size();
		allContData2.cont_data[k].point = new cv::Point[allContData2.cont_data[k].PntsNumb];

		obtainInfoFrmOneCont(contours2[k],k,allContData2);	
		landmarks2.LandmarksNumb[k] = Landmarks_Numb;		
		landmarks2.Landmarks[k] = new cv::Point[allContData2.cont_data[k].LandmarksNumb];
		

		for (int i=0;i<landmarks2.LandmarksNumb[k];i++)
		{
			landmarks2.Landmarks[k][i].x = allContData2.cont_data[k].point[allContData2.cont_data[k].indexLandmarks[i]].x;
			landmarks2.Landmarks[k][i].y = allContData2.cont_data[k].point[allContData2.cont_data[k].indexLandmarks[i]].y;
		}
	}
	
	//matching 
	*Results = new double[landmarks1.ContNumb];


	MatchShapes(landmarks1, landmarks2, Results);
	*ObjectNumber = landmarks1.ContNumb;

	Mat outImg;
	
	DrawLinesInOneImage(srcImg1, srcImg2, landmarks1, landmarks2, outImg);


	imshow("test",outImg);
	
	waitKey(0);

	for(size_t k=0;k<contours2.size();k++)
	{
		delete [] landmarks2.Landmarks[k];
		delete [] allContData2.cont_data[k].point;
		delete [] allContData2.cont_data[k].indexLandmarks;
	}
	delete [] landmarks2.Landmarks;
	delete [] landmarks2.LandmarksNumb;
	delete [] allContData2.cont_data;
	

	for(size_t k=0;k<contours1.size();k++)
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