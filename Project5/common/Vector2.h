#pragma once

/* ----------“ñŸŒ³î•ñƒNƒ‰ƒX---------- */

template <class T>class Vector2Templete
{
public:
	Vector2Templete();
	Vector2Templete(T x, T y);
	~Vector2Templete();
	T x;
	T y;

	// ‘ã“ü‰‰Zq
	Vector2Templete& operator = (const Vector2Templete<T>& vec);

	// “Y‚¦š‰‰Zq
	T& operator [] (int i);

	// ”äŠr‰‰Zq
	bool operator == (const Vector2Templete& vec)const;
	bool operator != (const Vector2Templete& vec)const;
	bool operator < (const Vector2Templete& vec)const;
	bool operator > (const Vector2Templete& vec)const;
	bool operator <= (const Vector2Templete& vec)const;
	bool operator >= (const Vector2Templete& vec)const;

	// ’P€‰‰Zq
	Vector2Templete& operator += (const Vector2Templete& vec);
	Vector2Templete& operator -= (const Vector2Templete& vec);
	Vector2Templete& operator *= (T k);
	Vector2Templete& operator /= (T k);
	Vector2Templete operator + ()const;
	Vector2Templete operator - ()const;
};

// ƒxƒNƒgƒ‹‚Ì‰‰Z

// Vector2 + T
template <class T>
Vector2Templete<T> operator + (const Vector2Templete<T>& vec, T k);

// Vector2 - T
template <class T>
Vector2Templete<T> operator - (const Vector2Templete<T>& vec, T k);

// Vector2 + Vector2
template <class T>
Vector2Templete<T> operator + (const Vector2Templete<T>& u, const Vector2Templete<T>& v);

// Vector2 - Vector2

template <class T>
Vector2Templete<T> operator - (const Vector2Templete<T>& u, const Vector2Templete<T>& v);

// Vector2 * Vector2
template <class T>
Vector2Templete<T> operator * (const Vector2Templete<T>& u, const Vector2Templete<T>& v);

// Vector2 / Vector2
template <class T>
Vector2Templete<T> operator / (const Vector2Templete<T>& u, const Vector2Templete<T>& v);

// Vector2 % Vector2
template <class T>
Vector2Templete<T> operator % (const Vector2Templete<T>& u, const Vector2Templete<T>& v);

// Vector2 * T
template <class T>
Vector2Templete<T> operator * (const Vector2Templete<T>& vec, T k);

// T * Vector2
template <class T>
Vector2Templete<T> operator * (T k, const Vector2Templete<T>& vec);

// Vector2 / T
template <class T>
Vector2Templete<T> operator / (const Vector2Templete<T>& vec, T k);

// Vector2 % int
template <class T>
Vector2Templete<T> operator % (const Vector2Templete<T>& vec, int k);

using Vector2Int = Vector2Templete<int>;
using Vector2Dbl = Vector2Templete<double>;
using Vector2Flt = Vector2Templete<float>;

#include "./detail/Vector2.h"
