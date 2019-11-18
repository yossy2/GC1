#include "..\Vector2.h"

template <class T>
Vector2Templete<T>::Vector2Templete()
{
	x = static_cast<T>(0);
	y = static_cast<T>(0);
}

template <class T>
Vector2Templete<T>::Vector2Templete(T x, T y)
{
	Vector2Templete<T>::x = x;
	Vector2Templete<T>::y = y;
}

template <class T>
Vector2Templete<T>::~Vector2Templete()
{

}

// ë„ì¸ââéZéq
template <class T>
Vector2Templete<T>& Vector2Templete<T>::operator=(const Vector2Templete<T>& vec)
{
	x = vec.x;
	y = vec.y;

	return *this;
}

// ìYÇ¶éöââéZéq
template <class T>
T& Vector2Templete<T>::operator[](int i)
{
	if (i == 1)
	{
		return this->x;
	}
	else if (i == 0)
	{
		return this->y;
	}
	else
	{
		return this->x;
	}
}

// î‰ärââéZéq
template <class T>
bool Vector2Templete<T>::operator==(const Vector2Templete<T> & vec) const
{
	return ((this->x == vec.x) && (this->y == vec.y));
}

template <class T>

bool Vector2Templete<T>::operator!=(const Vector2Templete<T> & vec) const
{
	return ((this->x != vec.x) || (this->y != vec.y));
}

template <class T>
bool Vector2Templete<T>::operator<(const Vector2Templete<T> & vec) const
{
	// ÉxÉNÉgÉãÇÃëÂÇ´Ç≥Ç≈î‰ärÇ∑ÇÈèÍçá
	//return ((this->x * this->x + this->y * this->y) < (vec.x * vec.x + vec.y * vec.y));

	// x,yÇªÇÍÇºÇÍÇÃëÂÇ´Ç≥Ç≈î‰Ç◊ÇÈèÍçá
	return ((this->x < vec.x) && (this->y < vec.y));
}

template <class T>
bool Vector2Templete<T>::operator>(const Vector2Templete<T> & vec) const
{
	//return ((this->x * this->x + this->y * this->y) > (vec.x * vec.x + vec.y * vec.y));
	return ((this->x > vec.x) && (this->y > vec.y));
}

template <class T>
bool Vector2Templete<T>::operator<=(const Vector2Templete<T> & vec) const
{
	//return ((this->x * this->x + this->y * this->y) <= (vec.x * vec.x + vec.y * vec.y));
	return ((this->x <= vec.x) && (this->y <= vec.y));
}

template <class T>
bool Vector2Templete<T>::operator>=(const Vector2Templete<T> & vec) const
{
	//return ((this->x * this->x + this->y * this->y) >= (vec.x * vec.x + vec.y * vec.y));
	return ((this->x >= vec.x) && (this->y >= vec.y));
}

// íPçÄââéZéq
template <class T>
Vector2Templete<T> & Vector2Templete<T>::operator+=(const Vector2Templete<T> & vec)
{
	x += vec.x;
	y += vec.y;

	return *this;
}

template <class T>
Vector2Templete<T> & Vector2Templete<T>::operator-=(const Vector2Templete<T> & vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}

template <class T>
Vector2Templete<T> & Vector2Templete<T>::operator*=(T k)
{
	x *= k;
	y *= k;

	return *this;
}

template <class T>
Vector2Templete<T> & Vector2Templete<T>::operator/=(T k)
{
	x /= k;
	y /= k;

	return *this;
}

template <class T>
Vector2Templete<T> Vector2Templete<T>::operator+()const
{
	return *this;
}

template <class T>
Vector2Templete<T> Vector2Templete<T>::operator-()const
{
	return Vector2Templete<T>(-this->x,-this->y);
}

// ÉxÉNÉgÉãÇÃââéZ
template <class T>
Vector2Templete<T> operator+(const Vector2Templete<T> & vec, T k)
{
	Vector2Templete<T> tmp;

	tmp.x = vec.x + k;
	tmp.y = vec.y + k;

	return tmp;
}

template <class T>
Vector2Templete<T> operator-(const Vector2Templete<T> & vec, T k)
{
	Vector2Templete<T> tmp;

	tmp.x = vec.x - k;
	tmp.y = vec.y - k;

	return tmp;
}

template <class T>
Vector2Templete<T> operator+(const Vector2Templete<T> & u, const Vector2Templete<T> & v)
{
	Vector2Templete<T> vec;

	vec.x = u.x + v.x;
	vec.y = u.y + v.y;

	return vec;
}

template <class T>
Vector2Templete<T> operator-(const Vector2Templete<T> & u, const Vector2Templete<T> & v)
{
	Vector2Templete<T> vec;

	vec.x = u.x - v.x;
	vec.y = u.y - v.y;

	return vec;
}

template <class T>
Vector2Templete<T> operator*(const Vector2Templete<T> & u, const Vector2Templete<T> & v)
{
	Vector2Templete<T> vec;

	vec.x = u.x * v.x;
	vec.y = u.y * v.y;

	return vec;
}

template <class T>
Vector2Templete<T> operator/(const Vector2Templete<T> & u, const Vector2Templete<T> & v)
{
	Vector2Templete<T> vec;

	vec.x = u.x / v.x;
	vec.y = u.y / v.y;

	return vec;
}

template <class T>
Vector2Templete<T> operator%(const Vector2Templete<T> & u, const Vector2Templete<T> & v)
{
	Vector2Templete<T> vec;

	vec.x = static_cast<T>(static_cast<int>(u.x) % static_cast<int>(v.x));
	vec.y = static_cast<T>(static_cast<int>(u.y) % static_cast<int>(v.y));

	return vec;
}

template <class T>
Vector2Templete<T> operator*(const Vector2Templete<T> & u, T k)
{
	Vector2Templete<T> vec;

	vec.x = u.x * k;
	vec.y = u.y * k;

	return vec;
}

template <class T>
Vector2Templete<T> operator*(T k, const Vector2Templete<T> & vec)
{
	Vector2Templete<T> tmp;

	tmp.x = vec.x * k;
	tmp.y = vec.y * k;

	return tmp;
}

template <class T>
Vector2Templete<T> operator/(const Vector2Templete<T> & vec, T k)
{
	Vector2Templete<T> tmp;

	tmp.x = vec.x / k;
	tmp.y = vec.y / k;

	return tmp;
}

template <class T>
Vector2Templete<T> operator%(const Vector2Templete<T> & vec, int k)
{
	Vector2Templete<T> tmp;

	tmp.x = static_cast<T>(static_cast<int>(vec.x) % k);
	tmp.y = static_cast<T>(static_cast<int>(vec.y) % k);

	return tmp;
}