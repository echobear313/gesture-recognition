#pragma once
/*
This version is made in 20160329
name: ShapeSpaceLib_Dist
Only shape space distance is left in the version.



This pre-version is ShapeSpaceLib2014; and previous version is ShapeSpaceLib2008

The code is wrotten by Han Yuexing 20160329

*/

#include <stdio.h> 
#include <math.h>


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

typedef struct point
{
	double* xy;  //the set of coordiate x and y
}point;

class CShapeSpace
{
public:
	CShapeSpace();
	~CShapeSpace();
public:
	int GetNorm(const point* InPoint, int Numb, double* Norm); //get the norm of the vector
	int GetPreShapeSpaceVector(point* EucP, int Numb, point* SsP);            //int GetShapeSpaceVector(point* EucP,int Numb,point* SsP);	
	int GetSameDimVect(point EucP1, point EucP2, int Num1, int Num2, point* EucP);




public:  //using complex number theory on Shape Space
	int GetHermiInnerProd(const point* InPoint1, const point* InPoint2, const int Numb, point* OutPoint);
	int GetShapeSpaceDistanceInComplex(const point* InPoint1, const point* InPoint2, const int Numb, double* Dist);
	int GetShapeSpaceDistanceInComplex(double* Dist);  //the finally function
	int GetShapeSpaceDistanceInComplex(const CvPoint* InSsPoint1, const CvPoint* InSsPoint2, const int Numb, double* Dist);

public:
	double m_dDegree; // the violence of change
					  // original contours' points	
	point m_EucP1, m_EucP2;
	point m_EucP;

	//contours' points on pre Shape Space						////contours' points on Shape Space in 2008
	point m_PreSsP1;											//point m_SsP1;     in 2008
	point m_PreSsP2;											//point m_SsP2;     in 2008
	point m_PreSsP;												//point m_SsP;      in 2008


																//the number of contours points
	int m_iNum1, m_iNum2;

};
