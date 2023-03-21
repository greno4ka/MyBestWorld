#include"BSpline.h"

#include<assert.h>

BSplineCoeffs::BSplineCoeffs(int numPoints,int degree,const double *kn) : numPoints(numPoints), degree(degree) {
	numKnots = numPoints + degree + 1;
	knots = new double[numKnots]; // Check out of memory here!
	for(int i = 0;i<numPoints + degree + 1;i++)
		knots[i] = kn[i];
	ComputeCoefficients();
}

BSplineCoeffs::BSplineCoeffs(BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) : numPoints(numPoints), degree(degree) {
	numKnots = numPoints + degree + 1;
	knots = new double[numKnots];  // Check out of memory here!
	if(type == OpenBasis) {
		for(int i = 0;i<=degree;i++)
			knots[i] = tmin;
		for(int i = degree+1;i<numPoints;i++)
			knots[i] = tmin + (tmax - tmin)*(i - degree)/(numPoints - degree);
		for(int i = numPoints;i < numPoints + degree + 1;i++)
			knots[i] = tmax;
	} else if(type == PeriodicalBasis) {
		for(int i = 0;i<numKnots;i++) 
			knots[i] = tmin + (i - degree)*(tmax - tmin)/(numPoints - degree);
	} else
		assert(false); // Wrong basis type
	ComputeCoefficients();
}

BSplineCoeffs::~BSplineCoeffs()
{
	if(knots != NULL)
		delete[] knots;
	if(coeffs != NULL)
		delete[] coeffs; 
}

void BSplineCoeffs::ComputeCoefficients() {
	coeffs = new double[(numKnots - 2*degree - 1)*(degree + 1)*(degree + 1)]; // Check out of memory here!
	for(int i = 0;i < numKnots - 2*degree - 1;i++) 
		for(int j = 0;j<=degree;j++)
			for(int k = 0;k<=degree;k++)
				Coeffs(i,j,k) = ComputeCoefficient(i + degree, degree, i + j,k);
}

double BSplineCoeffs::ComputeCoefficient(int interval,int n,int i,int k) {
	if(n == 0) // Базисная функция нулевой степени
		return (interval == i)?(1.0):(0);
	else { // n > 0
		double result = 0;
		double d1 = knots[i + n + 1] - knots[i + 1];
		double d0 = knots[i + n] - knots[i];
		if(k == 0) { // Нулевая степень коэффициента
			if(d1 > 0)
				result += ComputeCoefficient(interval,n-1,i+1,0)*knots[i + n + 1]/d1;
			if(d0 > 0)
				result -= ComputeCoefficient(interval,n-1,i,0)*knots[i]/d0;
		} else if(k == n) { // Максимальная степень коэффициента
			if(d0 > 0)
				result += ComputeCoefficient(interval,n-1,i,n-1)/d0;
			if(d1 > 0)
				result -= ComputeCoefficient(interval,n-1,i+1,n-1)/d1;
		} else {
			if(d0 > 0) 
				result += (ComputeCoefficient(interval,n-1,i,k-1) - 
					knots[i]*ComputeCoefficient(interval,n-1,i,k))/d0;
			if(d1 > 0)
				result -= (ComputeCoefficient(interval,n-1,i+1,k-1) - 
					knots[i+n+1]*ComputeCoefficient(interval,n-1,i+1,k))/d1;
		}
		return result;
	}
}

BSplineBasis::~BSplineBasis() { 
	if(b != NULL) 
		delete[] b; 
	if(d != NULL) delete[] d; 
}

void BSplineBasis::SetTesselation(int n) {
	assert(n >= 2);
	if(tesselation != n) {
		tesselation = n;
		b = new double [tesselation * numPoints]; // Check out of memory here
		d = new double [tesselation * numPoints]; // Check out of memory here
		int interval = degree;
		for(int i = 0;i<tesselation;i++) {
			double t = knots[degree] + i*(knots[numPoints] - knots[degree])/(tesselation - 1);
			while(knots[interval + 1] < t)
				interval++;
			for(int j = 0;j<numPoints;j++) {
				b[i*numPoints + j] = BSplineCoeffs::GetBasisValue(interval - degree,j,t);
				d[i*numPoints + j] = BSplineCoeffs::GetDerivativeValue(interval - degree,j,t);
			}
		}
	}
}

