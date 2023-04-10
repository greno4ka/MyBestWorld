#ifndef __VECTOR_H

#define __VECTOR_H

#include<math.h>

// Вектор произвольной размерности

template<class C,int N> class Vector {
protected:
	C data[N];
public:
	Vector() 
		{ }
	Vector(C c) 
		{ for(int i = 0;i<N;i++) data[i] = c; }
	Vector(const C *data2) 
		{ for(int i = 0;i<N;i++) data[i] = data2[i]; }
	Vector(const Vector<C,N> &another) 
		{ for(int i = 0;i<N;i++) data[i] = another.data[i]; }
	C &operator[](int index)
		{ return data[index]; }	
	C operator[](int index) const
		{ return data[index]; }	
	Vector<C,N> &operator += (const Vector<C,N> &v2);
	Vector<C,N> &operator -= (const Vector<C,N> &v2);
	Vector<C,N> &operator *= (C c);
	C GetLength() const;
	void Normalize();
};

template<class C,int N> inline C Vector<C,N>::GetLength() const
{
	C result = 0;
	for(int i = 0;i<N;i++)
		result += data[i]*data[i];
	return (C)sqrt(result);
}

template<class C,int N> inline void Vector<C,N>::Normalize() 
{
	C length = GetLength();
	if(length > 1e-6) 
		for(int i = 0;i<N;i++)
			data[i] /= length;
}


template<class C,int N> inline Vector<C,N> &Vector<C,N>::operator*=(C c)
{
	for(int i = 0;i<N;i++)
		data[i] *= c;
	return *this;
}

template<class C,int N> inline Vector<C,N> &Vector<C,N>::operator+=(const Vector<C,N> &another)
{
	for(int i = 0;i<N;i++)
		data[i] += another.data[i];
	return *this;
}

template<class C,int N> inline Vector<C,N> &Vector<C,N>::operator-=(const Vector<C,N> &another)
{
	for(int i = 0;i<N;i++)
		data[i] -= another.data[i];
	return *this;
}

// Вектор размерности 2

template<class C> class Vector<C,2> {
public:
#pragma pack(1)	// Побайтовое выравнивание
	union {
	    C data[2];
		struct {
			C x;
			C y;
		};
	};
#pragma pack() 
	Vector() 
		{ }
	Vector(C c) 
		{ x = y = c; }
	Vector(C x, C y) : x(x),y(y)
		{ }
	Vector(const C *data2) 
		{ x = data2[0]; y = data2[1]; }
	Vector(const Vector<C,3> &another) 
		{ x = another.x; y = another.y; }
	C &operator[](int index)
		{ return data[index]; }	
	C operator[](int index) const
		{ return data[index]; }	
	Vector<C,2> &operator += (const Vector<C,2> &v2)
	    { x += v2.x; y += v2.y; return *this; } 
	Vector<C,2> &operator -= (const Vector<C,2> &v2)
	    { x -= v2.x; y -= v2.y; return *this; }
	Vector<C,2> &operator *= (C c)
		{ x *= c; y *= c; return *this; }
	C GetLength() const 
	    { return sqrt(x*x + y*y); }
	void Perspective(const Vector<C,3> &v)
		{ x = v.x/v.z; y = v.y/v.z; }
	void Normalize();
};

template<class C> inline void Vector<C,2>::Normalize()
{
	C length = GetLength();
	x /= length;
	y /= length;
}

// Вектор размерности 3

template<class C> class Vector<C,3> {
public:
#pragma pack(1)	// Побайтовое выравнивание
	union {
	    C data[3];
		struct {
			C x;
			C y;
			C z;
		};
	};
#pragma pack() 
	Vector() 
		{ }
	Vector(C c) 
		{ x = y = z = c; }
	Vector(C x, C y, C z) : x(x),y(y),z(z)
		{ }
	Vector(const C *data2) 
		{ x = data2[0]; y = data2[1]; z = data2[2]; }
	Vector(const Vector<C,3> &another) 
		{ x = another.x; y = another.y; z = another.z; }
	C &operator[](int index)
		{ return data[index]; }	
	C operator[](int index) const
		{ return data[index]; }	
	Vector<C,3> &operator += (const Vector<C,3> &v2)
	    { x += v2.x; y += v2.y; z += v2.z; return *this; } 
	Vector<C,3> &operator -= (const Vector<C,3> &v2)
	    { x -= v2.x; y -= v2.y; z -= v2.z; return *this; }
	Vector<C,3> &operator *= (C c)
		{ x *= c; y *= c; z *= c; return *this; }
	C GetLength() const 
	    { return sqrt(x*x + y*y + z*z); }
	void Perspective(const Vector<C,4> &v)
		{ x = v.x/v.w; y = v.y/v.w; z = v.z/v.w; }
	Vector<C,2> Perspective() const
		{ return Vector<C,2>(x/z,y/z); }
	void Normalize();
};

template<class C> inline void Vector<C,3>::Normalize()
{
	C length = GetLength();
	x /= length;
	y /= length;
	z /= length;
}

template<class C> inline Vector<C,3> operator * (const Vector<C,3> &a, const Vector<C,3> &b)
{
	return double3(a.y*b.z - a.z*b.y,
		           b.x*a.z - b.z*a.x,
				   a.y*b.x - a.x*b.y);
}

// Вектор размерности 4

template<class C> class Vector<C,4> {
public:
#pragma pack(1)	// Побайтовое выравнивание
	union {
	    C data[4];
		struct {
			C x;
			C y;
			C z;
			C w;
		};
	};
#pragma pack() 
	Vector() 
		{ }
	Vector(C c) 
		{ x = y = z = c; }
	Vector(const C *data2) 
		{ x = data2[0]; y = data2[1]; z = data2[2]; w = data2[3]; }
	Vector(const Vector<C,4> &another) 
		{ x = another.x; y = another.y; z = another.z; w = another.w; }
	C &operator[](int index)
		{ return data[index]; }	
	C operator[](int index) const
		{ return data[index]; }	
	Vector<C,4> &operator += (const Vector<C,4> &v2)
	    { x += v2.x; y += v2.y; z += v2.z; w += v2.w; return *this; } 
	Vector<C,4> &operator -= (const Vector<C,4> &v2)
	    { x -= v2.x; y -= v2.y; z -= v2.z; w -= v2.z; return *this; }
	Vector<C,4> &operator *= (C c)
		{ x *= c; y *= c; z *= c; w *= c; return *this; }
	C GetLength() const 
	    { return sqrt(x*x + y*y + z*z + w*w); }
	Vector<C,3> Perspective() const
		{ return Vector<C,3>(x/w, y/w, z/w); }
	void Normalize();
};

template<class C> inline void Vector<C,4>::Normalize()
{
	C length = GetLength();
	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

// Операции над векторами произвольной размерности

template<class C,int N> inline Vector<C,N> operator*(const Vector<C,N> &a,C b)
{
	Vector<C,N> result;
	for(int i = 0;i<N;i++)
		result[i] = a[i]*b;
	return result;
}

template<class C,int N> inline Vector<C,N> operator/(const Vector<C,N> &a,C b)
{
	Vector<C,N> result;
	for(int i = 0;i<N;i++)
		result[i] = a[i]/b;
	return result;
}

template<class C,int N> inline Vector<C,N> operator+(const Vector<C,N> &a,const Vector<C,N> &another)
{
	Vector<C,N> result;
	for(int i = 0;i<N;i++)
		result[i] = a[i] + another[i];
	return result;
}

template<class C,int N> inline Vector<C,N> operator-(const Vector<C,N> &a,const Vector<C,N> &another)
{
	Vector<C,N> result;
	for(int i = 0;i<N;i++)
		result[i] = a[i] - another[i];
	return result;
}

template<class C,int N> inline C operator^(const Vector<C,N> &a,const Vector<C,N> &another)
{
	C result = 0;
	for(int i = 0;i<N;i++)
		result += a[i]*another[i];
	return result;
}

template<class C,int N> inline Vector<C,N> operator - (Vector<C,N> a)
{
	for(int i = 0;i<N;i++)
		a[i] = -a[i];
	return a;
}


template<class C,int N> inline Vector<C,N> Zero(Vector<C,N> &v) {
	for(int i = 0;i<N;i++)
		v[i] = 0;
	return v;
}

template<class C,int N,int M> class Matrix {
protected:
	C data[N][M];
public:
	Matrix();
	Matrix(const C data2[N][M]);
	Matrix(const Matrix<C,N,M> &another);
	C *operator[](int i)
		{ return data[i]; }
	const C *operator[](int i) const
		{ return data[i]; }
	void One();
	void Zero();
};

template<class C,int N,int M> inline Matrix<C,N,M>::Matrix() 
{
	for(int i = 0;i<N;i++)
		for(int j = 0;j<M;j++)
			data[i][j] = 0;
}

template<class C,int N,int M> inline Matrix<C,N,M>::Matrix(const C data2[N][M])
{
	for(int i = 0;i<N;i++)
		for(int j = 0;j<M;j++)
			data[i][j] = data2[i][j];
}

template<class C,int N,int M> inline Matrix<C,N,M>::Matrix(const Matrix<C,N,M> &another)
{
	for(int i = 0;i<N;i++)
		for(int j = 0;j<M;j++)
			data[i][j] = another.data[i][j];
}

template<class C,int N,int M> inline void Matrix<C,N,M>::Zero() 
{
	for(int i = 0;i<N;i++)
		for(int j = 0;j<M;j++)
			data[i][j] = 0;
}

template<class C,int N,int M> inline void Matrix<C,N,M>::One() 
{
	for(int i = 0;i<N;i++)
		for(int j = 0;j<M;j++)
			data[i][j] = 0;
	int m = (N<M)?(N):(M);
    for(int i = 0;i<m;i++)
		data[i][i] = 1;
}

template<class C,int N,int M> inline void Mul(Vector<C,N> &result,
											  const Matrix<C,N,M> &a,
											  const Vector<C,M> &b)
{
	for(int i = 0;i<N;i++) {
		C sum = 0;
		for(int j = 0;j<M;j++)
			sum += a[i][j]*b[j];
		result[i] = sum;
	}
}

template<class C,int N,int M,int L> inline void Mul(Matrix<C,N,L> &result,
											        const Matrix<C,N,M> &a,
													const Matrix<C,M,L> &b)
{
	for(int i = 0;i<N;i++) 
		for(int j = 0;j<L;j++) {
			C sum = 0;
			for(int k = 0;k<M;k++)
				sum += a[i][k]*b[k][j];
			result[i][j] = sum;
		}
}

template<class C,int N1,int M1,int N2,int M2> 
    inline void SubMatrix(Matrix<C,N1,M1> &dst,
						  const Matrix<C,N2,M2> &src,
						  int i0,
						  int j0) 
{
	for(int i = 0;i<N1;i++)
		for(int j = 0;j<M1;j++)
			dst[i][j] = src[i0 + i][j0 +  j];
}

template<class C> inline void Translate(Matrix<C,4,4> &m,C dx,C dy,C dz) 
{
	m.One();
	m[0][3] = dx;
	m[1][3] = dy;
	m[2][3] = dz;
}

template<class C> inline void RotateX(Matrix<C,4,4> &m,C angle) 
{
	m.One();
	C c = (C)cos(angle);
	C s = (C)sin(angle);
	m[1][1] = c;
	m[1][2] = -s;
	m[2][1] = s;
	m[2][2] = c;
}

template<class C> inline void RotateY(Matrix<C,4,4> &m,C angle) 
{
	m.One();
	C c = (C)cos(angle);
	C s = (C)sin(angle);
	m[0][0] = c;
	m[0][2] = -s;
	m[2][0] = s;
	m[2][2] = c;
}

// Определения основных типов

typedef Vector<float,4> float4;
typedef Vector<float,3> float3;
typedef Vector<float,2> float2;

typedef Vector<double,4> double4;
typedef Vector<double,3> double3;
typedef Vector<double,2> double2;

typedef Vector<int,2> int2;

typedef Matrix<float,4,4> float4x4;
typedef Matrix<float,3,3> float3x3;

typedef Matrix<double,4,4> double4x4;
typedef Matrix<double,3,3> double3x3;


#endif // __VECTOR_H
