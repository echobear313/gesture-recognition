#pragma once
#include "ShapeSpaceLib_Dist.h"


CShapeSpace::CShapeSpace()
{
	m_dDegree = 0;  // the violence of change

	m_EucP1.xy = NULL;
	m_EucP2.xy = NULL;
	m_EucP.xy = NULL;
	m_PreSsP1.xy = NULL;
	m_PreSsP2.xy = NULL;
	m_PreSsP.xy = NULL;

	m_iNum1 = 0;
	m_iNum2 = 0;
}

CShapeSpace::~CShapeSpace()
{
	if (m_EucP1.xy != NULL)
	{
		delete[] m_EucP1.xy;
		m_EucP1.xy = NULL;
	}

	if (m_EucP2.xy != NULL)
	{
		delete[] m_EucP2.xy;
		m_EucP2.xy = NULL;
	}
	if (m_EucP.xy != NULL)
	{
		delete[] m_EucP.xy;
		m_EucP.xy = NULL;
	}
	if (m_PreSsP1.xy != NULL)
	{
		delete[] m_PreSsP1.xy;
		m_PreSsP1.xy = NULL;
	}
	if (m_PreSsP2.xy != NULL)
	{
		delete[] m_PreSsP2.xy;
		m_PreSsP2.xy = NULL;
	}

	if (m_PreSsP.xy != NULL)
	{
		delete[] m_PreSsP.xy;
		m_PreSsP.xy = NULL;
	}
}


//get the norm of the vector
int CShapeSpace::GetNorm(const point* InPoint, int Numb, double* Norm)
{
	double tNorm = 0;
	int i = 0;
	while (1)
	{
		if (i >= 2 * Numb)
			break;

		tNorm += pow(InPoint->xy[i], 2);
		i++;
	}

	*Norm = sqrt(tNorm);


	return 0;
}


//int CShapeSpace::GetShapeSpaceVector(point* EucP,int Numb,point* SsP) in ShapeSpaceLib2008
int CShapeSpace::GetPreShapeSpaceVector(point* EucP, int Numb, point* PreSsP)
{
	if (PreSsP->xy == NULL)
		return 1;        //20100104

	double xMean, yMean; //the means of coordiate x and y
	xMean = 0;
	yMean = 0;
	int i = 0;
	int total = Numb;
	double Norm = 0;

	if (EucP == NULL)
		return 1;
	while (1)
	{
		xMean += EucP->xy[i];
		i++;
		yMean += EucP->xy[i];
		i++;
		if (i >= 2 * Numb)
		{
			xMean /= total;
			yMean /= total;
			break;
		}
	}

	//get the vector with diveding the centre
	for (i = 0; i<total; i++)
	{
		PreSsP->xy[i * 2] = EucP->xy[i * 2] - xMean;
		PreSsP->xy[i * 2 + 1] = EucP->xy[i * 2 + 1] - yMean;
	}

	//get the vector with removing scale
	GetNorm(PreSsP, Numb, &Norm);

	for (i = 0; i<2 * total; i++)
	{
		PreSsP->xy[i] /= Norm;
	}

	return 0;
}


int CShapeSpace::GetSameDimVect(point EucP1, point EucP2, int Num1, int Num2, point* EucP)
{
	int tNum1, tNum2, tNum3, tNum4;

	if (Num1<Num2)
	{
		//		EucP->xy = (double*)malloc(sizeof(double)*2*Num2);

		tNum1 = Num2 / Num1;
		tNum2 = Num2%Num1;
		tNum4 = tNum2;

		for (int i = 0; i<Num1; i++)
		{
			int j = 1;
			tNum3 = tNum1;
			EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2)] = EucP1.xy[2 * i];
			EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 1] = EucP1.xy[2 * i + 1];
			tNum3--;
			while (tNum3 >= 0)
			{
				if (tNum2 <= 0)
				{
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j] = EucP1.xy[2 * i] + j* ((EucP1.xy[2 * ((i + 1) % Num1)] - EucP1.xy[2 * i]) / tNum1);
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j + 1] = EucP1.xy[2 * i + 1] + j* ((EucP1.xy[2 * ((i + 1) % Num1) + 1] - EucP1.xy[2 * i + 1]) / tNum1);
					j++;
					tNum3--;
					if (tNum3 == 0)
						break;
				}
				else if (tNum2>0)
				{
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j] = EucP1.xy[2 * i] + j* ((EucP1.xy[2 * ((i + 1) % Num1)] - EucP1.xy[2 * i]) / (tNum1 + 1));
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j + 1] = EucP1.xy[2 * i + 1] + j* ((EucP1.xy[2 * ((i + 1) % Num1) + 1] - EucP1.xy[2 * i + 1]) / (tNum1 + 1));
					j++;
					tNum3--;
				}
			}

			if (tNum2>0)
				tNum2--;

		}
	}
	else if (Num1>Num2)
	{
		//		EucP->xy = (double*)malloc(sizeof(double)*2*Num1);

		tNum1 = Num1 / Num2;
		tNum2 = Num1%Num2;
		tNum4 = tNum2;

		for (int i = 0; i<Num2; i++)
		{
			int j = 1;
			tNum3 = tNum1;
			EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2)] = EucP2.xy[2 * i];
			EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 1] = EucP2.xy[2 * i + 1];
			tNum3--;
			while (tNum3 >= 0)
			{
				if (tNum2 <= 0)
				{
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j] = EucP2.xy[2 * i] + j* ((EucP2.xy[2 * ((i + 1) % Num2)] - EucP2.xy[2 * i]) / tNum1);
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j + 1] = EucP2.xy[2 * i + 1] + j* ((EucP2.xy[2 * ((i + 1) % Num2) + 1] - EucP2.xy[2 * i + 1]) / tNum1);
					j++;
					tNum3--;
					if (tNum3 == 0)
						break;
				}
				else if (tNum2>0)
				{
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j] = EucP2.xy[2 * i] + j* ((EucP2.xy[2 * ((i + 1) % Num2)] - EucP2.xy[2 * i]) / (tNum1 + 1));
					EucP->xy[tNum1 * 2 * i + 2 * (tNum4 - tNum2) + 2 * j + 1] = EucP2.xy[2 * i + 1] + j* ((EucP2.xy[2 * ((i + 1) % Num2) + 1] - EucP2.xy[2 * i + 1]) / (tNum1 + 1));
					j++;
					tNum3--;
				}
			}

			if (tNum2>0)
				tNum2--;

		}
	}
	return 0;

}


int CShapeSpace::GetHermiInnerProd(const point* InPoint1, const point* InPoint2, const int Numb, point* OutPoint)
{
	OutPoint->xy[0] = 0;
	OutPoint->xy[1] = 0;
	for (int i = 0; i<Numb; i++)
	{
		OutPoint->xy[0] += (InPoint1->xy[2 * i])*(InPoint2->xy[2 * i]) + (InPoint1->xy[2 * i + 1])*(InPoint2->xy[2 * i + 1]);
		OutPoint->xy[1] += (InPoint1->xy[2 * i + 1])*(InPoint2->xy[2 * i]) - (InPoint1->xy[2 * i])*(InPoint2->xy[2 * i + 1]);
	}

	return 0;
}


//InPoint1 and InPoint2 are vectors on Pre-Shape Space 
int CShapeSpace::GetShapeSpaceDistanceInComplex(const point* InSsPoint1, const point* InSsPoint2, const int Numb, double* Dist)
{
	point tPoint;
	tPoint.xy = new double[2 * 1];
	GetHermiInnerProd(InSsPoint1, InSsPoint2, Numb, &tPoint);

	if (tPoint.xy[0]>1.0)
		tPoint.xy[0] = 1.0;
	else if (tPoint.xy[0]<-1.0)
		tPoint.xy[0] = -1.0;
	if (tPoint.xy[1]>1.0)
		tPoint.xy[1] = 1.0;
	else if (tPoint.xy[1]<-1.0)
		tPoint.xy[1] = -1.0;

	GetNorm(&tPoint, 1, Dist);
	*Dist = acos(*Dist);
	delete[] tPoint.xy;
	tPoint.xy = NULL;
	return 0;
}

//be not used
int CShapeSpace::GetShapeSpaceDistanceInComplex(double* Dist)
{
	GetSameDimVect(m_EucP1, m_EucP2, m_iNum1, m_iNum2, &m_EucP);

	GetPreShapeSpaceVector(&m_EucP1, m_iNum1, &m_PreSsP1);
	GetPreShapeSpaceVector(&m_EucP2, m_iNum2, &m_PreSsP2);
	if (m_iNum1>m_iNum2)
		GetPreShapeSpaceVector(&m_EucP, m_iNum1, &m_PreSsP);
	else if (m_iNum1<m_iNum2)
		GetPreShapeSpaceVector(&m_EucP, m_iNum2, &m_PreSsP);

	if (m_iNum1>m_iNum2)
	{
		GetShapeSpaceDistanceInComplex(&m_PreSsP1, &m_PreSsP, m_iNum1, Dist);
	}
	else if (m_iNum1<m_iNum2)
	{
		GetShapeSpaceDistanceInComplex(&m_PreSsP, &m_PreSsP2, m_iNum2, Dist);
	}
	else if (m_iNum1 == m_iNum2)
	{
		GetShapeSpaceDistanceInComplex(&m_PreSsP1, &m_PreSsP2, m_iNum1, Dist);
	}

	return 0;
}

//2DPoint1 and 2DPoint2 are two points in Euclidean space or image pilex points
int CShapeSpace::GetShapeSpaceDistanceInComplex(const CvPoint* twoDPoint1, const CvPoint* twoDPoint2, const int Numb, double* Dist)
{
	point tPoint1;
	tPoint1.xy = new double[2 * Numb];
	point tPoint2;
	tPoint2.xy = new double[2 * Numb];

	if (m_PreSsP1.xy != NULL)
	{
		delete[] m_PreSsP1.xy;
		m_PreSsP1.xy = NULL;
	}
	m_PreSsP1.xy = new double[2 * Numb];

	if (m_PreSsP2.xy != NULL)
	{
		delete[] m_PreSsP2.xy;
		m_PreSsP2.xy = NULL;
	}
	m_PreSsP2.xy = new double[2 * Numb];


	for (int i = 0; i<Numb; i++)
	{
		tPoint1.xy[2 * i] = (double)twoDPoint1[i].x;
		tPoint1.xy[2 * i + 1] = (double)twoDPoint1[i].y;
		tPoint2.xy[2 * i] = (double)twoDPoint2[i].x;
		tPoint2.xy[2 * i + 1] = (double)twoDPoint2[i].y;
	}

	GetPreShapeSpaceVector(&tPoint1, Numb, &m_PreSsP1);
	GetPreShapeSpaceVector(&tPoint2, Numb, &m_PreSsP2);
	GetShapeSpaceDistanceInComplex(&m_PreSsP1, &m_PreSsP2, Numb, Dist);

	if (!tPoint1.xy)
	{
		delete[] tPoint1.xy;
		tPoint1.xy = NULL;
	}
	if (!tPoint2.xy)
	{
		delete[] tPoint2.xy;
		tPoint2.xy = NULL;
	}

	return 0;
}

