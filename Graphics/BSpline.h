#ifndef __BSPLINE_H

#define __BSPLINE_H

#include"Vector.h"

#include<stdio.h>

enum BSplineBasisType {
	PeriodicalBasis,
	OpenBasis
};

class BSplineCoeffs {
protected:
	// Количество контрольных точек
	int numPoints;
	// Степень сплайна (степень = порядок - 1)
	int degree;
	// Узловой вектор
	double *knots;
	// Длина узлового вектора = numPoints + degree + 1
	int numKnots;
	// Коэффициенты базисных функций
	// Обращение: coeffs[номер интервала - degree,
	// номер базисной функции - номер интервала - degree, степень коэффициента]
	double *coeffs;
public:
	BSplineCoeffs(int numPoints,int degree,const double *kn);
	BSplineCoeffs(BSplineBasisType type,int numPoints,int degree,double tmin,double tmax);
	~BSplineCoeffs();
public:
	int GetOrder() const
		{ return degree + 1; }
	int GetDegree() const
		{ return degree; }
	int GetNumPoints() const
		{ return numPoints; }	
	const double *GetKnots() const
		{ return knots; } 
	double GetParameterMin() const
	    { return knots[degree]; }
	double GetParameterMax() const
		{ return knots[numPoints]; }
	// Возвращает заранее вычисленный коэффициент базисной функции.
	// interval - Номер интервала - degree.
	// i - Номер базисной функции.
	// k - Степень при коэффициенте базисной функции.
	// Возвращаеи значение соответствующего коэффициента.
	double GetCoeff(int interval,int i,int k) const;
	double GetBasisValue(int interval,int i,double t) const;
	double GetDerivativeValue(int interval,int i,double t) const;
protected:
	inline double &Coeffs(int i,int j,int k) // Имитируем трехмерный массив
		{ return coeffs[(i*(degree + 1) + j)*(degree + 1) + k]; }
	inline double Coeffs(int i,int j,int k) const // Имитируем трехмерный массив
		{ return coeffs[(i*(degree + 1) + j)*(degree + 1) + k]; }
	void ComputeCoefficients();
	double ComputeCoefficient(int interval,int n,int i,int k);
};

inline double BSplineCoeffs::GetCoeff(int interval,int i,int k) const {
	if(i < interval || i > interval + degree)
		return 0;
	return Coeffs(interval,i - interval,k);
}

inline double BSplineCoeffs::GetBasisValue(int interval,int i,double t) const {
	if(i < interval || i > interval + degree)
		return 0;
	double result = Coeffs(interval,i - interval,degree);
	for(int k = degree-1;k>=0;k--) 
		result = t*result + Coeffs(interval,i - interval,k);
	return result;
}

inline double BSplineCoeffs::GetDerivativeValue(int interval,int i,double t) const {
	if(i < interval || i > interval + degree)
		return 0;
	double result = degree*Coeffs(interval,i - interval,degree);
	for(int k = degree-1;k>=1;k--)
		result = t*result + k*Coeffs(interval,i - interval,k);
	return result;
}

const int DefaultTesselation = 101;

class BSplineBasis : public BSplineCoeffs {
protected:
	// Количество точек разбиения параметрической кривой
	int tesselation; 
	// Значение базисных функций в точках разбиения. 
	double *b;
	// Значения базисный функций в точках разбиения для вычисления производной.
	double *d;
public:
	BSplineBasis(int tesselation,BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) : 
	  BSplineCoeffs(type,numPoints,degree,tmin,tmax) 
	    { SetTesselation(tesselation); }
	BSplineBasis(int tesselation,int numPoints,int degree,const double *knots) : 
	  BSplineCoeffs(numPoints,degree,knots) 
	    { SetTesselation(tesselation); }
	BSplineBasis(BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) :
	  BSplineCoeffs(type,numPoints,degree,tmin,tmax) 
	    { SetTesselation(DefaultTesselation); }
	BSplineBasis(int numPoints,int degree,const double *knots ) :
	  BSplineCoeffs(numPoints,degree,knots) 
		{ SetTesselation(DefaultTesselation); }
	~BSplineBasis();
	double GetBasisValue(int func,int i) const
		{ return b[i*numPoints + func]; }
	double GetDerivativeValue(int func,int i) const
		{ return d[i*numPoints + func]; }
	int GetTesselation() const
		{ return tesselation; }
	void SetTesselation(int n);
};

template<class Point> class BSpline : public BSplineBasis {
protected:
	Point *points;
public:
	BSpline(int tesselation,BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) : 
	  BSplineBasis(tesselation,type,numPoints,degree,tmin,tmax) 
		{ points = new Point[numPoints]; /* Check out of memory here! */  }
	BSpline(int tesselation,int numPoints,int degree,const double *knots) : 
	  BSplineBasis(tesselation,numPoints,degree,knots) 
	    { points = new Point[numPoints]; /* Check out of memory here! */ }
	BSpline(BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) :
	  BSplineBasis(type,numPoints,degree,tmin,tmax) 
	    { points = new Point[numPoints]; /* Check out of memory here! */ }
	BSpline(int numPoints,int degree,const double *knots ) :
	  BSplineBasis(numPoints,degree,knots) 
		{ points = new Point[numPoints]; /* Check out of memory here! */ }
	~BSpline()
		{ if(points != NULL) delete[] points; }
	Point &ControlPoint(int i)
		{ return points[i]; }
	Point ControlPoint(int i) const
		{ return points[i]; }
	Point GetPoint(int i) const;
	Point GetTangent(int i) const;
};

template<class Point> Point BSpline<Point>::GetPoint(int i) const
{
	Point result = points[0] * GetBasisValue(0,i);
	for(int j = 1;j<numPoints;j++)
		result += points[j] * GetBasisValue(j,i);
	return result;
}

template<class Point> Point BSpline<Point>::GetTangent(int i) const
{
	Point result = points[0] * GetDerivativeValue(0,i);
	for(int j = 1;j<numPoints;j++)
		result += points[j] * GetDerivativeValue(j,i);
	return result;
}

template<class Spline> inline double2 GetNormal(const Spline &bsp,int i)
{
	double2 t = bsp.GetTangent(i);
	t.Normalize();
	return double2(t.y,-t.x);
}

template<class Spline> inline double2 GetPerspectiveNormal(const Spline &bsp,int i)
{
	double2 t = bsp.GetTangent(i).Perspective();
	t.Normalize();
	return double2(t.y,-t.x);
}


template<class Point> class BSplineSurface {
protected:
	const BSplineBasis *uBasis;
	const BSplineBasis *vBasis;
	Point *points;
public:
	BSplineSurface(const BSplineBasis *uBasis,const BSplineBasis *vBasis) : uBasis(uBasis),vBasis(vBasis)
		{ points = new Point[uBasis->GetNumPoints()*vBasis->GetNumPoints()]; }
	~BSplineSurface()
		{ if(points) delete[] points; }
	const BSplineBasis *GetUBasis() const
		{ return uBasis; }
	const BSplineBasis *GetVBasis() const
		{ return vBasis; }
	Point ControlPoint(int i,int j) const
		{ return points[i*vBasis->GetNumPoints() + j]; }
	Point &ControlPoint(int i,int j)
		{ return points[i*vBasis->GetNumPoints() + j]; }
	Point GetPoint(int i,int j) const;
	Point GetNormal(int i,int j) const;
};

template<class Point> Point BSplineSurface<Point>::GetPoint(int i,int j) const
{
	Point result(0,0,0);
	for(int k = 0;k<uBasis->GetNumPoints();k++)
		for(int l = 0;l<vBasis->GetNumPoints();l++) 
			result += ControlPoint(k,l)*
			          uBasis->GetBasisValue(k,i)*
					  vBasis->GetBasisValue(l,j);
	return result;
}

template<class Point> Point BSplineSurface<Point>::GetNormal(int i,int j) const
{
	Point ut(0,0,0);
	Point vt(0,0,0);
	for(int k = 0;k<uBasis->GetNumPoints();k++)
		for(int l = 0;l<vBasis->GetNumPoints();l++) {
			ut += ControlPoint(k,l)*uBasis->GetDerivativeValue(k,i)*vBasis->GetBasisValue(l,j);
			vt += ControlPoint(k,l)*uBasis->GetBasisValue(k,i)*vBasis->GetDerivativeValue(l,j);
		}
	ut.Normalize();
	vt.Normalize();
	return ut * vt;
}



#endif // __BSPLINE_H